#include "Components/DetectEnemy/DetectEnemyComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/DrawSphereComponent.h"
#include "TOMEnemy.h"

DEFINE_LOG_CATEGORY(LogDetectEnemyComponent);

//-------------------------------------------------------------------------------------------------------------
ACharacter* UDetectEnemyComponent::GetPawnOwner()
{
    if (!GetOwner())
    {
        return nullptr;
    }

    const auto PawnOwner = Cast<ACharacter>(GetOwner());
    if (!PawnOwner)
    {
        UE_LOG(LogDetectEnemyComponent, Error, TEXT("Bad Cast To Character WeaponComponet"));
        return nullptr;
    }

    return PawnOwner;
}
//-------------------------------------------------------------------------------------------------------------
TArray<TObjectPtr<AActor>> UDetectEnemyComponent::TraceForTargets()
{

    if (!GetPawnOwner() || !GetWorld()) return TArray<TObjectPtr<AActor>>();

    FCollisionObjectQueryParams ObjectTypes;
    ObjectTypes.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);

    TArray<TObjectPtr<AActor>> IgnoreArr;

    if (GetOwner())
    {
        IgnoreArr.Add(GetOwner());
    }

    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;

    QueryParams.bTraceComplex = true;
    QueryParams.AddIgnoredActors(IgnoreArr);

    bool bHit = GetWorld()->SweepMultiByObjectType(HitResults, GetPawnOwner()->GetActorLocation(), GetPawnOwner()->GetActorLocation(),
        FQuat::Identity, ObjectTypes, FCollisionShape::MakeSphere(LockOnRadius), QueryParams);
    TArray<TObjectPtr<AActor>> LocalTargets;

    if (bHit)
    {
        for (auto& Hit : HitResults)
        {
            if (!Hit.GetActor()) continue;

            //if (Hit.GetActor()->GetClass() == LockOnClass)
            //{
                LocalTargets.Add(Hit.GetActor());
            //}
        }
    }

    return LocalTargets;
}
//-------------------------------------------------------------------------------------------------------------
float UDetectEnemyComponent::CheckHowCloseTargetIsToCenter(TObjectPtr<AActor> Target, float LocMaxAngleDegrees)
{
    if (!Target || !GetPawnOwner()) return 0.0f;

    FVector ForwardVector = GetPawnOwner()->GetRootComponent()->GetForwardVector();
    ForwardVector.Normalize();
    FVector TargetDirection = (Target->GetActorLocation() - GetPawnOwner()->GetActorLocation()).GetSafeNormal();

    float DotProduct = FVector::DotProduct(ForwardVector, TargetDirection);

    float AngleRadians = FMath::Acos(DotProduct);
    float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

    float Alignment = FMath::Clamp(1.0f - (AngleDegrees / LocMaxAngleDegrees), 0.0f, 1.0f);

    return Alignment;
}
//-------------------------------------------------------------------------------------------------------------
TObjectPtr<AActor> UDetectEnemyComponent::CheckForClosestTarget(TArray<TObjectPtr<AActor>> Targets)
{
    if (!GetPawnOwner()) return nullptr;

    FCollisionQueryParams QueryParams;

    float LocalCompareFloat = 0.0f;
    TObjectPtr<AActor> LocalTargetLockOn;

    for (auto& Target : Targets)
    {
        FHitResult HitResult;
        bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, GetPawnOwner()->GetRootComponent()->GetComponentLocation(),
            Target->GetActorLocation(), ECollisionChannel::ECC_Visibility);

        if (bHit)
        {
            if (!HitResult.GetActor()) continue;

            //if (HitResult.GetActor()->GetClass() == LockOnClass)
            //{
                const auto HowCloseTargetIsToCenter = CheckHowCloseTargetIsToCenter(HitResult.GetActor(), MaxAngleDegrees);
                if (HowCloseTargetIsToCenter > LocalCompareFloat)
                {
                    LocalCompareFloat = HowCloseTargetIsToCenter;
                    LocalTargetLockOn = HitResult.GetActor();
                }
            //}
        }
    }

    return LocalTargetLockOn;
}
//-------------------------------------------------------------------------------------------------------------
AActor* UDetectEnemyComponent::FindClosestEnemy() 
{
    if(!CheckForClosestTarget(TraceForTargets())) return nullptr;
    return CheckForClosestTarget(TraceForTargets());
}
//-------------------------------------------------------------------------------------------------------------
