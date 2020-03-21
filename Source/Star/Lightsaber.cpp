// Fill out your copyright notice in the Description page of Project Settings.

#include "Lightsaber.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Math/UnrealMathUtility.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "StarCharacter.h"
#include "Bolt.h"
#include "SliceableObject.h"
#include "CloneTrooperOne.h"


// Sets default values
ALightsaber::ALightsaber()
{
	lightsaber_BoxCollision_normal = FVector(1, 1, 1);
	lightsaber_BoxCollision_blocking = FVector(6, 11, 2);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightsaberHiltMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightsaberHiltMesh"));
	RootComponent = LightsaberHiltMesh;

	LightsaberBladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightsaberBladeMesh"));
	LightsaberBladeMesh->SetupAttachment(LightsaberHiltMesh);

	LightsaberBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LightsaberBoxCollision"));
	LightsaberBoxCollision->InitBoxExtent(FVector(1,1,11));
	LightsaberBoxCollision->SetupAttachment(LightsaberBladeMesh);
	LightsaberBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALightsaber::OnOverlapBegin);
	LightsaberBoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALightsaber::OnOverlapEnd);
	
	damage = 10.0f;
}

// Called when the game starts or when spawned
void ALightsaber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightsaber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ALightsaber::LightsaberBlock()
{
	LightsaberBoxCollision->SetRelativeScale3D(lightsaber_BoxCollision_blocking);

}

void ALightsaber::LightsaberUnblock()
{
	LightsaberBoxCollision->SetRelativeScale3D(lightsaber_BoxCollision_normal);
}

void ALightsaber::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	ACloneTrooperOne* TrooperOne = Cast<ACloneTrooperOne>(OtherActor);
	ABolt* Bolt = Cast<ABolt>(OtherActor);
	ASliceableObject* SliceObject = Cast<ASliceableObject>(OtherActor);

	candoDamage = true;

	if (isActivated == true)
	{
		if (OtherActor && (OtherActor != this) && (OtherActor != Player) && OtherComp)
		{
			if (candoDamage == true)
			{
				if (Player->isAttacking == true) // Check to see if the player is attacking
				{
					if (OtherActor == SliceObject)
					{
						SliceObject->canSlce = true;

						UGameplayStatics::PlaySoundAtLocation(this, LightsaberSliceSound, GetActorLocation());
					}
					if (OtherActor == TrooperOne)
					{
						TrooperOne->UpdateHealth(damage);
					}
					

				}
				if (Player->isBlocking == true) // Check to see if the player is blocking
				{

					if (OtherActor == Bolt)
					{	
						
						//Bolt->GetProjectileMovement()->SetVelocityInLocalSpace(Bolt->GetProjectileMovement()->Velocity / 1.0f);
						Bolt->ReturnToEnemy();
						//Bolt->Destroy();
						Player->Deflect();
					}
				}
			}
		}
	}
	
}

void ALightsaber::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ASliceableObject* SliceObject = Cast<ASliceableObject>(OtherActor);
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (OtherActor == SliceObject)
	{
		SliceObject->canSlce = false;
	}
	candoDamage = false;
	
}