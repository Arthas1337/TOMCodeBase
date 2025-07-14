#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskCombat.generated.h"


UCLASS()
class TEALESOFMEADOW_API UBTTaskCombat : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTaskCombat();

    UPROPERTY(EditAnywhere, Category = "AI")
    int32 AttackTokensRequired = 1;

    UPROPERTY(EditAnywhere, Category = "AI")
    FBlackboardKeySelector EnemyActorKeySelector;

protected:

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
