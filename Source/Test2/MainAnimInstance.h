// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST2_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual  void NativeInitializeAnimation() override;
	UFUNCTION(BlueprintCallable,Category=AnimationsProperties)
	void UpdateAnimationProperties();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float MovementSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	bool bIsInAir;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	class APawn* Pawn;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bStandIf;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCrouchIf;
	//人物的备战状态
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bReady;
	//看看主角是否在跑
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsRun;
	//看看主角是否在Dash
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsDash;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float z ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Y ;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    bool bIsDance1;
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
     bool bIsDance2;

	
	

	
	
};
