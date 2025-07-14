#include "Weapons/Components/TOMWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

//-------------------------------------------------------------------------------------------------------------
UTOMWeaponFXComponent::UTOMWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	 
}
//------------------------------------------------------------------------------------------------------------
void UTOMWeaponFXComponent::PlayImpactFX(const FHitResult& Hit) 
{
	auto Effect = DefaultEffect;

	if(Hit.PhysMaterial.IsValid())
   {
		if(Hit.GetActor()->CanBeDamaged())
		{
			const auto PhysMat = Hit.PhysMaterial.Get();
			if(EffectsMap.Contains(PhysMat))
			{
			    Effect = EffectsMap[PhysMat];
			}
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
}
//-------------------------------------------------------------------------------------------------------------
