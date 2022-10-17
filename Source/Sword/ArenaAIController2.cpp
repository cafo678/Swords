#include "ArenaAIController2.h"
#include "PawnSword.h"
#include "EnemySword.h"

void AArenaAIController2::BeginPlay()
{
    Super::BeginPlay();
}

void AArenaAIController2::Tick(float DeltaSeconds)
{    
    Super::Tick(DeltaSeconds);

    if (PossessedSword->bIsInDialog) return;

    // When the enemy is losing, increment his hits number
    if (PossessedSword->Balance < 0.75)
    {
        if (bDoOnceIncerementHits)
        {
            PossessedSword->MinHits += 2;
            PossessedSword->MaxHits += 2;
            bDoOnceIncerementHits = false;
        }
    }
    
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

// Different combo for the last enemy, first spawn all emitters, than attack sequentially
void AArenaAIController2::SetupCombo()
{
    HitDelaysArray.Empty();
    HitLocationArray.Empty();
    float TotalHitDelay = 0.f;
    PossessedSword->bIsInDefenseState = false;
    PossessedSword->bIsPawnReadyToAct = false;
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    int32 HitsToDo = FMath::RandRange(PossessedSword->MinHits, PossessedSword->MaxHits);
    PlayerSword->HitsToParry = HitsToDo;


    // For every hit of the combo get a random direction and call the emitter
    for (int32 i = 0; i < HitsToDo; i++)
    {        
        float HitDelay = FMath::RandRange(PossessedSword->MinDelayForHit, PossessedSword->MaxDelayForHit);
        TotalHitDelay += HitDelay;
        HitDelaysArray.Add(HitDelay);

        int32 XValue;
        int32 YValue;
        
        do
        {
            XValue = FMath::RandRange(-1, 1);
            YValue = FMath::RandRange(-1, 1);
        } 
        while (XValue == 0 && YValue == 0);

        FVector AttackLocation = PossessedSword->GetLocationForAttack(XValue, YValue);
        HitLocationArray.Add(AttackLocation);
        
        FTimerHandle EmitterHandle;
        FTimerDelegate EmitterDelegate;
        EmitterDelegate.BindUFunction(this, FName("SpawnEmitter"), AttackLocation);
        GetWorldTimerManager().SetTimer(EmitterHandle, EmitterDelegate, TotalHitDelay, false);   
    }

    TotalHitDelay += PossessedSword->EmitterAnticipationTime;

    // For every hit of the combo attack
    for (int32 i = 0; i < HitsToDo; i++)
    {
        bool bIsLastComboHit;

        if (i == HitsToDo - 1)
        {
            bIsLastComboHit = true;
        }
        else
        {
            bIsLastComboHit = false;
        }
        
        TotalHitDelay += HitDelaysArray[i];

        FTimerHandle HitDelayHandle;
        FTimerDelegate HitDelayDelegate;
        HitDelayDelegate.BindUFunction(this, FName("Attack"), HitLocationArray[i], bIsLastComboHit);
        GetWorldTimerManager().SetTimer(HitDelayHandle, HitDelayDelegate, TotalHitDelay, false);
    }
}

