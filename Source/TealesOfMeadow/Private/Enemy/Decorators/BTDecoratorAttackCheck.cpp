#include "Enemy/Decorators/BTDecoratorAttackCheck.h"
#include "TOM_AIPerceptionComponent.h"
#include "WeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interfaces/BaseTOMCharacterInterface.h"
#include "AIController.h"
#include "GameFramework/Character.h"

//-------------------------------------------------------------------------------------------------------------
UBTDecoratorAttackCheck::UBTDecoratorAttackCheck() 
{
   NodeName = "AttackCheck";
}
//-------------------------------------------------------------------------------------------------------------
bool UBTDecoratorAttackCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard) return false;

    const auto Enemy = Cast<ACharacter>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if(!Enemy) return false;

    const auto EnemyDodgeInterface = Cast<IBaseTOMCharacterInterface>(Enemy);
    if (!EnemyDodgeInterface)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyDodgeInterface is null!"));
        return false;
    }

    return EnemyDodgeInterface->GetIsAttackingPure();
}
//-------------------------------------------------------------------------------------------------------------
