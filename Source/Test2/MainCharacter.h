// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "MainCharacter.generated.h"
USTRUCT(BlueprintType)
struct FAttcakMontage:public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	class UAnimMontage*Montage;
	UPROPERTY(EditAnywhere)
	FString Des;
	
};

UENUM(BlueprintType)
enum class EAttackType:uint8
{
	PUNCH	UMETA(DiaplayName ="Punch"),
	KICK	UMETA(DiaplayName ="Kick"),
	Dash    UMETA(Diaplayname ="Dash"),
	BigSkill UMETA(DiaplayName= "BigSkill")

};


UCLASS()
class TEST2_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
	/** positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess="true"))
	class USpringArmComponent * CameraBoom;
	/** Follow Camera*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera,meta=(AllowPrivateAccess="true"))
	class UCameraComponent* FollowCamera;
	/** 调节相机基本转向速度*/
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Camera)
	float BaseLookUPRate;

	bool bIsRunning;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Health;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float EnemyHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IfDance1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IfDance2;
	UFUNCTION(BlueprintCallable)
	void Dance1();
	UFUNCTION(BlueprintCallable)
	void Dance2();
	void GameOver();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Punch();
	void Kick();
	void Attack(EAttackType Type);
	UFUNCTION()
	void OnAttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	bool bStandingMontagePlay;
	void CrouchStart();
	
	bool bReady;
	void ReadyToFight();
	static  const float ExitReadyTime;
	FTimerHandle ExitReadyTimer;
	void ExitReadyTimeout();
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float WalkingSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float RunningSpeedFactor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float ReadyWalkSpeedFactor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float CrouchSpeed;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float ReadyCrouchSpeedFactor;

	

	void RunStart();
	void RunEnd();
	void Dash();
	void UpdateMovementProp();
	bool bTargetAttacked;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Movement)
	float AttackColdTime;

	FTimerHandle AttackColdTimer;
private:
	//蒙太奇
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= Animation,meta=(AllowPrivateAccess="true"))
	class UAnimMontage* PunchMontage;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Animation,meta=(AllowPrivateAccess="true"))
	UDataTable* AttackMontageDataTable;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Collision,meta=(AllowPrivateAccess="true"))
	class UBoxComponent* LeftAttackBox;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category=Collision,meta=(AllowPrivateAccess="true"))
	class UBoxComponent* RightAttackBox;
	//声音资源
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category=Audio,meta=(AllowPrivateAccess="true"))
	USoundBase* AttackSound;
	//控制声音播放的组件
	class UAudioComponent* AttackAudioComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//角色的前后左右移动
	void MoveForward(float value);
	void MoveRight(float value);

	//角色是否可以Dash;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Dash)
	bool BIsDash;

	/** Called via input to turn at a given rate
* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
*/
	void TurnAtRate(float Rate);

	/** Called via input to look up/down at a given rate
	* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired look up/down rate
	*/
	void LookUpAtRate(float Rate);

	void OnAttackEnableChanged(bool Enable);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetStandingMontagePlay()const{return bStandingMontagePlay;}
	//通知当前状态是fist还是一般状态
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetBReady()const{return bReady;}

	UFUNCTION()
	virtual void HitBoxOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	 
	UFUNCTION(BlueprintCallable)
	FORCEINLINE  float GetEnemyHealth() const { return EnemyHealth; }
	
};
