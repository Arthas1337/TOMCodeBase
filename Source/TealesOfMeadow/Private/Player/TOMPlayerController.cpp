#include "Player/TOMPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TealesOfMeadow/TealesOfMeadowGameMode.h"
#include "TOMPlayerHUD.h"

//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerController::BeginPlay() 
{
   Super::BeginPlay();

   const auto GameMode = Cast<ATealesOfMeadowGameMode>(GetWorld()->GetAuthGameMode());
   if (GameMode)
   {
       GameMode->OnGameChangeState.AddDynamic(this, &ATOMPlayerController::OnMatchStateChanged);
   }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerController::SetupInputComponent() 
{
    Super::SetupInputComponent();

    if(!InputComponent) return;

    if (TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
    if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ATOMPlayerController::OnPauseGame);
    }
    else
    {
        UE_LOG(LogTemp, Error,
            TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input "
                 "system. If you intend to use the legacy system, then you will need to update this C++ file."),
            *GetNameSafe(this));
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerController::OnPauseGame(const FInputActionValue& Value)
{
   if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

   GetWorld()->GetAuthGameMode()->SetPause(this);
}
//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerController::OnMatchStateChanged(ETOMGameState GameState)
{
    if (GameState == ETOMGameState::InProgress || GameState == ETOMGameState::Fight)
    {
       SetInputMode(FInputModeGameOnly());
       bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}
//-------------------------------------------------------------------------------------------------------------
