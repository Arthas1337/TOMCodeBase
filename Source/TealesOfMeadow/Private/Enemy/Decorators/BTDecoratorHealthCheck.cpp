#include "Enemy/Decorators/BTDecoratorHealthCheck.h"
#include "Health/TOMHealthComponent.h"
#include "AIController.h"

//-------------------------------------------------------------------------------------------------------------
UBTDecoratorHealthCheck::UBTDecoratorHealthCheck() 
{
   NodeName = "HealthCheck";
}
//-------------------------------------------------------------------------------------------------------------
bool UBTDecoratorHealthCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
   const auto Controller = OwnerComp.GetAIOwner();
   if(!Controller) return false;

   const auto HealthComponent = Controller->GetPawn()->FindComponentByClass<UTOMHealthComponent>();
   if(!HealthComponent) return false;

    return !HealthComponent->IsDead();
}
//-------------------------------------------------------------------------------------------------------------
