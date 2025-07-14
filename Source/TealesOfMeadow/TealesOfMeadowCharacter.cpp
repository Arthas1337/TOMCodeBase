#include "TealesOfMeadowCharacter.h"
#include "TealesOfMeadowGameMode.h"
#include "Animation/AnimInstance.h"

//Components
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Health/TOMHealthComponent.h"
#include "Stamina/TOMStaminaComponent.h"
#include "Weapons/Components/WeaponComponent.h"
#include "Debug/DebugDrawComponent.h"
#include "Components/DetectEnemy/DetectEnemyComponent.h"
#include "Components/HitBodyDetectorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "MotionWarpingComponent.h"
#include "Components/SphereComponent.h"

#include "Weapons/TOMBaseWeapon.h"
#include "Engine/HitResult.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "InputActionValue.h"
#include "TOMEnemy.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//-------------------------------------------------------------------------------------------------------------
ATealesOfMeadowCharacter::ATealesOfMeadowCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, -90.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 500.f;
    GetCharacterMovement()->AirControl = 0.25f;
    GetCharacterMovement()->MaxWalkSpeed = 200.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 150.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 750.0f;
    GetCharacterMovement()->BrakingDecelerationFalling = 0.0f;

    KnightMesh = CreateDefaultSubobject<USkeletalMeshComponent>("KnightMesh");
    KnightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    KnightMesh->SetupAttachment(GetMesh());

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UTOMHealthComponent>("HealthComponent");
    StaminaComponent = CreateDefaultSubobject<UTOMStaminaComponent>("StaminaComponent");
    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarpingComponent");
    DetectEnemyComponent = CreateDefaultSubobject<UDetectEnemyComponent>("DetectEnemyComponent");
    HitBodyDetectorComponent = CreateDefaultSubobject<UHitBodyDetectorComponent>("HitBodyDetectorComponent");

    CheckEnemySphere = CreateDefaultSubobject<USphereComponent>("CheckEnemySphere");
    CheckEnemySphere->SetupAttachment(GetCapsuleComponent());
    CheckEnemySphere->SetSphereRadius(250.0f);
    CheckEnemySphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(GetCharacterMovement());
    check(HealthComponent);
    check(StaminaComponent);
    check(WeaponComponent);
    check(MotionWarpingComponent);
    check(DetectEnemyComponent);

    HealthComponent->OnDamage.AddDynamic(this, &ThisClass::OnTakeDamage);
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
    CheckEnemySphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCheckEnemySphereOverlapBegin);
    CheckEnemySphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCheckEnemySphereOverlapEnd);
    WeaponComponent->SpawnWeapons();
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);

   if(DetectedEnemies.Num() > 0 && HasTarget)
   {
      UpdateCloseActor();
      ActorToEnemyTarget(DeltaTime);
   }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(GetController()))
    {
        if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::Look);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATealesOfMeadowCharacter::OnSprintStart);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATealesOfMeadowCharacter::OnSprintEnd);
        EnhancedInputComponent->BindAction(LightAttackAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::StartLightAttack);
        EnhancedInputComponent->BindAction(DrawSwordAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::OnDrawSwordAction);
        EnhancedInputComponent->BindAction(DrawShieldAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::OnDrawShieldAction);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::OnCrouchAction);
        EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Triggered, this, &ATealesOfMeadowCharacter::OnThrustAction);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error,
            TEXT("'%s' Failed to find an Enhanced Input component!"),
            *GetNameSafe(this));
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::Move(const FInputActionValue& Value)
{

    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {

        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::PlayMontage(UAnimMontage* Montage)
{
    if (!Montage || !GetMesh()) return;

    TObjectPtr<UAnimInstance> AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(Montage);
    }
}
//-------------------------------------------------------------------------------------------------------------
bool ATealesOfMeadowCharacter::CantMove() const
{
    return WeaponComponent->IsDrawing || WeaponComponent->IsAttacking || GetCharacterMovement()->IsFlying() 
       || WeaponComponent->IsHit || IsHurt || !IsCanRun;
}
//-------------------------------------------------------------------------------------------------------------
bool ATealesOfMeadowCharacter::CantAction() const
{
    return WeaponComponent->IsDrawing || WeaponComponent->IsAttacking 
       || GetCharacterMovement()->IsFlying() || WeaponComponent->IsHit 
       || GetCharacterMovement()->IsCrouching() || StaminaComponent->IsTired 
       || IsDodging || !IsCanRun || IsHurt;
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnSprintStart(const FInputActionValue& Value)
{
    if (CantMove())
    {
        IsWalk = true;
        IsRunning = false;
        CharacterInputState.WantsToStrafe = false;
        return;
    }

    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

    IsWalk = false;
    IsRunning = true;
    CharacterInputState.WantsToStrafe = true;
    OverlayState = EOverlayStates::Default;

    if (WantsTireSprint && GetCharacterMovement()->Velocity.Length() >= 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            SprintReducedStaminaTimerHandle, this, &ATealesOfMeadowCharacter::ReducedStaminaDuringSprint, FrequencySprintTiredness, true);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::ReducedStaminaDuringSprint()
{
    if (CantAction() || GetCharacterMovement()->Velocity.Length() <= 0.0f)
    {
        UE_LOG(LogTemplateCharacter, Warning, TEXT("No Stamina"));
        OnSprintEnd(FInputActionValue());
        return;
    }

    StaminaComponent->ReduceStamina(SprintTiredness);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnSprintEnd(const FInputActionValue& Value)
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    IsRunning = false;
    IsWalk = true;
    if (WantsTireSprint)
    {
        GetWorld()->GetTimerManager().ClearTimer(SprintReducedStaminaTimerHandle);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnCrouchAction(const FInputActionValue& Value)
{
    if (CantMove()) return;

    if (GetCharacterMovement()->IsCrouching())
    {
        UnCrouch();
    }
    else if (!GetCharacterMovement()->IsCrouching())
    {
        Crouch();
    }
    OverlayState = EOverlayStates::Default;
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::StartLightAttack(const FInputActionValue& Value)
{
    if (IsRunning || IsAiming) return;

    WeaponComponent->Attack(nullptr);

}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::WarpTarget()
{
    if (DetectEnemyComponent)
    {
        const auto ClosestEnemy = DetectEnemyComponent->FindClosestEnemy();

        if (ClosestEnemy)
        {
            const auto EnemyHealthComponent = ClosestEnemy->FindComponentByClass<UTOMHealthComponent>();
            if (!EnemyHealthComponent || EnemyHealthComponent->IsDead())
            {
                UE_LOG(LogWeaponComponent, Warning, TEXT("Enemy is dead"));
                MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
                return;
            }

            const auto LookAtRotaion =
                UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ClosestEnemy->GetActorLocation());

            if (!MotionWarpingComponent) return;

            FMotionWarpingTarget WarpingTarget;

            WarpingTarget.Name = WarpTargetName;
            WarpingTarget.Location = ClosestEnemy->GetActorLocation();
            WarpingTarget.Rotation = LookAtRotaion;

            MotionWarpingComponent->AddOrUpdateWarpTarget(WarpingTarget);
        }
        else
        {
            MotionWarpingComponent->RemoveWarpTarget(WarpTargetName);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnTakeDamage()
{
    if(IsDodging) return;
    WeaponComponent->ResetWeaponCombo();
    WeaponComponent->IsAttacking = false;
    WeaponComponent->IsDrawing = false;
    WeaponComponent->IsHit = false;

    TakeHitChooseAnimation();

    UGameplayStatics::PlaySoundAtLocation(
        this,
        TakeDamageSound,
        GetActorLocation(),
        GetActorRotation(),
        1.0f,
        1.0f,
        0.0f,
        nullptr,
        nullptr);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::PlayHitAnimMontage(TArray<UAnimMontage*> Montages, float StaminaReduce = 0.0f)
{
    if (Montages.IsEmpty()) return;

    int RandIndex = FMath::RandRange(0, Montages.Num() - 1);
    PlayMontage(Montages[RandIndex]);
    WeaponComponent->IsHit = true;
    StaminaComponent->ReduceStamina(StaminaReduce);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::TakeHitChooseAnimation() 
{
    for (auto& TakeHitState : TakeHitStates)
    {
        if (TakeHitState.HitOverlayState != OverlayState)
        {
            continue;
        }

        bool bIsBlocking = WeaponComponent->IsShieldBlocking || WeaponComponent->IsSwordBlocking;

        PlayHitAnimMontage(TakeHitState.TakeHitMontages, bIsBlocking ? StaminanBlockReduce : 0.0f);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnCheckEnemySphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(OtherActor && OtherActor->IsA(ATealesOfMeadowCharacter::StaticClass()))
    {
         DetectedEnemies.Add(OtherActor);
         UpdateCloseActor();
         HasTarget = (CurrentTarget != nullptr);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnCheckEnemySphereOverlapEnd(
    UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(ATealesOfMeadowCharacter::StaticClass()))
    {
        DetectedEnemies.Remove(OtherActor);
        UpdateCloseActor();
        HasTarget = (CurrentTarget != nullptr);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::ActorToEnemyTarget(float DeltaTime) 
{
    if (!CurrentTarget || !HasTarget || DoingTraversalAction)
    {
        CurrentTarget = nullptr;
        HasTarget = false;
        return;
    }

    const auto TargetHealthComponent = CurrentTarget->FindComponentByClass<UTOMHealthComponent>();
    if(!TargetHealthComponent || TargetHealthComponent->IsDead())
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
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::Dodge() 
{
   if(!IsDodging && StaminaComponent->GetStamina() > 0.0f)
   {
       StaminaComponent->ReduceStamina(DodgeTiredness);
       IsDodging = true;
       PlayAnimMontage(CurrentDodgeAnimMontage, DodgeAnimMontageRate);
       SetCanBeDamaged(false);
       WeaponComponent->ResetWeaponCombo();
       GetWorldTimerManager().SetTimer(DisableDamageTimerHandle, [this]() { SetCanBeDamaged(true); }, 0.65f, false);
   }
}
//-------------------------------------------------------------------------------------------------------------
bool ATealesOfMeadowCharacter::ReserveAttackToken(int Amount)
{
    return WeaponComponent->ReserveAttackToken(Amount);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::ReturnAttackToken(int Amount) 
{
    WeaponComponent->ReturnAttackToken(Amount);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::UpdateCloseActor() 
{
   AActor* ClosestEnemy = nullptr;
   float ClosestDistance = FLT_MAX;

   for(auto& Enemy : DetectedEnemies)
   {
       float Distance = FVector::Dist(GetActorLocation(), Enemy->GetActorLocation());
       if (Distance < ClosestDistance)
       {
           ClosestDistance = Distance;
           ClosestEnemy = Enemy;
       }
   }

   CurrentTarget = ClosestEnemy;
   HasTarget = (CurrentTarget != nullptr);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnDeath()
{
    if(!GetWorld()) return;

    const auto GameMode = Cast<ATealesOfMeadowGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode)
    {
       GameMode->SetMatchState(ETOMGameState::GameOver);
    }

    SetLifeSpan(DestroyActorTime);
    GetCharacterMovement()->DisableMovement();

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
    bIsRagdolling = true;

    for (auto& i : WeaponComponent->SpawnWeaponsMap)
    {
        if (i.Value)
        {
            i.Value->Destroy();
        }
    }

    WeaponComponent->SpawnWeaponsMap.Empty();
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnDrawWeaponAction(const EWeaponType& WeaponType, const bool& IsWeaponEquip)
{
    if (IsAiming || GetCharacterMovement()->IsFalling() || IsHurt) return;
    WeaponComponent->DrawWeapon(WeaponType, IsWeaponEquip);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnDrawSwordAction(const FInputActionValue& Value)
{
    OnDrawWeaponAction(EWeaponType::Sword, WeaponComponent->IsSwordEquip);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnDrawShieldAction(const FInputActionValue& Value)
{
    OnDrawWeaponAction(EWeaponType::Shield, WeaponComponent->IsShieldEquip);
}
//-------------------------------------------------------------------------------------------------------------
void ATealesOfMeadowCharacter::OnThrustAction(const FInputActionValue& Value) 
{
    if (WeaponComponent->CurrentRightHandWeapon && WeaponComponent->CanAttack() && 
       OverlayState == EOverlayStates::TwoHandSword || OverlayState == EOverlayStates::SwordBlock)
    {
       WeaponComponent->CurrentRightHandWeapon->ThrustAttack();
    }
}
//-------------------------------------------------------------------------------------------------------------