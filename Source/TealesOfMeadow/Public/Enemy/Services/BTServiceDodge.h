#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceDodge.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTServiceDodge : public UBTService
{
	GENERATED_BODY()

public:

	UBTServiceDodge();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FBlackboardKeySelector EnemyActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   float Distance = 10.0f;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
//-------------------------------------------------------------------------------------------------------------
