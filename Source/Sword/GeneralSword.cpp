#include "GeneralSword.h"
#include "Components/BoxComponent.h"
#include "PawnSword.h"
#include "MyLevelScript.h"
#include "EnemySword.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyPlayerController.h"


AGeneralSword::AGeneralSword()
{
    BodyCollisionComponent = CreateDefaultSubobject<UBoxComponent>(FName("BodyCollisionComponent"));
	SetRootComponent(BodyCollisionComponent);

	SwordMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMeshComponent"));
	SwordMeshComponent->SetupAttachment(BodyCollisionComponent); 
}

void AGeneralSword::BeginPlay()
{
	Super::BeginPlay();

	MyController = GetController();

	DefaultSwordRelativeLocation = SwordMeshComponent->GetRelativeLocation();
	DefaultSwordRelativeRotation = SwordMeshComponent->GetRelativeRotation();
}

void AGeneralSword::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

    if (Balance > 0.55f) Balance += BalanceRecoveryForFrame; // If not the enemy will never die

	if (bCanPlayAnimation)
	{
        PlayAnimation(DeltaSeconds);
    }

    // Update Animation Time based on Balance
    if (Balance > 1.f)
    {
        float BalanceOffset = (Balance - 1.f) / 2.f;
        DefenseAnimationTime = DefaultDefenseAnimationTime * (Balance - BalanceOffset);
        AttackAnimationTime = DefaultAttackAnimationTime * (Balance - BalanceOffset);
    }
    else if (Balance < 1.f)
    {
        float BalanceOffset = (1.f - Balance) / 2.f;
        DefenseAnimationTime = DefaultDefenseAnimationTime * (Balance + BalanceOffset);
        AttackAnimationTime = DefaultAttackAnimationTime * (Balance + BalanceOffset);
    }
    else
    {
        DefenseAnimationTime = DefaultDefenseAnimationTime;
        AttackAnimationTime = DefaultAttackAnimationTime;
    }
    
}

void AGeneralSword::PlayAnimation(float DeltaSeconds)
{
    CurrentAnimTime += DeltaSeconds;
    Perc = FMath::Clamp(CurrentAnimTime / AnimationTime, 0.f, 1.f);
    Perc = Curve->GetFloatValue(Perc);
    SwordMeshComponent->SetRelativeLocation(FMath::Lerp(StartLocation, LocationToReach, Perc), true); 
}

void AGeneralSword::SetAnimationValues(bool bIsDeathBlow)
{
    if (bDoOnceSetAnimationValues)
    {
        StartLocation = SwordMeshComponent->GetRelativeLocation();   
        
        if (bIsDeathBlow) // If is a deathblow
        {
            AnimationTime = DeathblowAnimationTime;
            LocationToReach = FVector( (Cast<AEnemySword>(this)) ? -500.f : 500.f, 0.f, 0.f);
        }
        else
        {
            if (bIsInDefenseState) // If is a defense
            {
                AnimationTime = DefenseAnimationTime;
                LocationToReach = SwordMeshComponent->GetForwardVector() * MovementDefRate;
            }
            else                    // If is an attack
            {
                AnimationTime = AttackAnimationTime;
                LocationToReach = SwordMeshComponent->GetForwardVector() * MovementAttRate + SwordMeshComponent->GetRelativeLocation();
            }
        }

        if (Cast<AEnemySword>(this) && bIsInDefenseState) // If it is an enemy defense check fallibility
        {
            bool bWillFailDefense = FMath::RandRange((int32) 0, (int32) 100) < Cast<AEnemySword>(this)->FallibilityPercent;

            if (!bWillFailDefense)
            {
                bCanPlayAnimation = true;
            }
        }
        else
        {
            bCanPlayAnimation = true;
        }   

        bDoOnceSetAnimationValues = false; 
    }
}

void AGeneralSword::StopAnimation()
{
    CurrentAnimTime = 0.f;
    bCanPlayAnimation = false;
    bDoOnceSetAnimationValues = true;
    SwordMeshComponent->SetRelativeLocationAndRotation(DefaultSwordRelativeLocation, DefaultSwordRelativeRotation);
}

void AGeneralSword::SetDeathBlow()
{
    bIsDoingDeathBlow = true;
    
    float RotationSwordToSet = (Cast<AEnemySword>(this)) ? 90.f : -90.f;
    SwordMeshComponent->SetRelativeLocationAndRotation(DefaultSwordRelativeLocation, FRotator(RotationSwordToSet, 0.f, 0.f));
    
    if (Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())) Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())->bCanSwitchToDeathblowCamera = true;
    Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController())->DisableInput(Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController()));
    
    SetAnimationValues(true);
}

float AGeneralSword::GetBalance()
{
    return UKismetMathLibrary::NormalizeToRange(Balance, 0.5f, 1.5f);
}

FText AGeneralSword::GetState()
{
    if (bIsInDefenseState) return FText::FromString("Defense");
    else return FText::FromString("Attack");
}

// XValue and YValue acts like points on horizontal and vertical axes
FVector AGeneralSword::GetLocationForAttack(int32 XValue, int32 YValue)
{
	float XLocation;
    FVector SwordLocation;

    if (Cast<AEnemySword>(this))
    {
        XLocation = -500.f;
    }
    else
    {
        XLocation = 500.f;
    } 

    switch (XValue)
    {
        case -1:    // X = -1

            switch (YValue)
            {
                case -1:    // X = -1, Y = -1: down-left direction
                    SwordLocation = FVector(XLocation, -MovementAttRate * 0.9, -MovementAttRate * 0.9);
                    break;

                case 0:
                    SwordLocation = FVector(XLocation, -MovementAttRate, 0.f);
                    break;

                case 1:
                    SwordLocation = FVector(XLocation, -MovementAttRate * 0.9, MovementAttRate * 0.9);
                    break;
            }
            break;
        
        case 0:

            switch (YValue)
            {
                case -1:
                    SwordLocation = FVector(XLocation, 0.f, -MovementAttRate);
                    break;

                case 1:
                    SwordLocation = FVector(XLocation, 0.f, MovementAttRate);
                    break;
            }
            break;
        
        case 1:

            switch (YValue)
            {
                case -1:
                    SwordLocation = FVector(XLocation, MovementAttRate * 0.9, -MovementAttRate * 0.9);
                    break;

                case 0:
                    SwordLocation = FVector(XLocation, MovementAttRate, 0.f);
                    break;

                case 1:
                    SwordLocation = FVector(XLocation, MovementAttRate * 0.9, MovementAttRate * 0.9);
                    break;
            }
            break;
    }

    return SwordLocation; 
}

