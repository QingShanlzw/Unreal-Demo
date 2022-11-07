// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Enemy.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


const float AMainCharacter::ExitReadyTime =4;
// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength=600.f;
	CameraBoom->bUsePawnControlRotation=true;//有了这个就可以实现旋转等。使弹簧臂可以跟着旋转

	GetCapsuleComponent()->SetCapsuleSize(48.f,150.f);//设置胶囊体的大小，这个其实可以在角色BP里面快速设置
	
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera  ->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera ->bUsePawnControlRotation =false;//不要让相机跟着转

	//设置初始的转向速度
	BaseTurnRate =65.f;
	BaseLookUPRate=65.f;

	//控制器旋转时角色不要跟着转,摄像机转，角色不转
	bUseControllerRotationYaw =false;
	bUseControllerRotationPitch =false;
	bUseControllerRotationRoll =false;

	GetCharacterMovement()->bOrientRotationToMovement = true;//使角色可以沿着运动的方向转向。
	GetCharacterMovement()->RotationRate = FRotator(0.0f,540.f,0.0f);//沿着这个速率转，只有左右方向
	GetCharacterMovement()->JumpZVelocity =650.f;
	GetCharacterMovement()->AirControl =0.2f;   //角色在空中的控制

	
	static  ConstructorHelpers::FObjectFinder<USoundBase> AttackSoundObject(TEXT("SoundWave'/Game/charaterAsset/Sound/attackSound1.attackSound1'"));
	if(AttackSoundObject.Succeeded())
	{
		
		AttackSound = AttackSoundObject.Object;
	}
	static  ConstructorHelpers::FObjectFinder<UDataTable> AttackMontageDataTableObject(TEXT("DataTable'/Game/charaterAsset/DataTables/AttackMontageDataTables.AttackMontageDataTables'"));
	if(AttackMontageDataTableObject.Succeeded())
	{
		
		AttackMontageDataTable = AttackMontageDataTableObject.Object;
	}
	AttackAudioComp =CreateDefaultSubobject<UAudioComponent>(TEXT("AttackAudioComp"));
	AttackAudioComp->SetupAttachment(GetRootComponent());
	AttackAudioComp->bAutoActivate =false;
	if(AttackSound)
	{
		AttackAudioComp->SetSound(AttackSound);
	}
	LeftAttackBox =CreateDefaultSubobject<UBoxComponent>("LeftAttackBox");
	LeftAttackBox->SetupAttachment(GetMesh(),"hand_l_socket");
	LeftAttackBox->SetCollisionProfileName("WeaponP");
	LeftAttackBox->SetNotifyRigidBodyCollision(true);
	LeftAttackBox->SetBoxExtent(FVector(5));
	RightAttackBox =CreateDefaultSubobject<UBoxComponent>("RightAttackBox");
	RightAttackBox->SetupAttachment(GetMesh(),"hand_r_socket");
	RightAttackBox->SetCollisionProfileName("WeaponP");
	RightAttackBox->SetNotifyRigidBodyCollision(true);
	RightAttackBox->SetBoxExtent(FVector(5));
	bStandingMontagePlay =false;
	bReady = false;

	WalkingSpeed =300;
	RunningSpeedFactor =2;
	ReadyWalkSpeedFactor =0.6f;

	CrouchSpeed=300;
	ReadyCrouchSpeedFactor =0.5f;

	bIsRunning =false;
	bTargetAttacked =false;
	AttackColdTime = 1;
	BIsDash =false;

	Health =100.f;
	MaxHealth = 100.f;
	EnemyHealth = 100.f;

	IfDance1 = false;
	IfDance2  =false;

	
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	LeftAttackBox->OnComponentHit.AddDynamic(this,&AMainCharacter::OnAttackHit);
	RightAttackBox->OnComponentHit.AddDynamic(this,&AMainCharacter::OnAttackHit);

	LeftAttackBox->OnComponentBeginOverlap.AddDynamic(this,&AMainCharacter::HitBoxOnOverlapBegin);
	RightAttackBox->OnComponentBeginOverlap.AddDynamic(this,&AMainCharacter::HitBoxOnOverlapBegin);

	//初始化速度
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GameOver();

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	//Punch,Kick,Crouch,Run,Dash,Dance
	PlayerInputComponent->BindAction("Punch",IE_Pressed,this,&AMainCharacter::Punch);
	PlayerInputComponent->BindAction("Kick",IE_Pressed,this,&AMainCharacter::Kick);
	PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&AMainCharacter::CrouchStart);
	PlayerInputComponent->BindAction("Run",IE_Pressed,this,&AMainCharacter::RunStart);
	PlayerInputComponent->BindAction("Run",IE_Released,this,&AMainCharacter::RunEnd);
	PlayerInputComponent->BindAction("Dash",IE_Released,this,&AMainCharacter::Dash);
	PlayerInputComponent->BindAction("Dance1",IE_Released,this,&AMainCharacter::Dance1);
	PlayerInputComponent->BindAction("Dance2",IE_Released,this,&AMainCharacter::Dance2);
	//Jump
	PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released,this,&ACharacter::StopJumping);
	//添加wasd 移动
	PlayerInputComponent->BindAxis("MoveForward",this,&AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&AMainCharacter::MoveRight);
	//添加 鼠标控制镜头旋转
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	//  添加 上下左右控制镜头旋转
	PlayerInputComponent->BindAxis("TurnRate",this,&AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate",this,&AMainCharacter::LookUpAtRate);

}
void AMainCharacter::MoveForward(float value)
{
	if(Controller!=nullptr&&value!=0.0f&&!bStandingMontagePlay)
	{
		//找到前放向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f,Rotation.Yaw,0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction,value);
	}
}
void AMainCharacter::MoveRight(float value)
{
	if(Controller!=nullptr&&value!=0.0f&&!bStandingMontagePlay)
	{
		//找到前放向
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f,Rotation.Yaw,0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction,value);
	}
}

