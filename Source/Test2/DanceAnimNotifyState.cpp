// Fill out your copyright notice in the Description page of Project Settings.


#include "DanceAnimNotifyState.h"
#include "MainCharacter.h"

void UDanceAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			//C->bUseControllerRotationYaw =false;
			C->DisableInput(C->GetWorld()->GetFirstPlayerController());
		}
	}
}
void UDanceAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			//C->bUseControllerRotationYaw =true;
			C->EnableInput(C->GetWorld()->GetFirstPlayerController());
			
		}
	}
}

