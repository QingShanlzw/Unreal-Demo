// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(HUDAsset)
	{
		HUD = CreateWidget<UUserWidget>(this,HUDAsset);
	}
	if(HUD)
	{
		HUD->AddToViewport();//把HUD显示把屏幕.
	}
	if(EnemyHealhBarAsset)
	{
		EnemyHealthBar =CreateWidget<UUserWidget>(this,EnemyHealhBarAsset);
	}
	if(EnemyHealthBar)
	{
		EnemyHealthBar ->SetVisibility(ESlateVisibility::Hidden);
		EnemyHealthBar->AddToViewport();
	}
}

void AMainPlayerController::SetEnemyHealthBarVisibility(bool Visibility)
{
	if(EnemyHealthBar)
	{
		if(Visibility)
		{
			UE_LOG(LogTemp,Warning,TEXT("test bar"));
			EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
void AMainPlayerController::UpdateEnemyHealthBarPosition(FVector Position)
{
	if(EnemyHealthBar)
	{
		FVector2D ScreenPosition;
		FVector TargetPosition = FVector(Position.X,Position.Y,Position.Z+150);
		ProjectWorldLocationToScreen(TargetPosition,ScreenPosition);

		FVector2D ScreenSize(200,25);
		ScreenPosition.X -= ScreenSize.X*0.5f;
		EnemyHealthBar->SetPositionInViewport(ScreenPosition);
		EnemyHealthBar->SetDesiredSizeInViewport(ScreenSize);
		
	}
}

void AMainPlayerController::GameOver1()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("QUIT");
}
void AMainPlayerController::OpenLevel1()
{
	UWorld * World = GetWorld();
	UGameplayStatics::OpenLevel(World,"GameMain1");
}
