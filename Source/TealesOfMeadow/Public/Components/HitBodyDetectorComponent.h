#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitBodyDetectorComponent.generated.h"

class UAnimMontage;
class UPhysicalMaterial;

//-------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEALESOFMEADOW_API UHitBodyDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHitBodyDetectorComponent();

	void PlayImpactMontage(const FHitResult& Hit);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
    TMap<UPhysicalMaterial*, UAnimMontage*> HitMontages;

};
//-------------------------------------------------------------------------------------------------------------
