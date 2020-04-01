// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StarGameMode.generated.h"

UCLASS(minimalapi)
class AStarGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStarGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget>LightsaberWidgetClass;

	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY()
		class UUserWidget* PauseWidget;

	UPROPERTY()
		class UUserWidget* LightsaberWidget;


	void SetHUD(class UUserWidget* CurrentHUD,class UUserWidget* TargetHUD);

	void PauseGame();

	UFUNCTION(BlueprintCallable)
		void ResumeGame();

	UFUNCTION(BlueprintCallable)
		void OpenLightsaberMenu();

	UFUNCTION(BlueprintCallable)
		void ReturnToPauseMenu();


};



