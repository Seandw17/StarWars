// Fill out your copyright notice in the Description page of Project Settings.

#include "Bolt.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StarCharacter.h"
#include "CloneTrooperOne.h"
#include "Engine/World.h"
// Sets default values
ABolt::ABolt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	BoltCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoltCollision"));
	BoltCollision->InitBoxExtent(FVector(1, 1, 1));
	BoltCollision->OnComponentHit.AddDynamic(this, &ABolt::OnHit);
	RootComponent = BoltCollision;
	

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = BoltCollision;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->InitialSpeed = ProjectileMovement->MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;


}

// Called when the game starts or when spawned
void ABolt::BeginPlay()
{
	Super::BeginPlay();
	startinglocationVector = GetActorLocation();
	startingrotationRotator = GetActorRotation();
}

// Called every frame
void ABolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABolt::ReturnToEnemy(FVector boltVector, FRotator boltRotator)
{
	ProjectileMovement->MoveInterpolationTarget(boltVector, boltRotator); //Gets the location and rotation of the bolt
	ProjectileMovement->Velocity = ProjectileMovement->Velocity * -1.0f; //Reverses the velocity to send the bolt back
}

void ABolt::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AStarCharacter* Player = Cast<AStarCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	ACloneTrooperOne* TrooperOne = Cast<ACloneTrooperOne>(OtherActor);

	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) &&(OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		
		
		Destroy();
		
	}
	if (OtherActor == Player)
	{
		Player->UpdateHealth(10);
		Player->BoltDamaged();
		Destroy();
	}
	if (OtherActor == TrooperOne)
	{
		TrooperOne->UpdateHealth(10);
		Destroy();
	}
	
	Destroy();
}

