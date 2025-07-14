#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TOMUtility.h"
#include "TOMPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API ATOMPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


private:

	void OnPauseGame(const FInputActionValue& Value);

	UFUNCTION()
   void OnMatchStateChanged(ETOMGameState GameState);

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

	 UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> PauseAction;
};
//-------------------------------------------------------------------------------------------------------------
