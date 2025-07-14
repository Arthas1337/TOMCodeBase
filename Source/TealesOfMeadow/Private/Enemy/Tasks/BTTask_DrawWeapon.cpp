#include "Enemy/Tasks/BTTask_DrawWeapon.h"
#include "AIController.h"
#include "Weapons/Components/WeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

//-------------------------------------------------------------------------------------------------------------
UBTTask_DrawWeapon::UBTTask_DrawWeapon() 
{
   NodeName = "DrawWeapon";
}
//-------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTask_DrawWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return EBTNodeResult::Failed;

    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return EBTNodeResult::Failed;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const auto WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    if (!WeaponComponent) return EBTNodeResult::Failed;

    if (!WeaponComponent->IsDrawn() && HasAim)
    {
        WeaponComponent->DrawWeapon(EWeaponType::Sword, false);
    }
    else if (WeaponComponent->IsDrawn() && !HasAim)
    {
        WeaponComponent->DrawWeapon(EWeaponType::Sword, true);
    }

    return EBTNodeResult::Succeeded;
}
//-------------------------------------------------------------------------------------------------------------
