#include "Animation/Notifys/OnDrawSwordAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapons/Components/WeaponComponent.h"

void UOnDrawSwordAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (const auto Owner = MeshComp->GetOwner())
    {
        const auto Character = Cast<ACharacter>(Owner);
        if (Character)
        {
            const auto WeaponComponent = Character->FindComponentByClass<UWeaponComponent>();
            if (WeaponComponent)
            {
                WeaponComponent->OnDrawSwordNotify();
            }
        }
    }
}
