#include "Enemy/TOMEnemy.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Weapons/Components/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/TOMBaseWeapon.h"
#include "Camera/CameraComponent.h"
#include "TOMHealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TealesOfMeadow/TealesOfMeadowGameMode.h"

ATOMEnemy::ATOMEnemy()
{
   GetFollowCamera()->DestroyComponent();
   GetCameraBoom()->DestroyComponent();

	LockOnCollision = CreateDefaultSubobject<UCapsuleComponent>("LockOnCollision");
	LockOnCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   LockOnCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

   AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
   Tags.Add("GangMember");
}

void ATOMEnemy::OnDeath()
{
    if (!GetWorld()) return;

    SetLifeSpan(DestroyActorTimeAI);
    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    bIsRagdolling = true;
    
    GetWorldTimerManager().SetTimer(DestroyWeaponsTimerHandle, this,&ATOMEnemy::OnDestroyWeapons, DestroyActorTimeAI - 0.1f);

    OnEnemyDead.Broadcast();
}


void ATOMEnemy::OnDestroyWeapons() 
{
    for (auto& i : WeaponComponent->SpawnWeaponsMap)
    {
        if (i.Value)
        {
            i.Value->Destroy();
        }
    }

    WeaponComponent->SpawnWeaponsMap.Empty();
}

void ATOMEnemy::ActorToEnemyTarget(float DeltaTime) 
{
    if (!CurrentTarget || !HasTarget)
    {
        CurrentTarget = nullptr;
        HasTarget = false;
        return;
    }

    const auto TargetHealthComponent = CurrentTarget->FindComponentByClass<UTOMHealthComponent>();
    if (!TargetHealthComponent || TargetHealthComponent->IsDead() || CurrentTarget->Tags.Contains("GangMember"))
    {
        DetectedEnemies.Remove(CurrentTarget);
        UpdateCloseActor();
        return;
    }

    const auto RotateTowargToTarget = FVector(CurrentTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    const FRotator TargetRotation = RotateTowargToTarget.Rotation();
    AORotateTarget = TargetRotation;

    const auto ActorTowardEnemyRotation =
        FMath::RInterpConstantTo(GetActorRotation(), FRotator(0.0f, TargetRotation.Yaw, 0.0f), DeltaTime, ActorToEnemyRotateSpeed);

    SetActorRotation(ActorTowardEnemyRotation);
}

float ATOMEnemy::GetAttackRadius() const
{
    return AttackRadius;
}

float ATOMEnemy::GetDefendRadius() const
{
    return DefendRadius;
}

void ATOMEnemy::SetMovementSpeed(EMovementSpeed MovementType)
{

    switch(MovementType)
    {
        case EMovementSpeed::Runnig: 
           IsWalk = false;
           IsRunning = true;
           break;

        case EMovementSpeed::Walking: 
           IsRunning = false;
           IsWalk = true;
           break;

        default: break;
    }
}

bool ATOMEnemy::ReserveAttackToken(int Amount)
{
    if(WeaponComponent->ReserveAttackToken(Amount))
    {
      TokenAmountOnDeath = Amount;
      return true;
    }
    else
    {
       return false;
    }
}

void ATOMEnemy::ReturnAttackToken(int Amount) 
{
    WeaponComponent->ReturnAttackToken(Amount);
    TokenAmountOnDeath -= Amount;
}
