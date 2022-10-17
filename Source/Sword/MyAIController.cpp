#include "MyAIController.h"
#include "PawnSword.h"
#include "EnemySword.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


void AMyAIController::BeginPlay()
{
    Super::BeginPlay();

    PlayerSword = Cast<APawnSword>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PlayerSword) PlayerSwordMeshComponent = PlayerSword -> SwordMeshComponent;
}

void AMyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void AMyAIController::TurnVersusPlayer()
{
    if (PossessedSword->bIsInDefenseState)
    {
        RotationTowardsPlayer = UKismetMathLibrary::FindLookAtRotation(PossessedSword->GetActorLocation(), PlayerSwordMeshComponent->GetComponentLocation());
    }
    else
    {
        RotationTowardsPlayer = UKismetMathLibrary::FindLookAtRotation(PossessedSwordMeshComponent->GetComponentLocation(), PlayerSword->GetActorLocation());
    }
    
    PossessedSwordMeshComponent->SetRelativeRotation(RotationTowardsPlayer);
}

// Wait and start combo
void AMyAIController::DelayToAttack()
{
    FTimerHandle AttackDelayHandle;
    float DelayToAttack = FMath::RandRange(PossessedSword->MinDelayToStartCombo, PossessedSword->MaxDelayToStartCombo);

    GetWorldTimerManager().SetTimer(AttackDelayHandle, this, &AMyAIController::SetupCombo, DelayToAttack, false);
}

void AMyAIController::SetupCombo()
{
    float TotalHitDelay = 0.f;
    PossessedSword->bIsInDefenseState = false;
    PossessedSword->bIsPawnReadyToAct = false;
    GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
    int32 HitsToDo = FMath::RandRange(PossessedSword->MinHits, PossessedSword->MaxHits);
    PlayerSword->HitsToParry = HitsToDo;


    // For every hit of the combo get a random direction and call the timers
    for (int32 i = 0; i < HitsToDo; i++)
    {        
        float HitDelay = FMath::RandRange(PossessedSword->MinDelayForHit, PossessedSword->MaxDelayForHit);
        TotalHitDelay += HitDelay;

        int32 XValue;
        int32 YValue;
        
        do
        {
            XValue = FMath::RandRange(-1, 1);
            YValue = FMath::RandRange(-1, 1);
        } 
        while (XValue == 0 && YValue == 0);

        FVector AttackLocation = PossessedSword->GetLocationForAttack(XValue, YValue);
        bool bIsLastComboHit;

        if (i == HitsToDo - 1)
        {
            bIsLastComboHit = true;
        }
        else
        {
            bIsLastComboHit = false;
        }
        
        FTimerHandle EmitterHandle;
        FTimerHandle HitDelayHandle;
        FTimerDelegate EmitterDelegate;
        FTimerDelegate HitDelayDelegate;
        EmitterDelegate.BindUFunction(this, FName("SpawnEmitter"), AttackLocation);
        HitDelayDelegate.BindUFunction(this, FName("Attack"), AttackLocation, bIsLastComboHit);
        GetWorldTimerManager().SetTimer(EmitterHandle, EmitterDelegate, TotalHitDelay - PossessedSword->EmitterAnticipationTime, false);
        GetWorldTimerManager().SetTimer(HitDelayHandle, HitDelayDelegate, TotalHitDelay, false);
    }
}

void AMyAIController::SpawnEmitter(FVector SpawnLocation)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LightParticle, SpawnLocation + FVector(PossessedSword->GetActorLocation().X, 0.f, 0.f));
}

void AMyAIController::Attack(FVector AttackLocation, bool bIsLastComboHit)
{    
    PossessedSword->bIsLastHitOfCombo = bIsLastComboHit;
    PossessedSwordMeshComponent->SetRelativeLocation(AttackLocation);
    TurnVersusPlayer();
    PossessedSword->SetAnimationValues(false);
}

void AMyAIController::SetPossessedSword(AEnemySword* PossessedSwordToSet)
{
    PossessedSword = PossessedSwordToSet;
    if (PossessedSword) PossessedSwordMeshComponent = PossessedSword -> SwordMeshComponent;
}

void AMyAIController::DeathBlow()
{
    if (PossessedSword->bDoOnceDeathBlow)
    {
        PossessedSword->SetDeathBlow();
        PossessedSword->BlockedHits = 0;
        PlayerSword->bIsInDefenseState = true;
        PossessedSword->bDoOnceDeathBlow = false;
    }
}