// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "AIController.h"
#include "MainCharacter.h"
#include "MainPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(GetRootComponent());
	DetectSphere->InitSphereRadius(600.f);
	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());
	AttackSphere->InitSphereRadius(300.f);
	RightAttackBox =CreateDefaultSubobject<UBoxComponent>("RightAttackBox");
	RightAttackBox->SetupAttachment(GetMesh(),"rightWeapon1");
	RightAttackBox->SetBoxExtent(FVector(5));

	Health =100.f;
	MaxHealth =100.f;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	if(MainPlayerController)
	{
		UE_LOG(LogTemp,Warning,TEXT("has Get Controller!"));
	}
	AIController  = Cast<AAIController>(GetController());
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::OnDetectSphereOnOverlapBegin);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this,&AEnemy::AEnemy::OnDetectSphereOnOverlapEnd);
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::OnAttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this,&AEnemy::AEnemy::OnAttackSphereOnOverlapEnd);
	RightAttackBox->OnComponentBeginOverlap.AddDynamic(this,&AEnemy::AEnemy::RightBoxOnOverlapBegin);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(MainPlayerController&&TargetMan)
	{
		MainPlayerController->UpdateEnemyHealthBarPosition(GetActorLocation());
	}

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AEnemy::OnDetectSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!TargetMan)
	{
		if(OtherActor)
		{
			AMainCharacter* man = Cast<AMainCharacter>(OtherActor);
			if(man)
			{
				if(MainPlayerController)
				{
					MainPlayerController->SetEnemyHealthBarVisibility(true);
				}
				TargetMan =man;
				if(MoveStatus!=EEnemyMovementStatus::EMS_Attacking)
				{
					MoveStatus = EEnemyMovementStatus::EMS_MoveToTarget;
				}
				MoveToTarget();
			}
		}
	}
}
void AEnemy::OnDetectSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AMainCharacter * man = Cast<AMainCharacter>(OtherActor);
		if(man==TargetMan)
		{
			if(MainPlayerController)
			{
				MainPlayerController->SetEnemyHealthBarVisibility(false);
			}
			TargetMan=nullptr;
			if(MoveStatus!=EEnemyMovementStatus::EMS_Attacking)
			{
				MoveStatus = EEnemyMovementStatus::EMS_Idle;
			}
			if(AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}
void AEnemy::OnAttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!HIttingMan)
	{
		if(OtherActor)
		{
			AMainCharacter * man = Cast<AMainCharacter>(OtherActor);
			if(man)
			{
				HIttingMan = man;
				MoveStatus = EEnemyMovementStatus::EMS_Attacking;
				if(AIController)
				{
					AIController->StopMovement();
				}
			}
		}
	}
}
void AEnemy::OnAttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		AMainCharacter* man = Cast<AMainCharacter>(OtherActor);
		if(man ==HIttingMan)
		{
			TargetMan = HIttingMan;
			HIttingMan  =nullptr;
		}
	}
}
void AEnemy::MoveToTarget()
{
	if(TargetMan)
	{
		if(AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(TargetMan);
			MoveRequest.SetAcceptanceRadius(10);

			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest,&NavPath);
			
		}
	}
	else
	{
		MoveStatus = EEnemyMovementStatus::EMS_Idle;
	}
}

void AEnemy::RightBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AMainCharacter* man = Cast<AMainCharacter>(OtherActor);
		if(man)
		{
			man->Health -=5.f;
		}
	}
}