void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate*BaseTurnRate*GetWorld()->GetDeltaSeconds());
}
void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate*BaseLookUPRate*GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Punch()
{
	Attack(EAttackType::PUNCH);
}
void AMainCharacter::OnAttackEnableChanged(bool Enable)
{
	if(Enable)
	{
		LeftAttackBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RightAttackBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		UpdateMovementProp();
		bTargetAttacked =false;
		UWorld* World = GetWorld();
		if(World)
		{
			//这里是防止再这个动作没做完，但是计时器以经结束了，加个0.2秒，让动作再做一下。
			if(!World->GetTimerManager().IsTimerActive(AttackColdTimer))
			{
				World->GetTimerManager().SetTimer(AttackColdTimer,0.2f,false);
			}
		}
		bStandingMontagePlay =false;
		LeftAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightAttackBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
void AMainCharacter::OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bTargetAttacked)
	{
		return;
	}
	bTargetAttacked =true;
	if(AttackAudioComp)
	{
		AttackAudioComp->SetPitchMultiplier(FMath::RandRange(.5f,4.f));//随机数来设置音调
		AttackAudioComp->Play();
		
	}
}
void AMainCharacter::Kick()
{
	Attack(EAttackType::KICK);
}


void AMainCharacter::Attack(EAttackType Type)
{
	if(Type!=EAttackType::Dash)
	{
		
		UWorld *World = GetWorld();
		if(World)
		{
			if(World->GetTimerManager().IsTimerActive(AttackColdTimer))
			{
				return;
			}
			World->GetTimerManager().SetTimer(AttackColdTimer,AttackColdTime,false);
		}
		if(!bReady)
		{
			ReadyToFight();
			UpdateMovementProp();
		}
		else
		{
			if(World)
			{
				//重新计时，按照最后一次的攻击来计算攻击状态。
				World->GetTimerManager().SetTimer(ExitReadyTimer,this,&AMainCharacter::ExitReadyTimeout,ExitReadyTime);
			}
		}
	}
	if(AttackMontageDataTable)
	{
		FName RowName;
		switch (Type)
		{
		case EAttackType::PUNCH:
			LeftAttackBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,"hand_l_socket");
			RightAttackBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,"hand_r_socket");
			RowName ="Punch";
			break;
		case EAttackType::KICK:
			LeftAttackBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,"foot_l_socket");
			RightAttackBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,"foot_r_socket");
			bStandingMontagePlay =true;
			RowName ="Kick";
			break;
		case EAttackType::Dash:
			RowName = "Dash";
			BIsDash=true;
			break;
		default:break;		
		}
		FAttcakMontage* AttackMontage=AttackMontageDataTable->FindRow<FAttcakMontage>(RowName,TEXT("Montage"));
		if(AttackMontage&&AttackMontage->Montage)
		{
			int32 Count  = AttackMontage->Montage->CompositeSections.Num();//蒙太奇的这个数组大小代表里面的片段个数
			if(Count>0)
			{
				int32 Id = FMath::Rand()%Count;
				PlayAnimMontage(AttackMontage->Montage,1,AttackMontage->Montage->CompositeSections[Id].SectionName);
			}
		}
	}
	if(Type!=EAttackType::Dash)
	{
		
		//再punch/kick是不允许转身
		GetCharacterMovement() ->bUseControllerDesiredRotation =false;
	}
}

