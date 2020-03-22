// Fill out your copyright notice in the Description page of Project Settings.

#include "CloneTrooperOne.h"
#include "GameFramework\PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "StarCharacter.h"
#include "Bolt.h"

// Sets default values
ACloneTrooperOne::ACloneTrooperOne()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshHolder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BlasterHolder"));

	LockonSphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockonSphere"));
	

	BlasterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlasterMesh"));
	BlasterMesh->SetupAttachment(MeshHolder);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(BlasterMesh);

	PhysicsComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PrimitiveComponent"));
	PhysicsComponent->SetupAttachment(GetMesh());
	
	maxhealth = 100.0f;
	health = maxhealth;
	healthpercentage = 1.0f;
	previoushealth = healthpercentage;

	GunOffSet = FVector(00.0f, 0.0f, 10.0f);

	
}

// Called when the game starts or when spawned
void ACloneTrooperOne::BeginPlay()
{
	Super::BeginPlay();
	MeshHolder->AttachToComponent(GetMesh(), FAttachmentTransformRules(FAttachmentTransformRules::SnapToTargetIncludingScale), TEXT("Blaster"));
	LockonSphereMesh->SetActive(false);
}

// Called every frame
void ACloneTrooperOne::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (health <= 0)
	{
		Death();
	}

}

// Called to bind functionality to input
void ACloneTrooperOne::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ACloneTrooperOne::DistanceToPlayer()
{
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	FHitResult OutHit;

	GetWorld()->LineTraceSingleByObjectType(OutHit, GetActorLocation(), Player->GetActorLocation(), FCollisionObjectQueryParams(ECC_Pawn), FCollisionQueryParams(TEXT("IKTrace"), false, this));

	playerdistancetoenemy = OutHit.Distance;
	bcanbeTargeted = true;
}


float ACloneTrooperOne::GetHealth()
{
	return healthpercentage;
}

void ACloneTrooperOne::UpdateHealth(float HealthChange)
{
	health -= HealthChange;
	health = FMath::Clamp(health, 0.0f, maxhealth);
	previoushealth = healthpercentage;
	healthpercentage = health / maxhealth;
}


void ACloneTrooperOne::SetMontage(class UAnimMontage* Montage, float Time)
{
	if (Montage != NULL)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(Montage, Time);
		}
	}
}

void ACloneTrooperOne::OnFire()
{
	
	if (BoltClass != NULL)
	{
		PlayAnimMontage(FireAnimation, 1.0f);
		UWorld* const World = GetWorld();

		const FRotator SpawnRotation = GetActorRotation();

		const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffSet);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the bolt at the muzzle
		World->SpawnActor<ABolt>(BoltClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		
		
	}

	if (BlasterSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BlasterSound, GetActorLocation());
	}
	
}

void ACloneTrooperOne::Push()
{
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//FVector Force = FVector(GetActorLocation().X - Player->GetActorLocation().X, GetActorLocation().Y - Player->GetActorLocation().Y, 5);
	FVector Force = FVector(GetActorLocation().X - Player->GetActorLocation().X, GetActorLocation().Y - Player->GetActorLocation().Y, 5);
	//MeshObject->AddImpulseAtLocation(GetActorLocation() - Player->GetActorLocation() * 250000);
	Force.Normalize();
	LaunchCharacter(Force * 2500, false, false);
}

void ACloneTrooperOne::Pull()
{
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	FVector pullVector = rotate.Vector();
	pullVector.Normalize();

	LaunchCharacter(pullVector * 2500, false, false);

}

void ACloneTrooperOne::Death()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetGenerateOverlapEvents(true);
	//GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("Armature"), true, true);
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("Death"));
	
}