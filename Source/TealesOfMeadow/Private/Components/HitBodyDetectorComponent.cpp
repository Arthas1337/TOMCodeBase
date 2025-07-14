#include "Components/HitBodyDetectorComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Animation/AnimMontage.h"

//-------------------------------------------------------------------------------------------------------------
UHitBodyDetectorComponent::UHitBodyDetectorComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
}
//-------------------------------------------------------------------------------------------------------------
void UHitBodyDetectorComponent::PlayImpactMontage(const FHitResult& Hit) 
{
    UAnimMontage* HitMontage;
    if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (HitMontages.Contains(PhysMat))
        {
            HitMontage = HitMontages[PhysMat];
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
