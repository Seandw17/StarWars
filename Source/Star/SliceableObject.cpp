// Fill out your copyright notice in the Description page of Project Settings.

#include "SliceableObject.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/PrimitiveComponent.h"



// Sets default values
ASliceableObject::ASliceableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshObject"));
	StaticObjectMesh->bHiddenInGame = true;
	RootComponent = StaticObjectMesh;

	ProceduralObjectMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralObjectMesh"));
	ProceduralObjectMesh->bUseComplexAsSimpleCollision = false;
	ProceduralObjectMesh->SetupAttachment(StaticObjectMesh);
	
	PhysicsComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PrimitiveComponent"));
	
}

// Called when the game starts or when spawned
void ASliceableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASliceableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASliceableObject::Push()
{
	FVector Force = FVector(1000, 1000, 1000);
	
}