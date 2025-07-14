#include "Enemy/Services/BTServiceDodge.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/Components/WeaponComponent.h"
#include "Interfaces/BaseTOMCharacterInterface.h"
#include "GameFramework/CharacterMovementComponent.h"

//-------------------------------------------------------------------------------------------------------------
UBTServiceDodge::UBTServiceDodge() 
{
	NodeName = "DodgeService";
}
//-------------------------------------------------------------------------------------------------------------
void UBTServiceDodge::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) 
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if(!Blackboard) return;

    const auto Controller = OwnerComp.GetAIOwner();
    const auto Enemy = Cast<ACharacter>(Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName));
    if(!Enemy) return;

    const auto EnemyWeaponComponent = Enemy->FindComponentByClass<UWeaponComponent>();
    
    if(Controller)
    {
       const auto AICharacter = Cast<ACharacter>(Controller->GetPawn());
       if (AICharacter)
       {
            if(EnemyWeaponComponent)
            {
                const auto DodgeChance = FMath::RandBool();
                if (EnemyWeaponComponent->IsAttacking && DodgeChance)
                {
                    const auto DodgeInterface = Cast<IBaseTOMCharacterInterface>(AICharacter);
                    if (DodgeInterface)
                    {
                        DodgeInterface->Dodge();
                    }
                }
            }
       }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
//-------------------------------------------------------------------------------------------------------------
