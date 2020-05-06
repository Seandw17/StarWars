// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StarGameMode.h"
#include "StarCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AStarGameMode::AStarGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AStarGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AStarGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}