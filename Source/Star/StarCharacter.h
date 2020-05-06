// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StarCharacter.generated.h"

UCLASS(config=Game)
class AStarCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* LockOnCollision;

public:
	AStarCharacter();

	virtual void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	
	

public:
	//------------Animation Montages----------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ComboAttack1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ComboAttack2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ComboAttack3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ComboAttack4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ComboAttack5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* BlockMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* DeflectMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* ForcePowersMontage;

	//--------------Combat-----------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isAttacking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isActivated;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isBlocking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool saveAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int combocounter;

	void Deflect();

	//------------Combo---------------------------
	//Reset Player Combo
	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	//Save Players Combo Attack
	UFUNCTION(BlueprintCallable)
		void SaveComboAttack();

	//------------Health & Force----------------------
	UFUNCTION(BlueprintPure)
		float GetHealth();
	UFUNCTION(BlueprintPure)
		float GetForce();

	void UpdateHealth(float HealthChange);

	void UpdateForce(float ForceChange);

	//------------Timer-------------------------------
	FTimerHandle Timer;
	
	
	
private:
	//------------Health------------------------------
	float health;
	float maxhealth;
	float healthpercentage;
	float previoushealth;

	//-----------Force--------------------------------
	float force;
	float maxforce;
	float forcepercentage;
	float previousforce;




protected:

	void SetMontage(class UAnimMontage*, float Time);

//-----------Player Lock On Function-------------------
	void LockOnEvent();

//------------Player Movement Functions----------------
	void SetPlayerSpeed(float Speed);

	void StartSprint();

	void StopSprint();
	
	void Roll();

//------------Attack & Block Functions-----------------
	void Attack();

	void TimerToStopAttack();

	void StopAttack();

	void Block();

	void Unblock();


//-----------Force Abilities----------------------------
	void Force();

	void ForcePull();
	void ForcePush();
	

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