void AMainCharacter::CrouchStart()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}
void AMainCharacter::ReadyToFight()
{
	bReady =!bReady;
	UWorld*World = GetWorld();
	if(World)
	{
		World->GetTimerManager().ClearTimer(ExitReadyTimer);
		if(bReady)
		{
			World->GetTimerManager().SetTimer(ExitReadyTimer,this,&AMainCharacter::ExitReadyTimeout,ExitReadyTime);
		}
	}
}
void AMainCharacter::ExitReadyTimeout()
{
	bReady =false;
	UpdateMovementProp();
}
void AMainCharacter::UpdateMovementProp()
{
	//更新速度
	if(bReady)
	{
		GetCharacterMovement()->MaxWalkSpeed=WalkingSpeed* ReadyWalkSpeedFactor;
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed*ReadyCrouchSpeedFactor;
	}
	else
	{
		if(bIsRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed =WalkingSpeed*RunningSpeedFactor;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed =WalkingSpeed;
		}
		GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;
	}
	//更新运动方式
	GetCharacterMovement()->bUseControllerDesiredRotation =!bIsCrouched&&bReady;
	GetCharacterMovement()->bOrientRotationToMovement = bIsCrouched||!bReady;
}
void AMainCharacter::RunStart()
{
	bIsRunning =true;
	if(bReady)
	{
		ReadyToFight();//如果是攻击状态，则会自动解除攻击状态；
	}
	UpdateMovementProp();
}
void AMainCharacter::RunEnd()
{
	bIsRunning =false;
	UpdateMovementProp();
}
void AMainCharacter::Dash()
{
	Attack(EAttackType::Dash);
}
void AMainCharacter::HitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AEnemy * enemy = Cast<AEnemy>(OtherActor);
		if(enemy)
		{
			
			if(AttackAudioComp)
			{
				AttackAudioComp->SetPitchMultiplier(FMath::RandRange(.5f,4.f));//随机数来设置音调
				AttackAudioComp->Play();
		
			}
			enemy->Health -=10.f;
			EnemyHealth = enemy->Health;
			UE_LOG(LogTemp,Warning,TEXT("enemy health = %f"),enemy->Health);
		}
	}
}
void AMainCharacter::Dance1()
{
	IfDance1 = !IfDance1;
}
void AMainCharacter::Dance2()
{
	IfDance2 = !IfDance2;
}

void AMainCharacter::GameOver()
{
	if(EnemyHealth<=0.f)
	{
		UWorld * World = GetWorld();
		UGameplayStatics::OpenLevel(World,"GameEnd");
	}
}

