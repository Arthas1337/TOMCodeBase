// Copyright Epic Games, Inc. All Rights Reserved.

#include "TealesOfMeadowGameMode.h"
#include "TealesOfMeadowCharacter.h"
#include "Player/TOMPlayerController.h"
#include "UI/TOMPlayerHUD.h"
#include "UObject/ConstructorHelpers.h"

//-------------------------------------------------------------------------------------------------------------
ATealesOfMeadowGameMode::ATealesOfMeadowGameMode()
{
    DefaultPawnClass = ATealesOfMeadowCharacter::StaticClass();
    PlayerControllerClass = ATOMPlayerController::StaticClass();
    HUDClass = ATOMPlayerHUD::StaticClass();
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowGameMode::StartPlay() 
{
    Super::StartPlay();

    SetMatchState(ETOMGameState::InProgress);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowGameMode::SetMatchState(ETOMGameState State)
{
   if(GameState == State) return;

   GameState = State;
   OnGameChangeState.Broadcast(State);
}
//-------------------------------------------------------------------------------------------------------------
bool ATealesOfMeadowGameMode::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    const auto PauseState = Super::SetPause(PC, CanUnpauseDelegate);
    if(PauseState)
    {
        SetMatchState(ETOMGameState::Pause);
    }

    return PauseState;
}
//-------------------------------------------------------------------------------------------------------------
bool ATealesOfMeadowGameMode::ClearPause()
{
    const auto PauseCleared = Super::ClearPause();
    if (PauseCleared)
    {
        SetMatchState(ETOMGameState::InProgress);
    }

    return PauseCleared;
}
//-------------------------------------------------------------------------------------------------------------
