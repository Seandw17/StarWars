// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "StarGameMode.h"
#include "StarCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
	if (PauseWidgetClass != nullptr)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass);
	}
	if (LightsaberWidgetClass != nullptr)
	{
		LightsaberWidget = CreateWidget<UUserWidget>(GetWorld(), LightsaberWidgetClass);
	}
}

void AStarGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStarGameMode::SetHUD(class UUserWidget* CurrentHUD, class UUserWidget* TargetHUD)
{
	CurrentHUD->RemoveFromParent();
	if (TargetHUD)
	{
		TargetHUD->AddToViewport();
	}

}

void AStarGameMode::PauseGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AStarGameMode::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	SetHUD(PauseWidget, CurrentWidget);
}

void AStarGameMode::OpenLightsaberMenu()
{
	SetHUD(PauseWidget, LightsaberWidget);
}

void AStarGameMode::ReturnToPauseMenu()
{
	SetHUD(LightsaberWidget, PauseWidget);
}
