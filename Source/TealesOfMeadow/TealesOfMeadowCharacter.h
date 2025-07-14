#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TOMUtility.h"
#include "Interfaces/BaseTOMCharacterInterface.h"
#include "TealesOfMeadowCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAnimMontage;
class ATOMBaseWeapon;
class UTOMHealthComponent;
class UTOMStaminaComponent;
class UWeaponComponent;
class UMotionWarpingComponent;
class UDetectEnemyComponent;
class UHitBodyDetectorComponent;
class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

//-------------------------------------------------------------------------------------------------------------
UCLASS(config = Game)
class ATealesOfMeadowCharacter : public ACharacter, public IBaseTOMCharacterInterface
{

public:
    GENERATED_BODY()

    ATealesOfMeadowCharacter();

    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay();
    virtual void Tick(float DeltaTime) override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void OnSprintStart(const FInputActionValue& Value);
    void ReducedStaminaDuringSprint();
    void OnSprintEnd(const FInputActionValue& Value);
    void OnCrouchAction(const FInputActionValue& Value);
    void StartLightAttack(const FInputActionValue& Value);

    UFUNCTION()
    void OnDrawSwordAction(const FInputActionValue& Value);
    void OnDrawShieldAction(const FInputActionValue& Value);
    void OnThrustAction(const FInputActionValue& Value);

    UFUNCTION()
    void OnTakeDamage();

    UFUNCTION()
    virtual void OnDeath();

    UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Overlay")
    void NewOverlayAnim(); // Blueprint implementation

    UFUNCTION(BlueprintCallable)
    void WarpTarget();
    void UpdateCloseActor();
    virtual void ActorToEnemyTarget(float DeltaTime);

    UFUNCTION(BlueprintCallable)
    virtual void Dodge() override;

    virtual bool GetIsDodging() const override { return IsDodging; }
    virtual bool GetIsAttackingPure() const override { return IsAttackingPure; }

    virtual bool ReserveAttackToken(int Amount) override;
    virtual void ReturnAttackToken(int Amount) override;

    virtual EOverlayStates GetOverlayState() const override { return OverlayState; }

protected:

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SKM")
    TObjectPtr<USkeletalMeshComponent> KnightMesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
    TObjectPtr<UTOMHealthComponent> HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
    TObjectPtr<UTOMStaminaComponent> StaminaComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TObjectPtr<UWeaponComponent> WeaponComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Motion Warping")
    TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detect Enemy")
    TObjectPtr<UDetectEnemyComponent> DetectEnemyComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detect Enemy")
    TObjectPtr<UHitBodyDetectorComponent> HitBodyDetectorComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Detect Enemy")
    TObjectPtr<USphereComponent> CheckEnemySphere;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
    TArray<FTakeHitState> TakeHitStates;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dodge")
    TObjectPtr<UAnimMontage> CurrentDodgeAnimMontage;

    TObjectPtr<AActor> CurrentTarget;
    TArray<TObjectPtr<AActor>> DetectedEnemies;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
    USoundBase* TakeDamageSound = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FVector LandVelocity;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FRotator AORotateTarget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FCharacterInputState CharacterInputState;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    E_Gait Gait;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dodge")
    EDodgeDirection DodgeDirection = EDodgeDirection::Backward;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    EOverlayStates OverlayState = EOverlayStates::Default;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Warp")
    FName WarpTargetName = "AttackTarget";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movment)
    float SprintSpeed = 500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float WalkSpeed = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float SprintTiredness = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    float FrequencySprintTiredness = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Block")
    float StaminanBlockReduce = 25.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Target")
    float ActorToEnemyRotateSpeed = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dodge")
    float DodgeTiredness = 15.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dodge")
    float DodgeAnimMontageRate = 1.3f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Death")
    float DestroyActorTime = 4.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    bool IsRunning = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsJumping = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsWalk = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsCrouch = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
    bool TargetLocked = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsAiming = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsDodging = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool WantsTireSprint = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool DoingTraversalAction = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool JustLanded = false;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool bIsRagdolling = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool HasTarget = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool bUseStateMachine = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsAttackingPure = false;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsCanRun = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    bool IsHurt = false;

private:
    void PlayMontage(UAnimMontage* Montage);

    UFUNCTION()
    void OnDrawWeaponAction(const EWeaponType& WeaponType, const bool& IsWeaponEquip);

    bool CantMove() const;
    bool CantAction() const;
    void PlayHitAnimMontage(TArray<UAnimMontage*> Montages, float StaminaReduce);
    void TakeHitChooseAnimation();

    UFUNCTION()
    virtual void OnCheckEnemySphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
       int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnCheckEnemySphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    FTimerHandle SprintReducedStaminaTimerHandle;
    FTimerHandle SheathWeaponTimerHandle;
    FTimerHandle DisableDamageTimerHandle;
    FTimerHandle EnableDamageTimerHandle;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;    

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> CrouchAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LightAttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> DrawSwordAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> DrawShieldAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> BlockAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> ThrustAction;
};
//-------------------------------------------------------------------------------------------------------------
