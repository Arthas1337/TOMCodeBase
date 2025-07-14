#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TOMUtility.h"
#include "BTServiceFindEnemy.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UBTServiceFindEnemy : public UBTService
{
	GENERATED_BODY()

public:

	UBTServiceFindEnemy();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FBlackboardKeySelector AIStateActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
//-------------------------------------------------------------------------------------------------------------
