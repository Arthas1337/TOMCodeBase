#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Logging/LogMacros.h"
#include "DetectEnemyComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDetectEnemyComponent, Log, All);

class ATealesOfMeadowCharacter;

//-------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEALESOFMEADOW_API UDetectEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	 TArray<TObjectPtr<AActor>> TraceForTargets();
    TObjectPtr<AActor> CheckForClosestTarget(TArray<TObjectPtr<AActor>> Target);

	 UFUNCTION(BlueprintCallable)
    AActor* FindClosestEnemy();

protected:

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<ATealesOfMeadowCharacter> LockOnClass;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 float LockOnRadius = 200.0f;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxAngleDegrees = 45.0f;

private:

   float CheckHowCloseTargetIsToCenter(TObjectPtr<AActor> Target, float MaxAngleDegrees);
	ACharacter* GetPawnOwner();
};
//-------------------------------------------------------------------------------------------------------------
