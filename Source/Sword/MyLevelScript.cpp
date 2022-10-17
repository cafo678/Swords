#include "MyLevelScript.h"
#include "UIWidget.h"
#include "TutorialWidget.h"
#include "EndMenuWidget.h"
#include "EnemySword.h"
#include "PawnSword.h"
#include "WinLoseWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"


void AMyLevelScript::BeginPlay()
{
    Super::BeginPlay();

    PlayerSword = Cast<APawnSword>(GetWorld()->GetFirstPlayerController()->GetPawn());
    
    UIWidget = CreateWidget<UUIWidget>(GetWorld(), UITemplate);
    UIWidget->AddToViewport();
    
    // Spawn the first enemy
    EnemySword = Cast<AEnemySword>(GetWorld()->SpawnActor<AEnemySword>(EnemiesBlueprint[EnemiesIndex], EnemyLocation, FRotator(0.f)));
}

void AMyLevelScript::Death(bool bIsPlayerDeath)
{
    // If the player win the last battle, restart the game
    if (EnemiesIndex == 6 && !bIsPlayerDeath)
    {
        EndMenuWidget = CreateWidget<UEndMenuWidget>(GetWorld(), EndMenuTemplate);
        EndMenuWidget->AddToViewport();
        FTimerHandle EndMenuHandle;
        float DelayToEndGame = 5.f;
        GetWorldTimerManager().SetTimer(EndMenuHandle, this, &AMyLevelScript::EndGame, DelayToEndGame, false);
        return;
    }
    
    GetWorld()->GetFirstPlayerController()->SetPause(true);
    
    WinLoseWidget = CreateWidget<UWinLoseWidget>(GetWorld(), WinLoseTemplate);
    WinLoseWidget->AddToViewport();

    if (!bIsPlayerDeath)
    {        
        EnemiesIndex++;
        WinLoseWidget->WinLoseText = FText::FromString("WIN");
        WinLoseWidget->QuestionText = FText::FromString("Do you want to go to next floor?");
    }
    else
    {
        WinLoseWidget->WinLoseText = FText::FromString("GAME OVER");
        WinLoseWidget->QuestionText = FText::FromString("Do you want to retry this floor?");
    }
}

void AMyLevelScript::SetNewLevel()
{
    // Destroy the old enemy and spawns the new one
    EnemySword->Destroy();
    EnemySword = Cast<AEnemySword>(GetWorld()->SpawnActor<AEnemySword>(EnemiesBlueprint[EnemiesIndex], EnemyLocation, FRotator(0.f)));
    PlayerSword->Balance = 1.f;
    WinLoseWidget->RemoveFromViewport();
    GetWorld()->GetFirstPlayerController()->SetPause(false);
    if(!bDoOnceShowDialogue) EnemySword->bIsInDialog = false;

    // If it's the last enemy show the dialogue
    if (EnemiesIndex == 6 && bDoOnceShowDialogue)
    {
        TutorialWidget = CreateWidget<UTutorialWidget>(GetWorld(), DialogueTemplate);
        TutorialWidget->AddToViewport();
        TutorialWidget->OnDialogueOver.AddUniqueDynamic(this, &AMyLevelScript::StartLastFight);
        PlayerSword->bCanAttack = false;
        bDoOnceShowDialogue = false;
    }
}

void AMyLevelScript::StartLastFight()
{
    UGameplayStatics::PlaySound2D(GetWorld(), BossFightMusic, 0.5f, 1.f, 7.f);
    EnemySword->bIsInDialog = false;
    PlayerSword->bCanAttack = true;
}

void AMyLevelScript::EndGame()
{
    Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())->ConsoleCommand("quit");
}
