#include "UI/TOMPlayerHUD.h"
#include "Blueprint/UserWidget.h"
#include "TealesOfMeadow/TealesOfMeadowGameMode.h"

//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerHUD::BeginPlay()
{
   Super::BeginPlay();

    if (!GetWorld()) return;

    GameWidgets.Add(ETOMGameState::InProgress ,CreateWidget<UUserWidget>(GetWorld(), PlayerWidgetClass));
    GameWidgets.Add(ETOMGameState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ETOMGameState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for(auto& WidgetPair : GameWidgets)
    {
        const auto Widget = WidgetPair.Value;
        if(!Widget) continue;

        Widget->AddToViewport();
        Widget->SetVisibility(ESlateVisibility::Hidden);
    }

    const auto GameMode = Cast<ATealesOfMeadowGameMode>(GetWorld()->GetAuthGameMode());
    if(GameMode)
    {
        GameMode->OnGameChangeState.AddDynamic(this, &ATOMPlayerHUD::OnMatchStateChanged);
    }
}
//-------------------------------------------------------------------------------------------------------------
void ATOMPlayerHUD::OnMatchStateChanged(ETOMGameState State) 
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

   if(GameWidgets.Contains(State))
   {
      CurrentWidget = GameWidgets[State];
   }

   if(CurrentWidget)
   {
       CurrentWidget->SetVisibility(ESlateVisibility::Visible);
   }
}
//-------------------------------------------------------------------------------------------------------------
