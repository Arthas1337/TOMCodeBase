#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TOMBaseWeapon.h"
#include "Stamina/TOMStaminaComponent.h"
#include "Components/DetectEnemy/DetectEnemyComponent.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Health/TOMHealthComponent.h"
#include "Enemy/TOMEnemy.h"
#include "Interfaces/BaseTOMCharacterInterface.h"

DEFINE_LOG_CATEGORY(LogWeaponComponent);

//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::PlayMontage(const TObjectPtr<UAnimMontage>& Montage)
{
    const auto Character = Cast<ACharacter>(GetOwner());
    if (!Montage || !Character->GetMesh()) return;

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(Montage);
    }
}
//-------------------------------------------------------------------------------------------------------------
ACharacter* UWeaponComponent::GetPawnOwner()
{
    if (!GetOwner())
    {
        return nullptr;
    }

    const auto PawnOwner = Cast<ACharacter>(GetOwner());
    if (!PawnOwner)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Bad Cast To Character WeaponComponet"));
        return nullptr;
    }

    return PawnOwner;
}
IBaseTOMCharacterInterface* UWeaponComponent::GetPawnOwnerInterface()
{
    return Cast<IBaseTOMCharacterInterface>(GetPawnOwner());
}
//-------------------------------------------------------------------------------------------------------------
UTOMStaminaComponent* UWeaponComponent::GetStaminaComponent()
{
    return GetPawnOwner()->FindComponentByClass<UTOMStaminaComponent>();
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<UDetectEnemyComponent> UWeaponComponent::GetDetectEnemyComponent()
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
TObjectPtr<UMotionWarpingComponent> UWeaponComponent::GetMotionWarpingComponent()
{
    if (!GetPawnOwner()) return nullptr;
    const auto MotionWarpingComponent = GetPawnOwner()->FindComponentByClass<UMotionWarpingComponent>();
    if (!MotionWarpingComponent)
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Cant Find MotionWarpingComponent!"));
        return nullptr;
    }

    return MotionWarpingComponent;
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::SpawnWeapons()
{
    if (!GetWorld()) return;

    for (auto& i : WeaponsClassesMap)
    {
        if (i.Value)
        {
            const auto Weapon = GetWorld()->SpawnActor<ATOMBaseWeapon>(i.Value);

            if (!Weapon) continue;

            Weapon->SetOwner(GetPawnOwner());

            SpawnWeaponsMap.Add(i.Key, Weapon);

            UE_LOG(LogWeaponComponent, Display, TEXT("Actor has been spawned: %s"), *Weapon->GetName());

            const auto InstanceAttachMesh = Cast<USkeletalMeshComponent>(GetPawnOwner()->GetDefaultSubobjectByName(InstanceSkeleton));
            if (!InstanceAttachMesh) return;

            const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);

            Weapon->AttachToComponent(InstanceAttachMesh, AttachmentRule, i.Key);

            UE_LOG(LogWeaponComponent, Display, TEXT("Actor has been Attach: %s"), *i.Key.ToString());
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::ChangeHandState(const bool& bIsDrawing, const FWeaponInfo& WeaponInfo, const bool& IsRightHandState)
{
   if(IsRightHandState)
   {
      PlayMontage(bIsDrawing ? WeaponInfo.SheathMontage : WeaponInfo.DrawMontage);
      RightHandState = bIsDrawing ? ERightHandStates::Unarmed : ERightHandStates::OneHandSword;
      IsDrawing = true;
      CurrentRightHandWeapon = SpawnWeaponsMap[WeaponInfo.SheathSocket];
   }
   else
   {
       PlayMontage(bIsDrawing ? WeaponInfo.SheathMontage : WeaponInfo.DrawMontage);
       LeftHandState = bIsDrawing ? ELeftHandStates::Unarmed : ELeftHandStates::Shield;
       IsDrawing = true;
       CurrentLeftHandWeapon = SpawnWeaponsMap[WeaponInfo.SheathSocket];
   }
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::DrawWeapon(const EWeaponType& WeaponType, const bool& bIsDrawing)
{
    if (IsAttacking || IsDrawing || IsHit || !WeaponInfoMap.Contains(WeaponType)) return;

    FWeaponInfo WeaponInfo = WeaponInfoMap[WeaponType];

    if (!SpawnWeaponsMap.Contains(WeaponInfo.SheathSocket)) return;
    SpawnWeaponsMap[WeaponInfo.SheathSocket].Get()->ResetCombo();

    TargetSocket = bIsDrawing ? WeaponInfo.SheathSocket : WeaponInfo.HandSocket;

    if(WeaponType == EWeaponType::Sword)
    {
       if(LeftHandState == ELeftHandStates::Shield)
       {
           PlayMontage(bIsDrawing ? WeaponInfo.SwordWithShieldSheathMontage : WeaponInfo.SwordWithShieldDrawMontage);
           RightHandState = bIsDrawing ? ERightHandStates::Unarmed : ERightHandStates::OneHandSword;
           CurrentRightHandWeapon = SpawnWeaponsMap[WeaponInfo.SheathSocket];
       }
       else
       {
         ChangeHandState(bIsDrawing, WeaponInfo, true);
       }
    }
    else if (WeaponType == EWeaponType::Shield)
    {
        ChangeHandState(bIsDrawing, WeaponInfo, false);
    }
}
//-------------------------------------------------------------------------------------------------------------
bool UWeaponComponent::IsDrawn()
{
   return RightHandState != ERightHandStates::Unarmed;
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::AttachWeaponsToCharacter(const TObjectPtr<ATOMBaseWeapon> Weapon, const FName& Socket)
{
    if (!Weapon)
    {
        UE_LOG(LogTemp, Error, TEXT("NULL Weapon, Bad Attach"));
        return;
    }

    const auto KnightMesh = Cast<USkeletalMeshComponent>(GetPawnOwner()->GetDefaultSubobjectByName(InstanceSkeleton));
    if (!KnightMesh) return;

    if (!KnightMesh->DoesSocketExist(Socket))
    {
        UE_LOG(LogTemp, Error, TEXT("No Socket With Name %s"), *Socket.ToString());
        return;
    }

    const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(KnightMesh, AttachmentRule, Socket);
}
//-------------------------------------------------------------------------------------------------------------
bool UWeaponComponent::CanAttack()
{
    return RightHandState != ERightHandStates::Unarmed && 
       IsSwordEquip && !IsAttacking && !IsDrawing && !IsHit &&
       !GetPawnOwner()->GetCharacterMovement()->IsCrouching() && 
       !GetStaminaComponent()->IsTired && CurrentRightHandWeapon &&
       !GetPawnOwner()->GetCharacterMovement()->IsFalling() && 
       !GetPawnOwnerInterface()->GetIsDodging();
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::NotifyAttachBool(bool& IsEquip, TObjectPtr<ATOMBaseWeapon>& CurrentWeapon)
{
    if (IsEquip == false)
    {
        AttachWeaponsToCharacter(CurrentWeapon, TargetSocket);
        IsEquip = true;
    }
    else
    {
        AttachWeaponsToCharacter(CurrentWeapon, TargetSocket);
        IsEquip = false;
        CurrentWeapon = nullptr;
    }
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::OnDrawSwordNotify()
{
    NotifyAttachBool(IsSwordEquip, CurrentRightHandWeapon);
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::OnDrawShieldNotify()
{
    NotifyAttachBool(IsShieldEquip, CurrentLeftHandWeapon);
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::Attack(AActor* TargetActor)
{
   if(CurrentRightHandWeapon && CanAttack())
   {
       CurrentRightHandWeapon->OnAttack(TargetActor);
   }
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::ResetWeaponCombo() 
{
    if (CurrentRightHandWeapon)
    {
        CurrentRightHandWeapon->ResetCombo();
    }
}
//-------------------------------------------------------------------------------------------------------------
bool UWeaponComponent::ReserveAttackToken(int Amount)
{
    if(AttackTokensCount >= Amount)
    {
        AttackTokensCount -= Amount;
        return true;
    }

    return false;
}
//-------------------------------------------------------------------------------------------------------------
void UWeaponComponent::ReturnAttackToken(int Amount)
{
    AttackTokensCount += Amount;
}
//-------------------------------------------------------------------------------------------------------------