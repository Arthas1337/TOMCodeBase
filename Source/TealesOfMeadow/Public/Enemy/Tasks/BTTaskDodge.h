#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskDodge.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTTaskDodge : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskDodge();

protected:

   virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
   float DodgeChance = 0.3;
};
//-------------------------------------------------------------------------------------------------------------
