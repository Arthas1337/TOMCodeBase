#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TOMGameOverWidget.generated.h"

class UButton;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UTOMGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> QuitButton;

	virtual void NativeOnInitialized() override;

private:

	UFUNCTION()
	void OnResettLevel();

	UFUNCTION()
    void OnQuitGame();
	
};
//-------------------------------------------------------------------------------------------------------------
