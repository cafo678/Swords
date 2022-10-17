#include "ArenaAIController.h"
#include "PawnSword.h"
#include "EnemySword.h"

void AArenaAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AArenaAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (PossessedSword->BlockedHits == PossessedSword->HitsForDeathBlow)
    {
        PossessedSword->bIsInDefenseState = false;
        DeathBlow();
    }
    
    // if the player is in attack state, go in defense state and reset behaviour
    if (!PlayerSword->bIsInDefenseState)
    {
        PossessedSword->bIsInDefenseState = true;
        PossessedSword->bIsPawnReadyToAct = true;
        GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    }

    // if has no more balance wait for deathblow
    if (FMath::IsNearlyEqual(PossessedSword->Balance, 0.5f))
    {
        PossessedSword->bIsInDefenseState = true;
        PossessedSword->bIsPawnReadyToAct = false;
        return;
    }  
    
    // if the player attacks, start defense animation
    if (FMath::IsNearlyEqual(PlayerSwordMeshComponent->GetRelativeLocation().X, PossessedSword->GetActorLocation().X) && !PlayerSword->bIsInDefenseState)
    {
        TurnVersusPlayer();
        PossessedSword->SetAnimationValues(false);
    }
    
    // if the player defend, wait and start combo
    if (PossessedSword->bIsPawnReadyToAct && PlayerSword->bIsInDefenseState)
    {  
        DelayToAttack();
    }  
}



