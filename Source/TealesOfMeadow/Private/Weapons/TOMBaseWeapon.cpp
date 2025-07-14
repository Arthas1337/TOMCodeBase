#include "Weapons/TOMBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "TOMStaminaComponent.h"
#include "Components/DetectEnemy/DetectEnemyComponent.h"
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "Health/TOMHealthComponent.h"

//-------------------------------------------------------------------------------------------------------------
ATOMBaseWeapon::ATOMBaseWeapon()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
}
//-------------------------------------------------------------------------------------------------------------
void ATOMBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------
void ATOMBaseWeapon::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted) 
{
    if (bInterrupted)
    {
        OnMontageEndedDelegate.Broadcast();
        UE_LOG(LogTemp, Warning, TEXT("Montage was interrupted!"));
    }
    else
    {
        OnMontageEndedDelegate.Broadcast();
        UE_LOG(LogTemp, Warning, TEXT("Montage finished normally"));
    }
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<ACharacter> ATOMBaseWeapon::GetPawnOwner()
{
    if (!GetOwner())
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant GetOwner Sword"));
       return nullptr;
    }

    const auto PawnOwner = Cast<ACharacter>(GetOwner());
    if (!PawnOwner)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Bad Cast To Character Sword"));
        return nullptr;
    }

    return PawnOwner;

}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<UTOMStaminaComponent> ATOMBaseWeapon::GetStaminaComponent()
{
    if (!GetPawnOwner()) return nullptr;
    const auto StaminaComponent = GetPawnOwner()->FindComponentByClass<UTOMStaminaComponent>();
    if(!StaminaComponent)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant Find StaminaComponent!"));
        return nullptr;
    }
    return StaminaComponent;
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<UWeaponComponent> ATOMBaseWeapon::GetWeaponComponent()
{
    if(!GetPawnOwner()) return nullptr;
    const auto WeaponComponent = GetPawnOwner()->FindComponentByClass<UWeaponComponent>();
    if(!WeaponComponent)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant Find WeaponComponent!"));
        return nullptr;
    }

    return WeaponComponent;
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<UDetectEnemyComponent> ATOMBaseWeapon::GetDetectEnemyComponent()
{
    if (!GetPawnOwner()) return nullptr;
    const auto DetectEnemyComponent = GetPawnOwner()->FindComponentByClass<UDetectEnemyComponent>();
    if (!DetectEnemyComponent)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant Find DetectEnemyComponent!"));
        return nullptr;
    }

    return DetectEnemyComponent;
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<UTOMHealthComponent> ATOMBaseWeapon::GetHealthComponent()
{
    if (!GetPawnOwner()) return nullptr;
    const auto HealthComponent = GetPawnOwner()->FindComponentByClass<UTOMHealthComponent>();
    if (!HealthComponent)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant Find HealthComponent!"));
        return nullptr;
    }

    return HealthComponent;
}
//-------------------------------------------------------------------------------------------------------------
void ATOMBaseWeapon::PlayMontage(const TObjectPtr<UAnimMontage>& Montage)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Montage || !Character->GetMesh()) return;

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        if (!AnimInstance->OnMontageEnded.IsAlreadyBound(this, &ATOMBaseWeapon::HandleMontageEnded))
        {
            AnimInstance->OnMontageEnded.AddDynamic(this, &ATOMBaseWeapon::HandleMontageEnded);
        }

        float Duration = AnimInstance->Montage_Play(Montage);
        if (Duration <= 0.f)
        {
            UE_LOG(LogWeaponComponent, Warning, TEXT("Failed to play montage"));
        }
    }
    else
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Cant get anim instance"));
    }
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<APlayerController> ATOMBaseWeapon::GetPlayerController()
{
    return Cast<APlayerController>(GetPawnOwner()->GetController());
}
//-------------------------------------------------------------------------------------------------------------
void ATOMBaseWeapon::PlayCameraShake(const TObjectPtr<APlayerController>& Controller,
    const EOverlayStates& CameraShakeState, const float& scale)
{
    if (!Controller || scale <= 0.0f)
    {
        UE_LOG(LogWeaponComponent, Display, TEXT("Camera Shake, uncorrect data"));
        return;
    }

    if(CameraShakeClassMap.Find(CameraShakeState))
    {
       Controller->ClientStartCameraShake(*CameraShakeClassMap.Find(CameraShakeState), scale);
    }
    else
    {
        UE_LOG(
            LogWeaponComponent, Warning, TEXT("Dont Have Key with value: %s"), *UEnum::GetDisplayValueAsText(CameraShakeState).ToString());
        return;
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMBaseWeapon::OnNotifyStateEnd()
{
    HitActors.Empty();
}
//-------------------------------------------------------------------------------------------------------------
