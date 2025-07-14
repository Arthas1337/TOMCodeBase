#include "Enemy/Components/TOM_AIPerceptionComponent.h"
#include "AIController.h"
#include "Health/TOMHealthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"
#include "Kismet/GameplayStatics.h"

//-------------------------------------------------------------------------------------------------------------
TObjectPtr<AActor> UTOM_AIPerceptionComponent::GetClosestActor() const
{
   TArray<AActor*> PerceiveActors;
   GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
   if(PerceiveActors.Num() == 0)
   {
      return nullptr;
   }
   const auto Controller = Cast<AAIController>(GetOwner());
   if (!Controller) return nullptr;

   const auto Pawn = Controller->GetPawn();

   float BestDistance = MAX_FLT;
   TObjectPtr<AActor> NearestActor = nullptr;

   for(auto& PerceiveActor : PerceiveActors)
   {
      KnownSeenActors.AddUnique(PerceiveActor);

      const auto HealthComponent = PerceiveActor->FindComponentByClass<UTOMHealthComponent>();
      if(HealthComponent && !HealthComponent->IsDead())
      {
          const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();

          for(auto& Tag : PerceiveActor->Tags)
          {
             if(Tag == FriendlyTagName)
             {
                continue;
             }
             else
             {
                 if (CurrentDistance < BestDistance)
                 {
                     BestDistance = CurrentDistance;
                     NearestActor = PerceiveActor;
                 }
             }
          }
      }
   }

    return NearestActor;
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<AActor> UTOM_AIPerceptionComponent::GetDamageInstigatorActor() const
{
    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Damage::StaticClass(), PerceiveActors);

    if(PerceiveActors.Num() > 0)
    {
       return PerceiveActors[0];
    }

    return nullptr;
}
//-------------------------------------------------------------------------------------------------------------