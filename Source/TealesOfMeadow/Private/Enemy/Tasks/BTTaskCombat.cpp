#include "BTTaskCombat.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/Components/WeaponComponent.h"
#include "BaseTOMCharacterInterface.h"

UBTTaskCombat::UBTTaskCombat()
{
    NodeName = "ExecuteAttack";
}

EBTNodeResult::Type UBTTaskCombat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto Enemy = Cast<ACharacter>(Blackboard->GetValueAsObject(EnemyActorKeySelector.SelectedKeyName));
    if (!Enemy) return EBTNodeResult::Failed;

    const auto WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    const auto TokenInterface = Cast<IBaseTOMCharacterInterface>(Enemy);

    if (!WeaponComponent || !TokenInterface) return EBTNodeResult::Failed;

    WeaponComponent->Attack(Enemy);

    return EBTNodeResult::Succeeded;
}