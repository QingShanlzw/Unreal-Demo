// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"

#include "MainCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void UMainAnimInstance::NativeInitializeAnimation()
{ 
	if(Pawn==nullptr)
	{
		Pawn =TryGetPawnOwner();
	}
}
void UMainAnimInstance::UpdateAnimationProperties()
{
	if(Pawn==nullptr)
	{
		//这里把这个类的蓝图放到character的动画类里，就可以获得主角的Pawn了
		Pawn =TryGetPawnOwner();
	}
	if(Pawn)
	{
		FVector Speed =Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X,Speed.Y,0);//这里是不希望获得z轴的变量
		MovementSpeed = LateralSpeed.Size();
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
	//扶着更新变量，bSandIf用来判断人物踢腿还是其他
	AMainCharacter * c = Cast<AMainCharacter>(TryGetPawnOwner());
	if(c)
	{
		
		bIsDance1 = c->IfDance1;
		bIsDance2 = c->IfDance2;
		bStandIf = c->GetStandingMontagePlay();
		//设置角色蹲着走
		bCrouchIf = c->bIsCrouched;
		//设置人物的备战状态
		bReady = c->GetBReady();
		if(bReady)
		{
			
		}
		//用于瞄准偏移，更新角色的看向。
		FRotator v1 =c->GetActorRotation();
		FRotator v2=c->GetControlRotation();
		if(GetWorld())
		{
			float x =GetWorld()->GetDeltaSeconds();
			  z=FMath::RInterpTo(FRotator(0.0f,0.0f,z),v2-v1,x,3).Yaw;
			 //float y1 =FMath::RInterpTo(FRotator(0.0f,Y,z),v2-v1,x,3).Pitch;
			 z= FMath::ClampAngle(z,-90.0f,90.0f);
			 //Y= FMath::ClampAngle(y1,-90.0f,90.0f);
		
		}
		bIsRun = c->bIsRunning;
		bIsDash = c->BIsDash;
	}
	
	
	
}


