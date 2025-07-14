#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecoratorHealthCheck.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTDecoratorHealthCheck : public UBTDecorator
{
	GENERATED_BODY()
	
public:

	UBTDecoratorHealthCheck();

protected:

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
//-------------------------------------------------------------------------------------------------------------
