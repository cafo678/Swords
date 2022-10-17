#include "MyPlayerController.h"
#include "EnemySword.h"
#include "Kismet/GameplayStatics.h"
#include "PawnSword.h"
#include "Kismet/KismetMathLibrary.h"


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();	

    if (Cast<APawnSword>(GetPawn()))
    {
        PawnSword = Cast<APawnSword>(GetPawn());
        SwordMeshComponent = PawnSword->FindComponentByClass<UStaticMeshComponent>();
    } 
}

void AMyPlayerController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);    
    
    // Keep a direction pressed will not work
    if (FMath::IsNearlyEqual(XAmount, 0.f) && FMath::IsNearlyEqual(YAmount, 0.f))
    {
        bCanAct =  true;
    }

    // Check input of the player and call the method to set attack or defense locations and rotations
    if (bCanAct)
    {   
        if (FMath::IsNearlyEqual(XAmount, 0.f, 0.2f))
        { 
            if (FMath::IsNearlyEqual(YAmount, 1.f, 0.2f)) // Attack or defend upwards
            {
                if (PawnSword->bIsInDefenseState) SetDefenseRotation(1.f, -1.f);
                else SetAttackLocationAndRotation(0, -1);   
            }
        
            if (FMath::IsNearlyEqual(YAmount, -1.f, 0.2f)) // Attack or defend downwards
            {
                if (PawnSword->bIsInDefenseState) SetDefenseRotation(-1.f, -1.f);
                else SetAttackLocationAndRotation(0, 1);
            }     
        }

        if (FMath::IsNearlyEqual(YAmount, 0.f, 0.2f))
        {
            if (FMath::IsNearlyEqual(XAmount, 1.f, 0.2f)) // Attack or defend from right
            {
                if (PawnSword->bIsInDefenseState) SetDefenseRotation(0.f, 1.f); 
                else SetAttackLocationAndRotation(-1, 0); 
            }
            
            if (FMath::IsNearlyEqual(XAmount, -1.f, 0.2f)) // Attack or defend from left
            {
                if (PawnSword->bIsInDefenseState) SetDefenseRotation(0.f, -1.f); 
                else SetAttackLocationAndRotation(1, 0);
            }     
        }

        // Attack or defend from diagonals directions

        if (FMath::IsNearlyEqual(XAmount, 0.7f, 0.2f) && FMath::IsNearlyEqual(YAmount, 0.7f, 0.2f))
        {
            if (PawnSword->bIsInDefenseState) SetDefenseRotation(0.5f, 1.f);            
            else SetAttackLocationAndRotation(-1, -1);       
        }    

        if (FMath::IsNearlyEqual(XAmount, 0.7f, 0.2f) && FMath::IsNearlyEqual(YAmount, -0.7f, 0.2f))
        {
            if (PawnSword->bIsInDefenseState) SetDefenseRotation(-0.5f, 1.f);            
            else SetAttackLocationAndRotation(-1, 1);       
        }

        if (FMath::IsNearlyEqual(XAmount, -0.7f, 0.2f) && FMath::IsNearlyEqual(YAmount, 0.7f, 0.2f))
        {
            if (PawnSword->bIsInDefenseState) SetDefenseRotation(0.5f, -1.f);   
            else SetAttackLocationAndRotation(1, -1);       
        }

        if (FMath::IsNearlyEqual(XAmount, -0.7f, 0.2f) && FMath::IsNearlyEqual(YAmount, -0.7f, 0.2f))
        {
            if (PawnSword->bIsInDefenseState) SetDefenseRotation(-0.5f, -1.f); 
            else SetAttackLocationAndRotation(1, 1);      
        }
    }
}

void AMyPlayerController::SetAttackLocationAndRotation(int32 XValue, int32 YValue)
{    
    bCanAct =  false;
    FVector LocationToSet = PawnSword->GetLocationForAttack(XValue, YValue);
    SwordMeshComponent->SetRelativeLocation(LocationToSet);
    FRotator RotationTowardsEnemy = UKismetMathLibrary::FindLookAtRotation(SwordMeshComponent->GetRelativeLocation(), EnemySword->GetActorLocation());
    SwordMeshComponent->SetRelativeRotation(RotationTowardsEnemy);
    
    PawnSword->SetAnimationValues(false);
}

void AMyPlayerController::SetDefenseRotation(float PitchMultiplier, float YawMultiplier)
{
    SwordMeshComponent->SetRelativeLocationAndRotation(PawnSword->DefaultSwordRelativeLocation, PawnSword->DefaultSwordRelativeRotation);
    FRotator RotatorToSet = FRotator(RotationRate * PitchMultiplier, RotationRate * YawMultiplier, 0.f);
    SwordMeshComponent->SetRelativeRotation(RotatorToSet);

    PawnSword->SetAnimationValues(false);
}

void AMyPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAxis("SwordRight", this, &AMyPlayerController::SwordRight);
    InputComponent->BindAxis("SwordUp", this, &AMyPlayerController::SwordUp);

    DECLARE_DELEGATE_OneParam(FCustomInputDelegate, bool);

    InputComponent->BindAction<FCustomInputDelegate>("SetDefenseState", IE_Pressed, this, &AMyPlayerController::SetDefenseState, false);
    InputComponent->BindAction<FCustomInputDelegate>("SetDefenseState", IE_Released, this, &AMyPlayerController::SetDefenseState, true);

    InputComponent->BindAction("Interrupt", IE_Pressed, this, &AMyPlayerController::InterruptAnimation);
    InputComponent->BindAction("DeathBlow", IE_Pressed, this, &AMyPlayerController::DeathBlow);
}

void AMyPlayerController::InterruptAnimation()
{
    PawnSword->StopAnimation();
}

void AMyPlayerController::DeathBlow()
{
    if (!EnemySword) return;
    
    if (FMath::IsNearlyEqual(EnemySword->Balance, 0.5f) && EnemySword->bIsInDefenseState)
    {
        PawnSword->SetDeathBlow();
        PawnSword->bIsInDefenseState = true;
    }   
}

void AMyPlayerController::SwordRight(float Amount)
{
    XAmount = Amount;
}

void AMyPlayerController::SwordUp(float Amount)
{
    YAmount = Amount;
}

void AMyPlayerController::SetDefenseState(bool bDefenseStateToSet)
{
    if (PawnSword->bCanAttack) 
    {
        PawnSword->bIsInDefenseState = bDefenseStateToSet;
        PawnSword->StopAnimation();
    }
}

void AMyPlayerController::SetEnemySword(AEnemySword* EnemySwordToSet)
{
    EnemySword = EnemySwordToSet;
    EnemyXLocation = EnemySword->GetActorLocation().X;
}
