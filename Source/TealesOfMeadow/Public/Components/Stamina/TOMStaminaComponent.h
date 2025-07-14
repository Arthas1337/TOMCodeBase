#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TOMStaminaComponent.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEALESOFMEADOW_API UTOMStaminaComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetStamina() const { return Stamina; }

    UFUNCTION(BlueprintCallable)
    float GetStaminaPercent() const { return Stamina / 100.0f; }

    UFUNCTION(BlueprintCallable)
    void ReduceStamina(float tiredness);

    bool IsTired = false;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MaxStamina = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "15.0"))
    float RestoreTimeRate = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "30.0"))
    float RestorePoint = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.5", ClampMax = "5.0"))
    float RestoreFrequancy = 1.0f;

    void RestoreStamina();

private:
    FTimerHandle RestoreStaminaTimerHandle;

    float Stamina = MaxStamina;
};
//-------------------------------------------------------------------------------------------------------------
