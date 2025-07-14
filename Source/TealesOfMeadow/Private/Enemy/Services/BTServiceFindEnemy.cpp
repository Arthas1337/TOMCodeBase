#include "Enemy/Services/BTServiceFindEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/TOM_AIPerceptionComponent.h"


//-------------------------------------------------------------------------------------------------------------
UBTServiceFindEnemy::UBTServiceFindEnemy() 
{
	NodeName = "FindEnemy";
}
//-------------------------------------------------------------------------------------------------------------
void UBTServiceFindEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();
	if(Blackboard)
   {
		const auto Controller = OwnerComp.GetAIOwner();
      const auto PerceprionComponent = Controller->FindComponentByClass<UTOM_AIPerceptionComponent>();
		if(PerceprionComponent)
      {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, 
					PerceprionComponent->GetClosestActor());

				if (PerceprionComponent->GetClosestActor())
				{
                    Blackboard->SetValueAsEnum(AIStateActorKey.SelectedKeyName, static_cast<uint8>(EAIStates::Attacking));
				}
		}
   }
   Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
//-------------------------------------------------------------------------------------------------------------
