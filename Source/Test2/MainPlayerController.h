// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TEST2_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget>HUDAsset;

	UUserWidget* HUD;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget>EnemyHealhBarAsset;

	UUserWidget* EnemyHealthBar;

	void SetEnemyHealthBarVisibility(bool Visibility);
	void UpdateEnemyHealthBarPosition(FVector Position);

	
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void OpenLevel1();
	UFUNCTION(BlueprintCallable)
	void GameOver1();
};
