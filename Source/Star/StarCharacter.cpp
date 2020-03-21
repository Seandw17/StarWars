// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StarCharacter.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Lightsaber.h"
#include "SliceableObject.h"
#include "TestPushObject.h"
#include "CloneTrooperOne.h"

//////////////////////////////////////////////////////////////////////////
// AStarCharacter

AStarCharacter::AStarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	RootComponent = GetCapsuleComponent();

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Create a box component
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->InitBoxExtent(FVector(1, 1, 1));


	//-----------------Player Variables--------------------------------------------
	
	//-----------------Health Variables--------------------------------------------
	maxhealth = 100.0f; // Set the players max health to 100
	health = maxhealth; // Set the players health to max health
	healthpercentage = 1.0f; // Set the percentage value to 1 so that it is full
	previoushealth = healthpercentage; // Set the previoushealth to the healthpercentage

	//-----------------Force Variables---------------------------------------------
	maxforce = 100.0f; // Set the players max force to 100
	force = maxforce; // Set the players force to max force
	forcepercentage = 1.0f; // Set the percentage value to 1 so that it is full
	previousforce = forcepercentage; // Set the previousforce to the forcepercentage
}



//-----------------------Tick-------------------------------------------------

void AStarCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void AStarCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AStarCharacter::Attack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &AStarCharacter::TimerToStopAttack);

	PlayerInputComponent->BindAction("Pull", IE_Pressed, this, &AStarCharacter::ForcePull);
	PlayerInputComponent->BindAction("Push", IE_Pressed, this, &AStarCharacter::ForcePush);

	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AStarCharacter::Block);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &AStarCharacter::Unblock);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &AStarCharacter::Roll);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AStarCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AStarCharacter::StopSprint);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStarCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStarCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AStarCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AStarCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AStarCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AStarCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AStarCharacter::OnResetVR);
}


void AStarCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AStarCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AStarCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AStarCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStarCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStarCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AStarCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AStarCharacter::SetMontage(class UAnimMontage* Montage, float Time)
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

float AStarCharacter::GetHealth()
{
	return healthpercentage;
}

void AStarCharacter::UpdateHealth(float HealthChange)
{
	health -= HealthChange; // Minus the value set when the function is called 
	health = FMath::Clamp(health, 0.0f, maxhealth); // Wont allow the health to go above the max health
	previoushealth = healthpercentage; // Set the previous health to be the current health percentage
	healthpercentage = health / maxhealth; // Set the health percentage to be the percentage of health divded by max health
}

void AStarCharacter::UpdateForce(float ForceChange)
{
	force -= ForceChange; // Minus the value set when the function is called
	force = FMath::Clamp(force, 0.0f, maxforce); // Wont allow force to go above the max force
	previousforce = forcepercentage; // Set the previousforce to be the current forcepercentage
	forcepercentage = force / maxforce; // Set the force percentage to be the percentage of the force divded by max force
}

float AStarCharacter::GetForce()
{
	return forcepercentage;
}

void AStarCharacter::SetPlayerSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
	
}

void AStarCharacter::StartSprint()
{
	SetPlayerSpeed(600.0f);
}

void AStarCharacter::StopSprint()
{
	SetPlayerSpeed(300.0f);
}

void AStarCharacter::Roll()
{
	SetMontage(RollMontage, 1.0f);
}

void AStarCharacter::Block()
{
	
	SetMontage(BlockMontage, 1.0f);
	isBlocking = true;
}

void AStarCharacter::Unblock()
{
	StopAnimMontage(BlockMontage);
	isBlocking = false;
}

void AStarCharacter::Deflect()
{
	Unblock();
	SetMontage(DeflectMontage, 0.5f);
}

void AStarCharacter::ResetCombo()
{
	combocounter = 0;
	saveAttack = false;
	isAttacking = false;
}

void AStarCharacter::SaveComboAttack()
{
	if (saveAttack == true)
	{
		saveAttack = false;
	}
}

void AStarCharacter::Attack()
{
	
	isAttacking = true;
	combocounter += 1;
	switch (combocounter)
	{
		case 1:
		{
			SetMontage(ComboAttack1, 1.5f);
		}
		break;
		case 2:
		{
			SetMontage(ComboAttack2, 1.5f);
		}
		break;
		case 3:
		{
			SetMontage(ComboAttack3, 1.5f);
		}
		break;
		case 4:
		{
			SetMontage(ComboAttack4, 1.5f);
		}
		break;
		case 5:
		{
			SetMontage(ComboAttack5, 1.5f);
		}
		break;

	}
	if (combocounter > 4)
	{
		combocounter = 1;
	}
	
}

void AStarCharacter::TimerToStopAttack()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, 1.0f, &AStarCharacter::StopAttack, 0.0f);
}

void AStarCharacter::StopAttack()
{
	isAttacking = false;
}

void AStarCharacter::Force()
{
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	BoxCollision->GetOverlappingActors(CollectedActors);

	//for each actor we collected
	for (int i = 0; i < CollectedActors.Num(); i++)
	{
		//Cast the actor to Sliceable object
		ATestPushObject* const SliceableObject = Cast<ATestPushObject>(CollectedActors[i]);
		ACloneTrooperOne* const CloneObject = Cast<ACloneTrooperOne>(CollectedActors[i]);

		SetMontage(ForcePowersMontage, 1.0f);
	}
	
}

void AStarCharacter::ForcePush()
{
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	BoxCollision->GetOverlappingActors(CollectedActors);

	//for each actor we collected
	for (int i = 0; i < CollectedActors.Num(); i++)
	{
		//Cast the actor to Sliceable object
		ATestPushObject* const SliceableObject = Cast<ATestPushObject>(CollectedActors[i]);
		ACloneTrooperOne* const CloneObject = Cast<ACloneTrooperOne>(CollectedActors[i]);
		

		if (SliceableObject)
		{
			SliceableObject->push();
		}
		if (CloneObject)
		{
			CloneObject->Push();
		}
		

		
		SetMontage(ForcePowersMontage, 1.0f);
	}
}

void AStarCharacter::ForcePull()
{
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	BoxCollision->GetOverlappingActors(CollectedActors);

	//for each actor we collected
	for (int i = 0; i < CollectedActors.Num(); i++)
	{
		//Cast the actor to Sliceable object
		ATestPushObject* const SliceableObject = Cast<ATestPushObject>(CollectedActors[i]);
		ACloneTrooperOne* const CloneObject = Cast<ACloneTrooperOne>(CollectedActors[i]);
		

		if (SliceableObject)
		{
			SliceableObject->pull();
		}
		if (CloneObject)
		{
			CloneObject->Pull();
		}
		


		SetMontage(ForcePowersMontage, 1.0f);
	}
}
