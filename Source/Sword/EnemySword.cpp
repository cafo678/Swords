#include "EnemySword.h"
#include "PawnSword.h"
#include "MyAIController.h"
#include "MyPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UIWidget.h"


void AEnemySword::BeginPlay()
{
    Super::BeginPlay();

    BlockedHits = 0;
    
    bIsInDefenseState = true;

    PlayerSword = Cast<APawnSword>(GetWorld()->GetFirstPlayerController()->GetPawn());
    PlayerSword->SetEnemySword(this);

    // Necessary because when spawned, actors BeginPlay() activates after his controller's BeginPlay()
    MyAIController = Cast<AMyAIController>(MyController);
    MyAIController->SetPossessedSword(this);

    PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
    PlayerController->SetEnemySword(this);

    // Update Widget
    TArray<UUserWidget*> WidgetArray;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), WidgetArray, UUIWidget::StaticClass());
    Widget = Cast<UUIWidget>(WidgetArray[0]);     
    Widget->SetEnemySword(this);
}

void AEnemySword::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Change fallibility based on the balance

    if (Balance == 1.f)
    {
        FallibilityPercent = DefaultFallibilityPercent;
    }
    
    if (Balance > 1.f)
    {
        float FallibilityOffset = (Balance - 1.f) * 2.f;
        FallibilityPercent = DefaultFallibilityPercent * (Balance - FallibilityOffset);
    }

    if (Balance < 1.f)
    {
        float FallibilityOffset = (1.f - Balance) * 2.f;
        FallibilityPercent = DefaultFallibilityPercent * (Balance + FallibilityOffset);
    }   
}

FText AEnemySword::GetName()
{
    return Name;
}

int32 AEnemySword::GetDeathBlowCountdown()
{
    return HitsForDeathBlow - BlockedHits;
}