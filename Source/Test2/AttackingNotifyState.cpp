// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackingNotifyState.h"

#include "MainCharacter.h"


void UAttackingNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			C->OnAttackEnableChanged(true);
		}
	}
	
	
}
void UAttackingNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp)
	{
		AMainCharacter* C =Cast<AMainCharacter>(MeshComp->GetOwner());
		if(C)
		{
			C->OnAttackEnableChanged(false);
		}
	}
}


