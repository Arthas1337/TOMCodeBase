#include "UI/TOMPauseWidget.h"
#include "TealesOfMeadow/TealesOfMeadowGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/Button.h"

//-------------------------------------------------------------------------------------------------------------
void UTOMPauseWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

   if(ClearPauseButton)
   {
       ClearPauseButton->OnClicked.AddDynamic(this, &UTOMPauseWidget::OnClearPause);
   }

   if (ResetGameButton)
   {
       ResetGameButton->OnClicked.AddDynamic(this, &UTOMPauseWidget::OnResetGame);
   }

   if (QuitGameButton)
   {
       QuitGameButton->OnClicked.AddDynamic(this, &UTOMPauseWidget::OnQuitGame);
   }
}
//-------------------------------------------------------------------------------------------------------------
void UTOMPauseWidget::OnClearPause() 
{

   if(!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

   GetWorld()->GetAuthGameMode()->ClearPause();

}
//-------------------------------------------------------------------------------------------------------------
void UTOMPauseWidget::OnResetGame() 
{
    const auto CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
    UGameplayStatics::OpenLevel(this, CurrentLevelName);
}
//-------------------------------------------------------------------------------------------------------------
void UTOMPauseWidget::OnQuitGame() 
{
    if (!GetWorld())
    {
        UE_LOG(LogTemp, Error, TEXT("Error GetWorld()"));
        return;
    }
    const auto Controller = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
    if (!Controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Error Cast Controller"));
        return;
    }
    UKismetSystemLibrary::QuitGame(GetWorld(), Controller, EQuitPreference::Quit, false);
}
//-------------------------------------------------------------------------------------------------------------
