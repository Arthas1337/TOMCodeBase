#include "Weapons/TOMOneHandSword.h"
#include "TOMStaminaComponent.h"
#include "TOMWeaponFXComponent.h"
#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraShakeBase.h"
#include "BaseTOMCharacterInterface.h"
#include "Health/TOMHealthComponent.h"

//-------------------------------------------------------------------------------------------------------------
ATOMOneHandSword::ATOMOneHandSword() 
{
    WeaponFXComponent = CreateDefaultSubobject<UTOMWeaponFXComponent>("FXComponent");
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::BeginPlay() 
{
   Super::BeginPlay();
   check(WeaponFXComponent);

   if (!OnMontageEndedDelegate.IsAlreadyBound(this, &ATOMOneHandSword::ReturnTokens))
   {
       OnMontageEndedDelegate.AddDynamic(this, &ATOMOneHandSword::ReturnTokens);
   }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::OnAttack(AActor* TargetActor)
{

    if (!GetWeaponComponent() || !GetStaminaComponent() || !GetPawnOwner() || !GetHealthComponent()) return;

    AttachReturnTokenOnDeathOwner();

    GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

    if (GetStaminaComponent()->IsTired)
    {
        UE_LOG(LogTemp, Warning, TEXT("No Stamina"));
        ResetCombo();
        return;
    }

    if (GetPawnOwner()->GetCharacterMovement()->IsFalling())
    {
      FlyAttack();
    }

    TArray<TObjectPtr<UAnimMontage>> CurrentAttacksAnims = OneHandedLightAttacks;

    ChooseAttacksAnimsMontages(CurrentAttacksAnims);

    if (CurrentAttacksAnims.Num() <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack Anims Array is emmpty!"));
        return;
    }

    if (ComboCounter < CurrentAttacksAnims.Num())
    {
        if (!CurrentAttacksAnims[ComboCounter]) return;

        if (TargetActor)
        {
            const auto EnemyAIINterface = Cast<IBaseTOMCharacterInterface>(TargetActor);
            if (EnemyAIINterface)
            {
                if (!EnemyAIINterface->ReserveAttackToken(TokensAmount))
                {
                    return;
                }
                else
                {
                    EnenmyTargetActor = TargetActor;
                }
            }
        }

        PlayComboAttackAnimation(CurrentAttacksAnims[ComboCounter], TargetActor);
        ++ComboCounter;
    }
    else
    {
        ResetCombo();
        return;
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::FlyAttack()
{
    PlayMontage(FlyAttackMontage);
    GetStaminaComponent()->ReduceStamina(AttackTiredness);
    return;
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::ReturnTokens() 
{
    if (EnenmyTargetActor)
    {
        const auto CharacterInterface = Cast<IBaseTOMCharacterInterface>(EnenmyTargetActor);
        if (CharacterInterface)
        {
            CharacterInterface->ReturnAttackToken(TokensAmount);
            EnenmyTargetActor = nullptr;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::PlayComboAttackAnimation(const TObjectPtr<UAnimMontage>& AttackAnimation, AActor* TargetActor)
{
    GetStaminaComponent()->ReduceStamina(AttackTiredness);
    PlayMontage(AttackAnimation);
    IsAttacked.Broadcast();
    GetWeaponComponent()->IsAttacking = true;
    GetWorld()->GetTimerManager().SetTimer(
        ComboTimerHandle, this, &ATOMOneHandSword::ResetCombo, AttackAnimation->CalculateSequenceLength(), false);
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::ChooseAttacksAnimsMontages(TArray<TObjectPtr<UAnimMontage>>& CurrentAttacksAnimsMontages) 
{
    switch (GetWeaponComponent()->LeftHandState)
    {
        case ELeftHandStates::TwoHandSword: CurrentAttacksAnimsMontages = OneHandedLightAttacks; break;

        case ELeftHandStates::Shield: CurrentAttacksAnimsMontages = WithShieldAttacks; break;

        default: break;
    }
}
void ATOMOneHandSword::AttachReturnTokenOnDeathOwner()
{
    const auto HealthComponent = GetHealthComponent();
    if (!HealthComponent) return;

    if (!HealthComponent->OnDeath.IsAlreadyBound(this, &ATOMOneHandSword::ReturnTokens))
    {
        HealthComponent->OnDeath.AddDynamic(this, &ATOMOneHandSword::ReturnTokens);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::ThrustAttack() 
{
    GetStaminaComponent()->ReduceStamina(AttackTiredness);
    PlayMontage(ThrustMontage);
    IsAttacked.Broadcast();
    GetWeaponComponent()->IsAttacking = true;
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::ResetCombo()
{
    GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
    ComboCounter = 0;
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::TraceSwordHit() 
{
    if (!GetPawnOwner() || !GetWorld() || !GetWeaponComponent()) return;

    FCollisionObjectQueryParams ObjectTypes;
    ObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

    TArray<TObjectPtr<AActor>> IgnoreArr;

    if (GetOwner())
    { 
        IgnoreArr.Add(GetOwner());
    }

    const auto InstanceAttachMesh =
        Cast<USkeletalMeshComponent>(GetPawnOwner()->GetDefaultSubobjectByName(GetWeaponComponent()->InstanceSkeleton));
    if (!InstanceAttachMesh) return;

    FVector SwordStartLocation = InstanceAttachMesh->GetSocketLocation(DamageStartLocation);
    FVector SwordEndLocation = InstanceAttachMesh->GetSocketLocation(DamageEndLocation);

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = true;
    QueryParams.AddIgnoredActors(IgnoreArr);

    FHitResult HitResult;
    bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, SwordStartLocation, SwordEndLocation, FQuat::Identity, ObjectTypes,
        FCollisionShape::MakeSphere(SphereRadius), QueryParams);

    if (bHit && HitResult.GetActor())
    {
        if (GetPawnOwner()->Tags.Contains(FriendlyTag) && HitResult.GetActor()->Tags.Contains(FriendlyTag) && !bFriendlyDamageAI) return;

        if (!HitActors.Contains(HitResult.GetActor()))
        {
            if (const auto CharacterInterface = Cast<IBaseTOMCharacterInterface>(GetPawnOwner()))
            {
                const auto CharacterOverlayState = CharacterInterface->GetOverlayState();

                switch (CharacterOverlayState)
                {
                   case EOverlayStates::TwoHandSword:

                        ApplyPointDamageOverlayState(HitResult.GetActor(), TwoHandSwordDamage,
                            (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal(), HitResult, GetPawnOwner()->GetController(),
                           GetPawnOwner(), UDamageType::StaticClass(), EOverlayStates::TwoHandSword);

                   break;

                   case EOverlayStates::Shield:

                       ApplyPointDamageOverlayState(HitResult.GetActor(), OneHandSwordDamage,
                           (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal(), HitResult, GetPawnOwner()->GetController(),
                           GetPawnOwner(), UDamageType::StaticClass(), EOverlayStates::Shield);

                    break;
                }

            }
        }
        HitActors.Add(HitResult.GetActor());
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMOneHandSword::ApplyPointDamageOverlayState(AActor* DamagedActor, float BaseDamage, FVector const& HitFromDirection,
    FHitResult const& HitInfo, AController* EventInstigator, AActor* DamageCauser, TSubclassOf<UDamageType> DamageTypeClass,
    EOverlayStates OverlayState)
{
    UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, HitFromDirection, HitInfo, EventInstigator, DamageCauser, DamageTypeClass);
    PlayCameraShake(GetPlayerController(), OverlayState);
    const auto DamagedActorHealthComponent = DamagedActor->FindComponentByClass<UTOMHealthComponent>();

    if (DamagedActor->CanBeDamaged() && !DamagedActorHealthComponent->IsDead())
    {
        WeaponFXComponent->PlayImpactFX(HitInfo);
    }
}
//-------------------------------------------------------------------------------------------------------------
