#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TOMUtility.h"
#include "BTTask_SetMovementSpeed.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_SetMovementSpeed();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EMovementSpeed MovementType = EMovementSpeed::Walking;
	
};
//-------------------------------------------------------------------------------------------------------------
