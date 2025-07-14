#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TOMWeaponFXComponent.generated.h"	

class UNiagaraSystem;
class UPhysicalMaterial;

//-------------------------------------------------------------------------------------------------------------
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEALESOFMEADOW_API UTOMWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTOMWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> DefaultEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<TObjectPtr<UPhysicalMaterial>, TObjectPtr<UNiagaraSystem>> EffectsMap;


};
//-------------------------------------------------------------------------------------------------------------
