#include "Health/TOMHealthComponent.h"
#include "WeaponComponent.h"

//-------------------------------------------------------------------------------------------------------------
void UTOMHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    if (Health <= 0.0f) OnDeath.Broadcast();
    const auto ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeDamage);
    }
}
//-------------------------------------------------------------------------------------------------------------
void UTOMHealthComponent::OnTakeDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld() || !InstigatedBy || !InstigatedBy->GetPawn())
    {
        return;
    }
    GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);

    if (DamagedActor->ActorHasTag(FriendlyTag) && InstigatedBy->GetPawn()->ActorHasTag(FriendlyTag) && bFriendlyDamageAI)
    {
        UE_LOG(LogTemp, Warning, TEXT("Friendly fire blocked: %s tried to damage %s"), *InstigatedBy->GetPawn()->GetName(),
            *DamagedActor->GetName());
        return;
    }

    if(const auto DamagedActorWeaponComponent = DamagedActor->FindComponentByClass<UWeaponComponent>())
    {
        if(DamagedActorWeaponComponent->IsShieldBlocking)
        {
            ReduceHealthPercent(Damage, ShieldDamageReduce);
        }
        else if(DamagedActorWeaponComponent->IsSwordBlocking)
        {
            ReduceHealthPercent(Damage, SwordDamageReduce);
        }
        else
        {
            ReduceHealthPercent(Damage);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(AutoHealTimerHandle, this,&UTOMHealthComponent::RestoreHealth, HealFrequncy, true, HealDelay);

    if (OnDamage.IsBound())
    {
        OnDamage.Broadcast();
    }
    if (IsDead())
    {
        OnDeath.Broadcast();
    }
}
//-------------------------------------------------------------------------------------------------------------
void UTOMHealthComponent::RestoreHealth() 
{
    if(HealthAmount > 0.0f && !IsDead() && AutoHeal)
    {
      Health += FMath::Clamp(HealthAmount, 0.0f, MaxHealth);
      OnHeal.Broadcast();
    }
    else
    {
       UE_LOG(LogTemp, Warning, TEXT("Uncorrect data Restore Health"));
       GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
       return;
    }

    if(Health >= MaxHealth)
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
    }
}
//-------------------------------------------------------------------------------------------------------------
void UTOMHealthComponent::ReduceHealthPercent(const float& Damage, const float& ReducePercent)
{
    Health -= FMath::Clamp(Damage * ReducePercent, 0.0f, Health);
}
//-------------------------------------------------------------------------------------------------------------
