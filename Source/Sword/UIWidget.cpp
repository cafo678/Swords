#include "UIWidget.h"
#include "PawnSword.h"
#include "EnemySword.h"


void UUIWidget::NativeConstruct()
{
    Super::NativeConstruct();

    PlayerSword = Cast<APawnSword>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void UUIWidget::SetEnemySword(AEnemySword* EnemySwordToSet)
{
    EnemySword = EnemySwordToSet;
}


