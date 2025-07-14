#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TOMUtility.h"
#include "TOMPlayerHUD.generated.h"

class UUserWidget;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API ATOMPlayerHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PlayerWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> PauseWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<UUserWidget> GameOverWidgetClass;  

private:

   UFUNCTION()
   void OnMatchStateChanged(ETOMGameState State);

   UPROPERTY()
   TMap<ETOMGameState, TObjectPtr<UUserWidget>> GameWidgets;

   UPROPERTY()
   TObjectPtr<UUserWidget> CurrentWidget = nullptr;
};
//-------------------------------------------------------------------------------------------------------------
