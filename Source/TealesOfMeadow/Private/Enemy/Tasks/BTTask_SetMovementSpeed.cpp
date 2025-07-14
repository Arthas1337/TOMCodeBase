#include "Enemy/Tasks/BTTask_SetMovementSpeed.h"
#include "AIController.h"
#include "EnemyAIInterface.h"

//-------------------------------------------------------------------------------------------------------------
UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed() 
{
   NodeName = "SetMovementSpeed";
}
//-------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto AIInterface = Cast<IEnemyAIInterface>(Pawn);
    if (!AIInterface) return EBTNodeResult::Failed;

    AIInterface->SetMovementSpeed(MovementType);

    return EBTNodeResult::Succeeded;
}
//-------------------------------------------------------------------------------------------------------------
