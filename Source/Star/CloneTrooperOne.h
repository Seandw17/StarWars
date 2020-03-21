// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LockOnInterface.h"
#include "CloneTrooperOne.generated.h"

UCLASS()
class STAR_API ACloneTrooperOne : public ACharacter, public ILockOnInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* MeshHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* LockonSphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MuzzleLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPrimitiveComponent* PhysicsComponent;

public:
	// Sets default values for this character's properties
	ACloneTrooperOne();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* BlasterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector GunOffSet;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABolt> BoltClass;

	UFUNCTION(BlueprintCallable)
		void OnFire();

	
//---------------EnemyAnimations-----------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAnimMontage* FireAnimation;

//---------------Health--------------------------
	UFUNCTION(BlueprintPure)
		float GetHealth();

	void UpdateHealth(float HealthChange);
	void Push();
	void Pull();

	void Death();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int playerdistancetoenemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bcanbeTargeted;
	
private:
	float health;
	float maxhealth;
	float healthpercentage;
	float previoushealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetMontage(class UAnimMontage*, float Time);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	


	
};
