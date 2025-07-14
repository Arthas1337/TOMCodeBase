#pragma once

#include "CoreMinimal.h"
#include "../TealesOfMeadowCharacter.h"
#include "EnemyAIInterface.h"
#include "TOMEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDead);

class UCapsuleCollisionComponent;
class UBehaviorTree;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API ATOMEnemy : public ATealesOfMeadowCharacter, public IEnemyAIInterface
{
    GENERATED_BODY()

public:

    ATOMEnemy();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

    UPROPERTY()
    FOnEnemyDead OnEnemyDead;

protected:

    virtual void OnDeath() override;
    virtual void ActorToEnemyTarget(float DeltaTime) override;
    virtual float GetAttackRadius() const override;
    virtual float GetDefendRadius() const override;
    virtual void SetMovementSpeed(EMovementSpeed MovementType) override;

    virtual bool ReserveAttackToken(int Amount) override;
    virtual void ReturnAttackToken(int Amount) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UCapsuleComponent* LockOnCollision;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float AttackRadius = 150.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DefendRadius = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DestroyActorTimeAI = 3.0f;

private:

   void OnDestroyWeapons();

   FTimerHandle DestroyWeaponsTimerHandle;
   int TokenAmountOnDeath = 0;

};
//-------------------------------------------------------------------------------------------------------------
