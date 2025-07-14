#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Logging/LogMacros.h"
#include "BTServiceAttack.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAttackService, Log, All);

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTServiceAttack : public UBTService
{
    GENERATED_BODY()

public:
    UBTServiceAttack();

    UPROPERTY(EditAnywhere, Category = "AI")
    float AttackDistance = 300.f;

    UPROPERTY(EditAnywhere, Category = "AI")
    int32 AttackTokensRequired = 1;

        UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector EnemyActorKeySelector;

protected:

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
//-------------------------------------------------------------------------------------------------------------
