#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "TOMUtility.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Unarmed UMETA(DisplayName = "Unarmed"),
    Sword UMETA(DisplayName = "Sword"),
    Shield UMETA(DisplayName = "Shield"),
    TwoHandSword UMETA(DisplayName = "Two Hand Sword"),
    Bow UMETA(DisplayName = "Bow")
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SheathSocket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName HandSocket;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> DrawMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> SheathMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> SwordWithShieldDrawMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimMontage> SwordWithShieldSheathMontage;
};

UENUM(BlueprintType)
enum class ELeftHandStates : uint8
{
    Unarmed UMETA(DisplayName = "Unarmed"),
    Shield UMETA(DisplayName = "Shield"),
    TwoHandSword UMETA(DisplayName = "Two Hand Sword"),
    Bow UMETA(DisplayName = "Bow"),
    Aiming UMETA(DisplayName = "Aiming")
};

UENUM(BlueprintType)
enum class ERightHandStates : uint8
{
    Unarmed UMETA(DisplayName = "Unarmed"),
    OneHandSword UMETA(DisplayName = "One Hand Sword"),
    TwoHandSword UMETA(DisplayName = "Two Hand Sword")
};

UENUM(BlueprintType)
enum class ETOMGameState : uint8
{
    WaitingToStart UMETA(DisplayName = "WaitingToStart"),
    InProgress UMETA(DisplayName = "InProgress"),
    Pause UMETA(DisplayName = "Pause"),
    GameOver UMETA(DisplayName = "GameOver"),
    Fight UMETA(DisplayName = "Fight")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameChangeStateSignature, ETOMGameState, GameState);

USTRUCT(BlueprintType)
struct FCharacterInputState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool WantsToSprint = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool WantsToWalk = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool WantsToStrafe = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool WantsToAim = false;
};

UENUM(BlueprintType)
enum class E_Gait : uint8
{
    Walk UMETA(DisplayName = "Walk"),
    Run UMETA(DisplayName = "Run"),
    Sprint UMETA(DisplayName = "Sprint"),
    Hurt UMETA(DisplayName = "Hurt")
};

UENUM(BlueprintType)
enum class EOverlayStates : uint8
{
    Default UMETA(DisplayName = "Default"),
    TwoHandSword UMETA(DisplayName = "TwoHandSword"),
    Shield UMETA(DisplayName = "Shield"),
    Torch UMETA(DisplayName = "Torch"),
    Bow UMETA(DisplayName = "Bow"),
    SwordBlock UMETA(DisplayName = "SwordBlock"),
    ShieldBlock UMETA(DisplayName = "ShieldBlock"),
    Hurt UMETA(DisplayName = "Hurt")
};

UENUM(BlueprintType)
enum class EDodgeDirection : uint8
{
    Forward UMETA(DisplayName = "Forward"),
    Backward UMETA(DisplayName = "Backward"),
    Right UMETA(DisplayName = "Right"),
    Left UMETA(DisplayName = "Left")

};

USTRUCT(BlueprintType)
struct FTakeHitState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TObjectPtr<UAnimMontage>> TakeHitMontages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EOverlayStates HitOverlayState = EOverlayStates::Default;
};

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
    Runnig UMETA(DisplayName = "Running"),
    Walking UMETA(DisplayName = "Walking")
};

UENUM(BlueprintType)
enum class EAIStates : uint8
{
    Patrolling UMETA(DisplayName = "Patrolling"),
    Attacking UMETA(DisplayName = "Attacking"),
    Investigate UMETA(DisplayName = "Investigate")
};