#include "TutorialScriptActor.h"
#include "PawnSword.h"
#include "EnemySword.h"
#include "TutorialWidget.h"
#include "UIWidget.h"
#include "Kismet/GameplayStatics.h"

void ATutorialScriptActor::BeginPlay()
{
    Super::BeginPlay();

    PlayerSword = Cast<APawnSword>(GetWorld()->GetFirstPlayerController()->GetPawn());

    UIWidget = CreateWidget<UUIWidget>(GetWorld(), UITemplate);
    UIWidget->AddToViewport();
    UIWidget->SetVisibility(ESlateVisibility::Hidden);

    NixSword = Cast<AEnemySword>(GetWorld()->SpawnActor<AEnemySword>(EnemyBlueprint, EnemyLocation, FRotator(0.f)));

    TutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), DialogueTemplate);
    TutorialWidget->AddToViewport();
}

void ATutorialScriptActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (TutorialWidget->Index == 3) // Displays the widget only when it reach the appropriate tutorial dialogue 
    {
        UIWidget->SetVisibility(ESlateVisibility::Visible);
    }
}