#include "Animation/Notifys/OnDrawTraceAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapons/TOMOneHandSword.h"
#include "Weapons/Components/WeaponComponent.h"

void UOnDrawTraceAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (const auto Pawn = MeshComp->GetOwner())
    {
        if (const auto Character = Cast<ACharacter>(Pawn))
        {
            const auto WeaponComponent = Character->GetComponentByClass<UWeaponComponent>();
            if (WeaponComponent)
            {
               if(WeaponComponent->CurrentRightHandWeapon)
               {
                   WeaponComponent->CurrentRightHandWeapon->TraceSwordHit();
               }
            }
        }
    }
}

void UOnDrawTraceAnimNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (const auto Pawn = MeshComp->GetOwner())
    {
        if (const auto Character = Cast<ACharacter>(Pawn))
        {
            const auto WeaponComponent = Character->GetComponentByClass<UWeaponComponent>();
            if (WeaponComponent)
            {
                if (WeaponComponent->CurrentRightHandWeapon)
                {
                    WeaponComponent->CurrentRightHandWeapon->OnNotifyStateEnd();
                }
            }
        }
    }
}
