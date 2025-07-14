#include "BTServiceAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/Components/WeaponComponent.h"
#include "BaseTOMCharacterInterface.h"

UBTServiceAttack::UBTServiceAttack()
{
    NodeName = "AttackCheck";
    bNotifyTick = true;
}

void UBTServiceAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Blackboard) return;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return;

    const auto Enemy = Cast<ACharacter>(Blackboard->GetValueAsObject(EnemyActorKeySelector.SelectedKeyName));
    if (!Enemy)
    {
        Blackboard->SetValueAsBool("CanAttack", false);
        return;
    }

    const auto WeaponComponent = Pawn->FindComponentByClass<UWeaponComponent>();
    const auto TokenInterface = Cast<IBaseTOMCharacterInterface>(Enemy);

    bool bCanAttack = false;

    if (WeaponComponent && TokenInterface && WeaponComponent->IsDrawn() &&
        FVector::Dist(Pawn->GetActorLocation(), Enemy->GetActorLocation()) <= AttackDistance)
    {
        bCanAttack = TokenInterface->ReserveAttackToken(AttackTokensRequired);
        TokenInterface->ReturnAttackToken(AttackTokensRequired);
    }

    Blackboard->SetValueAsBool("CanAttack", bCanAttack);
}