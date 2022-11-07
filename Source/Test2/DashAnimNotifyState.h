// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DashAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class TEST2_API UDashAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UDashAnimNotifyState();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="dash")
	FVector DirectionForce;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="dash")
	float ForceSize;
	float TotalDurationConsuming;//通知的时间。
	float ForceSizeConsuming;//残值，从满道0；
	FVector GetMainCharacterDirection(ACharacter * C);
	

	
	void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

};
