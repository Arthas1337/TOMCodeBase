#pragma once

#include "CoreMinimal.h"
#include "Weapons/TOMBaseWeapon.h"
#include "TOMOneHandSword.generated.h"

class UTOMWeaponFXComponent;
class UCapsuleComponent;
class UTOMStaminaComponent;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API ATOMOneHandSword : public ATOMBaseWeapon
{
    GENERATED_BODY()

public:

    ATOMOneHandSword();

    virtual void ThrustAttack() override;

protected:

    virtual void BeginPlay();
    UFUNCTION()
    virtual void OnAttack(AActor* TargetActor) override;
    void FlyAttack();
    virtual void ResetCombo() override;
    virtual void TraceSwordHit() override;


   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   TObjectPtr<UTOMWeaponFXComponent> WeaponFXComponent;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
   TArray<TObjectPtr<UAnimMontage>> OneHandedLightAttacks;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
   TArray<TObjectPtr<UAnimMontage>> WithShieldAttacks;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
   TObjectPtr<UAnimMontage> FlyAttackMontage;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
   TObjectPtr<UAnimMontage> ThrustMontage;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
   float AttackTiredness = 15.0f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FName FriendlyTag = "GangMemeber";

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   UMaterialInterface* DecalMatrial;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   FVector DecalSize = FVector(8.0f, 8.0f, 8.0f);

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageSystem")
   float TwoHandSwordDamage = 15.0f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageSystem")
   float OneHandSwordDamage = 15.0f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   float DecalLifeSpan = 30.0f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   bool bFriendlyDamageAI = true;

   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int TokensAmount = 1;

private:

   FTimerHandle ComboTimerHandle;
   int ComboCounter = 0;

   UFUNCTION()
   void ReturnTokens();

private:

   void PlayComboAttackAnimation(const TObjectPtr<UAnimMontage>& AttackAnimation, AActor* TargetActor);
   void ChooseAttacksAnimsMontages(TArray<TObjectPtr<UAnimMontage>>& CurrentAttacksAnimsMontages);
   void AttachReturnTokenOnDeathOwner();
   void ApplyPointDamageOverlayState(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection, FHitResult const& HitInfo,
       AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass, EOverlayStates OverlayState);

   AActor* EnenmyTargetActor;
};
//-------------------------------------------------------------------------------------------------------------
