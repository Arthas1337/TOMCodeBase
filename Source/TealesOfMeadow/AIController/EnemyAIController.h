#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DetourCrowdAIController.h"
#include "EnemyAIController.generated.h"

class UTOM_AIPerceptionComponent;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API AEnemyAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()

public:

	AEnemyAIController();

protected:

	virtual void OnPossess(APawn* InPawn) override;
   virtual void OnUnPossess() override;
   virtual void Tick(float DeltaTime) override;

   UFUNCTION()
   void CanSenseActor(const TArray<AActor*>& UpdatedActors);

   UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
   TObjectPtr<UTOM_AIPerceptionComponent> EnemyPerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FName FocusOnKeyName = "EnemyActor";

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FName ForgotActorKeyName = "IsForgotActor";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FName AttackRadiusKeyName = "AttackRadius";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
   FName DefendRadiusKeyName = "DefendRadius";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TObjectPtr<AActor> FocusActor;

private:

	AActor* GetFocusOnActor() const;

   UFUNCTION(BlueprintCallable)
   void CheckIfForgottenSeenActor();
   void HandleForgotActor(AActor* Actor);
   FTimerHandle ForrgottenSeenActorTimerHandle;

};
//-------------------------------------------------------------------------------------------------------------
