#include "EnemyAIController.h"
#include "Enemy/TOMEnemy.h"
#include "Enemy/Components/TOM_AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIInterface.h"
#include "Perception/AISense_Sight.h"
#include "Navigation/CrowdFollowingComponent.h"

//-------------------------------------------------------------------------------------------------------------
AEnemyAIController::AEnemyAIController() 
{
    EnemyPerceptionComponent = CreateDefaultSubobject<UTOM_AIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*EnemyPerceptionComponent.Get());
}
//-------------------------------------------------------------------------------------------------------------
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

	const auto AICharacter = Cast<ATOMEnemy>(InPawn);
    if (AICharacter)
   {
        RunBehaviorTree(AICharacter->BehaviorTreeAsset.Get());
       const auto AIInterface = Cast<IEnemyAIInterface>(AICharacter);
       if(AIInterface)
       {
           const auto BlackBoardComponent = GetBlackboardComponent();
           if (BlackBoardComponent)
           {
               BlackBoardComponent->SetValueAsFloat(AttackRadiusKeyName, AIInterface->GetAttackRadius());
               BlackBoardComponent->SetValueAsFloat(DefendRadiusKeyName, AIInterface->GetDefendRadius());
           }
       }
   }

    EnemyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::CanSenseActor);

    UCrowdFollowingComponent* CrowdFollowingComponent = FindComponentByClass<UCrowdFollowingComponent>();
    if(CrowdFollowingComponent)
    {
        UE_LOG(LogTemp, Display, TEXT("Find CrowdFollowingComponent"))
        CrowdFollowingComponent->SetCrowdSeparation(true);
        CrowdFollowingComponent->SetCrowdSeparationWeight(50.0f);
        CrowdFollowingComponent->SetCrowdAvoidanceRangeMultiplier(1.1f);
    }

  // GetWorldTimerManager().SetTimer(ForrgottenSeenActorTimerHandle, this, &AEnemyAIController::CheckIfForgottenSeenActor, 
  //    0.5f, true);
}
//-------------------------------------------------------------------------------------------------------------
void AEnemyAIController::OnUnPossess() 
{
    Super::OnUnPossess();

    GetWorldTimerManager().ClearTimer(ForrgottenSeenActorTimerHandle);
}
//-------------------------------------------------------------------------------------------------------------
void AEnemyAIController::Tick(float DeltaTime) 
{
   Super::Tick(DeltaTime);
   FocusActor = GetFocusOnActor();
   SetFocus(FocusActor);
}
//-------------------------------------------------------------------------------------------------------------
AActor* AEnemyAIController::GetFocusOnActor() const
{
    if(!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
void AEnemyAIController::CheckIfForgottenSeenActor() {}
//-------------------------------------------------------------------------------------------------------------
void AEnemyAIController::CanSenseActor(const TArray<AActor*>& UpdatedActors)
{
    if (!EnemyPerceptionComponent) return;

    const FAIStimulus* LastSightStimulus;

    for (auto& Actor : UpdatedActors)
    {
        FActorPerceptionBlueprintInfo PerceptionInfo;
        if (EnemyPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo))
        {
            LastSightStimulus = PerceptionInfo.LastSensedStimuli.FindByPredicate(
                [&](const FAIStimulus& Stimulus) 
               {
                     const auto SensedClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus);
                     return SensedClass == UAISense_Sight::StaticClass(); 
               });

            if (LastSightStimulus && !LastSightStimulus->WasSuccessfullySensed()) UE_LOG(LogTemp, Display, TEXT("Lost Sense"));
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void AEnemyAIController::HandleForgotActor(AActor* Actor)
{
    if (!Actor) return;
    EnemyPerceptionComponent->KnownSeenActors.Remove(Actor);

    if(!FocusActor)
      UE_LOG(LogTemp, Display, TEXT("FocusActor Invalid"));

    if(Actor == FocusActor)
    {
        
        UE_LOG(LogTemp, Display, TEXT("SetValue ForgotActorKeyName"))
    }
}
//-------------------------------------------------------------------------------------------------------------
