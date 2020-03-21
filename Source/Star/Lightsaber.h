// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lightsaber.generated.h"

UCLASS()
class STAR_API ALightsaber : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LightsaberHiltMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LightsaberBladeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* LightsaberBoxCollision;

public:	
	// Sets default values for this actor's properties
	ALightsaber();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isActivated;
	
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable)
	void LightsaberBlock();
	UFUNCTION(BlueprintCallable)
		void LightsaberUnblock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* LightsaberSliceSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool candoDamage;
	float damage;
	
	FQuat originalRotation;

	FVector lightsaber_BoxCollision_blocking;
	FVector lightsaber_BoxCollision_normal;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
