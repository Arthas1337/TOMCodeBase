#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"
#include "Animation/AnimInstance.h"
#include "TOMUtility.h"
#include "TOMBaseWeapon.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttacked);
DECLARE_LOG_CATEGORY_EXTERN(LogBaseWeapon, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMontageEndedSignature);

class USkeletalMeshComponent;
class UTOMStaminaComponent;
class UWeaponComponent;
class UCameraShakeBase;
class UDetectEnemyComponent;
class UTOMHealthComponent;

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API ATOMBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ATOMBaseWeapon();

    virtual void OnAttack(AActor* TargetActor) { return; };
    virtual void ResetCombo() {return;};
    virtual void ThrustAttack() { return; };
    void OnNotifyStateEnd();
    virtual void TraceSwordHit() { return; };

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnAttacked IsAttacked;

protected:

    virtual void BeginPlay() override;

    UFUNCTION()
    void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<USkeletalMeshComponent> WeaponMesh;

    TArray<TObjectPtr<AActor>> HitActors;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
    TMap<EOverlayStates, TSubclassOf<UCameraShakeBase>> CameraShakeClassMap;

    UPROPERTY()
    FOnMontageEndedSignature OnMontageEndedDelegate;

    UPROPERTY()
    FOnMontageEndedSignature OnMontageInterruptedDelegate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageSystem")
    FName DamageStartLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageSystem")
    FName DamageEndLocation;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DamageSystem")
    float SphereRadius = 50.0f;

    TObjectPtr<ACharacter> GetPawnOwner();
    TObjectPtr<UTOMStaminaComponent> GetStaminaComponent();
    TObjectPtr<UWeaponComponent> GetWeaponComponent();
    TObjectPtr<UDetectEnemyComponent> GetDetectEnemyComponent();
    TObjectPtr<UTOMHealthComponent> GetHealthComponent();
    void PlayMontage(const TObjectPtr<UAnimMontage>& Montage);
    void PlayCameraShake(const TObjectPtr<APlayerController>& Controller, const EOverlayStates& CameraShakeState,const float& scale = 1.0f);
    TObjectPtr<APlayerController> GetPlayerController();
    
};
//-------------------------------------------------------------------------------------------------------------
