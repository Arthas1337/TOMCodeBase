#include "Enemy/Tasks/BTTaskDodge.h"
#include "GameFramework/Character.h"
#include "Interfaces/BaseTOMCharacterInterface.h"
#include "AIController.h"

//-------------------------------------------------------------------------------------------------------------
UBTTaskDodge::UBTTaskDodge() 
{
   NodeName = "Dodge";
}
//-------------------------------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTaskDodge::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Controller is null!"));
        return EBTNodeResult::Failed;
    }

    const auto AICharacter = Cast<ACharacter>(Controller->GetPawn());
    if (!AICharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("AICharacter is null!"));
        return EBTNodeResult::Failed;
    }

    const auto DodgeInterface = Cast<IBaseTOMCharacterInterface>(AICharacter);
    if (!DodgeInterface)
    {
        UE_LOG(LogTemp, Error, TEXT("DodgeInterface is null!"));
        return EBTNodeResult::Failed;
    }

    if (DodgeInterface->GetIsDodging())
    {
        UE_LOG(LogTemp, Warning, TEXT("Dodge is already in progress!"));
        return EBTNodeResult::Failed;
    }

    const float DodgeRandomChance = FMath::FRand();
    if (DodgeRandomChance <= DodgeChance)
    {
        DodgeInterface->Dodge();
        UE_LOG(LogTemp, Warning, TEXT("Dodge successful!"));
        return EBTNodeResult::Succeeded;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Dodge failed (chance: %f)"), DodgeRandomChance);
        return EBTNodeResult::Failed;
    }
}
//-------------------------------------------------------------------------------------------------------------
