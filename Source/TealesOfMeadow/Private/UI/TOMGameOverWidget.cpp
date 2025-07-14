#include "UI/TOMGameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/BlueprintFunctionLibrary.h"

//-------------------------------------------------------------------------------------------------------------
void UTOMGameOverWidget::NativeOnInitialized()
{
   Super::NativeOnInitialized();

	if(RestartButton)
	{
        RestartButton->OnClicked.AddDynamic(this, &UTOMGameOverWidget::OnResettLevel);
	}

	if(QuitButton)
   {
        QuitButton->OnClicked.AddDynamic(this, &UTOMGameOverWidget::OnQuitGame);
   }
}
//-------------------------------------------------------------------------------------------------------------
void UTOMGameOverWidget::OnResettLevel()
{
   const auto CurrentLevelName = FName(*UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
	
}
//-------------------------------------------------------------------------------------------------------------
void UTOMGameOverWidget::OnQuitGame() 
{
	if(!GetWorld())
   {
        UE_LOG(LogTemp, Error, TEXT("Error GetWorld()"));
        return;
   }
   if(!GetOwningPlayerPawn()) return;
	const auto Controller = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
	if(!Controller)
   {
		UE_LOG(LogTemp, Error, TEXT("Error Cast Controller"));
      return;
   }
	UKismetSystemLibrary::QuitGame(GetWorld(), Controller, EQuitPreference::Quit, false);
}
//-------------------------------------------------------------------------------------------------------------
   