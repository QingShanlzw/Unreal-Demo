// Fill out your copyright notice in the Description page of Project Settings.


#include "DashAnimNotifyState.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UDashAnimNotifyState::UDashAnimNotifyState():Super(),
                                             DirectionForce(1.f,0.f,0.f),
                                             ForceSize(10000.f),
                                             TotalDurationConsuming(0.f),
                                             ForceSizeConsuming(0.f)
{
	
}

//在闪现的时候不可以转向，不可以做其他的动作。
void UDashAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	TotalDurationConsuming = TotalDuration;//获取通知时间长度
	ForceSizeConsuming  =ForceSize;//复制编辑器的力
	
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			C->BIsDash = true;
			//C->bUseControllerRotationYaw =false;
			C->DisableInput(C->GetWorld()->GetFirstPlayerController());
		}
	}
}
void UDashAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if(TotalDurationConsuming>0.f)
	{
		if(MeshComp)
		{
			AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
			if(C)
			{
				FVector NewDirection = GetMainCharacterDirection(C);
				C->GetCharacterMovement()->AddForce(ForceSizeConsuming*NewDirection);
				if(ForceSizeConsuming>0.f)
				{
					float  PreSecondFrame  =1.0f/FrameDeltaTime;//每秒帧数
					float TotalFrame = TotalDurationConsuming*PreSecondFrame;//一共会消耗几帧
					float ForceFrameDeltaTimeToLow =  ForceSize/TotalFrame;//没帧减去多少
					ForceSizeConsuming-=ForceFrameDeltaTimeToLow;
				}
			}
		}
	}
}

void UDashAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			C->BIsDash = false;
			//C->bUseControllerRotationYaw =true;
			C->EnableInput(C->GetWorld()->GetFirstPlayerController());
		}
	}
}

FVector UDashAnimNotifyState::GetMainCharacterDirection(ACharacter* C)
{
	FVector V = FVector::ZeroVector;
	if(C)
	{
		if(DirectionForce.X!=0.f)
		{
			V += C->GetActorForwardVector()*DirectionForce.X;
		}
		if(DirectionForce.Z!=0.f)
		{
			V += C->GetActorUpVector()*DirectionForce.Z;
		}
		if(DirectionForce.Y!=0.f)
		{
			V += C->GetActorRightVector()*DirectionForce.Y;
		}
	}
	return V;
}
