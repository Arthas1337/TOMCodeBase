#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TOMHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeal);

//-------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEALESOFMEADOW_API UTOMHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / 100.0f; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return Health <= 0.0f; }

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnDeath OnDeath;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnDamage OnDamage;

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnHeal OnHeal;

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "100.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool AutoHeal = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealthAmount = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealFrequncy = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealDelay = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ShieldDamageReduce = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SwordDamageReduce = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bFriendlyDamageAI = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName FriendlyTag = "GangMemeber";

private:

    float Health = MaxHealth;
    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void OnTakeDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void RestoreHealth();
    void ReduceHealthPercent(const float& Damage, const float& ReducePercent = 1.0f);
};
//-------------------------------------------------------------------------------------------------------------
