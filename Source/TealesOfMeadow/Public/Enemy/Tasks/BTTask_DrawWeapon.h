#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DrawWeapon.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTTask_DrawWeapon : public UBTTaskNode
{
	GENERATED_BODY()
	

public:

	UBTTask_DrawWeapon();

protected:

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FBlackboardKeySelector EnemyActorKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
//-------------------------------------------------------------------------------------------------------------
