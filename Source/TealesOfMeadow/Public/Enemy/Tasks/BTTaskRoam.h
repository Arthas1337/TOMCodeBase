#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskRoam.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTTaskRoam : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UBTTaskRoam();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector RoamLocation;

};
//-------------------------------------------------------------------------------------------------------------
