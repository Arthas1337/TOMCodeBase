#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TOMUtility.h"
#include "TealesOfMeadowGameMode.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class ATealesOfMeadowGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATealesOfMeadowGameMode();

    virtual void StartPlay() override;
    void SetMatchState(ETOMGameState State);

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnGameChangeStateSignature OnGameChangeState;

protected:

   virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;
   virtual bool ClearPause() override;

   ETOMGameState GameState = ETOMGameState::WaitingToStart;

};
//-------------------------------------------------------------------------------------------------------------
