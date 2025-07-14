#include "Stamina/TOMStaminaComponent.h"

//-------------------------------------------------------------------------------------------------------------
void UTOMStaminaComponent::ReduceStamina(float tiredness)
{
    GetWorld()->GetTimerManager().ClearTimer(RestoreStaminaTimerHandle);
    Stamina -= FMath::Clamp(tiredness, 0.0f, Stamina);

    if (Stamina <= 0.0f)
    {
        IsTired = true;
    }

    GetWorld()->GetTimerManager().SetTimer(
        RestoreStaminaTimerHandle, this, &UTOMStaminaComponent::RestoreStamina, RestoreFrequancy, true, RestoreTimeRate);
}
//-------------------------------------------------------------------------------------------------------------
void UTOMStaminaComponent::RestoreStamina()
{
    Stamina += FMath::Clamp(RestorePoint, 0.0f, MaxStamina);

    if (Stamina > 0.0f) IsTired = false;

    if (Stamina >= MaxStamina)
    {
        Stamina = MaxStamina;
        GetWorld()->GetTimerManager().ClearTimer(RestoreStaminaTimerHandle);
    }
}
//-------------------------------------------------------------------------------------------------------------
