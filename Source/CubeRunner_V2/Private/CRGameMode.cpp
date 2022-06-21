// Fill out your copyright notice in the Description page of Project Settings.


#include "CRGameMode.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ACRGameMode::ACRGameMode()
{
	CurrentLevelIndex = 0;
}


void ACRGameMode::BeginPlay()
{
	Super::BeginPlay();

	ControllerRef = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly InputMode;
	ControllerRef->SetInputMode(InputMode);
	ControllerRef->bShowMouseCursor = false;

	CheckLevel();
}

//
void ACRGameMode::CheckLevel()
{
	FString CurrentLevelName = GetWorld()->GetMapName();

	Levels.Find(CurrentLevelName, CurrentLevelIndex);

	if(CurrentLevelIndex < Levels.Num() - 1)
	{
		NextLevel = Levels[CurrentLevelIndex + 1];
	}
	else
	{
		NextLevel = "End";
	}
	
}

//
void ACRGameMode::EndGame()
{
	FString LevelString = GetWorld()->GetMapName();
	FName LevelToLoad = FName(*LevelString);

	UGameplayStatics::OpenLevel(this, LevelToLoad, true);
}

//
void ACRGameMode::LevelComplete()
{
	if(DefaultLevelCompleteWidget)
	{
		LevelCompleteWidget = CreateWidget<UUserWidget>(GetWorld(),DefaultLevelCompleteWidget);
		LevelCompleteWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No default Level Complete Widget Selected"));
	}

	GetWorldTimerManager().SetTimer(LevelSwapTimer, this, &ACRGameMode::LoadNextLevel, 2.0f, false);
}

//
void ACRGameMode::LoadNextLevel()
{
	if(Levels.Contains(NextLevel))
	{
		FName LevelToLoad = FName(*NextLevel);
		UGameplayStatics::OpenLevel(this, LevelToLoad, true);
	}
	else
	{
		if(LevelCompleteWidget)
		{
			LevelCompleteWidget->RemoveFromParent();

			if (DefaultGameCompleteWidget)
			{
				GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefaultGameCompleteWidget);
				GameCompleteWidget->AddToViewport();

				ControllerRef->bShowMouseCursor = true;
				FInputModeUIOnly InputMode;
				ControllerRef->SetInputMode(InputMode);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("No default Game Complete Widget Selected"));
			}
		}
	}
}



