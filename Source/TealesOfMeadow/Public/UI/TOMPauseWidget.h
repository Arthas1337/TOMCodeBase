#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TOMPauseWidget.generated.h"

class UButton;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UTOMPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClearPauseButton;

	UPROPERTY(meta = (BindWidget))
   TObjectPtr<UButton> ResetGameButton;

   UPROPERTY(meta = (BindWidget))
   TObjectPtr<UButton> QuitGameButton;

private:

	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
   void OnResetGame();

	UFUNCTION()
   void OnQuitGame();

};
//-------------------------------------------------------------------------------------------------------------
