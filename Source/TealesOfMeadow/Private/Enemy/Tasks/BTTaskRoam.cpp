#include "Enemy/Tasks/BTTaskRoam.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

//-------------------------------------------------------------------------------------------------------------
UBTTaskRoam::UBTTaskRoam() 
{
	NodeName = "Roam";
}
//-------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTaskRoam::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
   if (!Blackboard) return EBTNodeResult::Failed;

	const auto Controller = OwnerComp.GetAIOwner();
   if (!Controller) return EBTNodeResult::Failed;

   const auto Pawn = Controller->GetPawn() ;
   if (!Pawn) return EBTNodeResult::Failed;
   
   const auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn);
   if (!NavSystem) return EBTNodeResult::Failed;

   FNavLocation NavResult;
   const auto Found = NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, NavResult);
   if (!Found) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(RoamLocation.SelectedKeyName, NavResult.Location);

   return EBTNodeResult::Succeeded;
}
//-------------------------------------------------------------------------------------------------------------
