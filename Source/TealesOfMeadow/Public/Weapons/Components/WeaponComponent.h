#pragma once

#include "Components/ActorComponent.h"
#include "TOMUtility.h"
#include "CoreMinimal.h"
#include "WeaponComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWeaponComponent, Log, All);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDraw);

class ATOMBaseWeapon;
class UAnimMontage;
class ACharacter;
class UTOMStaminaComponent;
class ATOMEnemy;
class UDetectEnemyComponent;
class UMotionWarpingComponent;
class IBaseTOMCharacterInterface;

//-------------------------------------------------------------------------------------------------------------
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEALESOFMEADOW_API UWeaponComponent : public UActorComponent
{
public:
    GENERATED_BODY()

    void SpawnWeapons();
    void DrawWeapon(const EWeaponType& WeaponType, const bool& bIsDrawing);
    bool IsDrawn();

    UFUNCTION()
    bool CanAttack();
    void OnDrawSwordNotify();
    void OnDrawShieldNotify();
    void Attack(AActor* TargetActor);
    void ResetWeaponCombo();

    bool ReserveAttackToken(int Amount);
    void ReturnAttackToken(int Amount);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Overlay")
    ELeftHandStates LeftHandState = ELeftHandStates::Unarmed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Overlay")
    ERightHandStates RightHandState = ERightHandStates::Unarmed;

    TMap<FName, TObjectPtr<ATOMBaseWeapon>> SpawnWeaponsMap;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<ATOMBaseWeapon> CurrentRightHandWeapon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<ATOMBaseWeapon> CurrentLeftHandWeapon = nullptr;

    FName TargetSocket;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FName InstanceSkeleton = "KnightMesh";

    UPROPERTY(BlueprintAssignable, BlueprintCallable)
    FOnDraw OnDraw;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsSwordEquip = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsShieldEquip = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsBowEquip = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsAttacking = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsDrawing = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsHit = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsLongSword = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Block")
    bool IsShieldBlocking = false;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Block")
    bool IsSwordBlocking = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int AttackTokensCount = 1;

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TMap<FName, TSubclassOf<ATOMBaseWeapon>> WeaponsClassesMap;

    UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TMap<EWeaponType, FWeaponInfo> WeaponInfoMap;

private:

    void PlayMontage(const TObjectPtr<UAnimMontage>& Montage);
    void AttachWeaponsToCharacter(const TObjectPtr<ATOMBaseWeapon> Weapon, const FName& Socket);
    ACharacter* GetPawnOwner();
    IBaseTOMCharacterInterface* GetPawnOwnerInterface();
    UTOMStaminaComponent* GetStaminaComponent();
    TObjectPtr<UDetectEnemyComponent> GetDetectEnemyComponent();
    TObjectPtr<UMotionWarpingComponent> GetMotionWarpingComponent();
    void NotifyAttachBool(bool& IsEquip, TObjectPtr<ATOMBaseWeapon>& CurrentWeapon);
    void ChangeHandState(const bool& bIsDrawing, const FWeaponInfo& WeaponInfo, const bool& IsRightHandState);
};
//-------------------------------------------------------------------------------------------------------------
