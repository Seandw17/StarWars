// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPushObject.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "StarCharacter.h"
#include "TimerManager.h"
#include "Engine/World.h"

// Sets default values
ATestPushObject::ATestPushObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshObject;

	PhysicsComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PrimitiveComponent"));
	PhysicsComponent->SetupAttachment(MeshObject);

	
}

// Called when the game starts or when spawned
void ATestPushObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPushObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	
	
}

void ATestPushObject::push()
{


	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//FVector Force = FVector(GetActorLocation().X - Player->GetActorLocation().X, GetActorLocation().Y - Player->GetActorLocation().Y, 5);
	FVector Force = FVector(GetActorLocation().X - Player->GetActorLocation().X, GetActorLocation().Y - Player->GetActorLocation().Y, 5);
	//MeshObject->AddImpulseAtLocation(GetActorLocation() - Player->GetActorLocation() * 250000);
	Force.Normalize();
	MeshObject->AddImpulse(Force * 250000);
	
}

void ATestPushObject::pull()
{
	//No WORK FIX THIS SHIT
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	//FVector Force = FVector(GetActorLocation().X - Player->GetActorLocation().X, GetActorLocation().Y - Player->GetActorLocation().Y, 5);
	//FVector pullvector = FVector(GetActorForwardVector().X - Player->GetActorForwardVector().X, GetActorForwardVector().Y - Player->GetActorForwardVector().Y, 2);
	
	FRotator rotate = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	FVector test = rotate.Vector();
	//Force.Normalize();
	//pullvector.Normalize();
	//MeshObject->AddImpulse(Force.ForwardVector * 250000);
	test.Normalize();
	MeshObject->AddImpulse( test * 250000);
}

void ATestPushObject::Lift()
{
	FVector Force = FVector(0.0f, 0.0, 15.0);
	
	MeshObject->AddImpulse(Force * 10000);
	
	
}

