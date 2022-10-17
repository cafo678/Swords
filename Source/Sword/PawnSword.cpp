#include "PawnSword.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "MyPlayerController.h"
#include "MyLevelScript.h"
#include "EnemySword.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

APawnSword::APawnSword()
{
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
    CameraComponent->SetupAttachment(BodyCollisionComponent);
}

void APawnSword::BeginPlay()
{
    Super::BeginPlay();

    MyPlayerController = Cast<AMyPlayerController>(MyController);

    BodyCollisionComponent->OnComponentHit.AddDynamic(this, &APawnSword::OnBodyHit);
    SwordMeshComponent->OnComponentHit.AddDynamic(this, &APawnSword::OnSwordHit);
}

void APawnSword::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    // If the player dies display the retry UI
    if (FMath::IsNearlyEqual(Balance, 0.5f))
    {
        Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())->Death(true);
    }
    
    // Change camera based on the player state
    if (bIsInDefenseState)
    {
        CameraComponent->SetRelativeLocation(DefCameraLocation);
    }
    else
    {
        CameraComponent->SetRelativeLocation(AttCameraLocation);

        if (StoredDamage > 0.f) // If there is pending damage and the player switch stance, apply the damage and reset
        {
            Balance = FMath::Clamp(Balance -= StoredDamage, 0.5f, 1.5f);
            StoredDamage = 0;
            BlockedHits = 0;
        }
    }
}

// When the player take an hit
void APawnSword::OnBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BodySlashSE, GetActorLocation());
    EnemySword->StopAnimation();
    StopAnimation();

    // If the player take a deathblow
    if (EnemySword->bIsDoingDeathBlow)
    {
        EnemySword->bDoOnceDeathBlow = true;
        MyPlayerController->EnableInput(MyPlayerController);
        Balance = FMath::Clamp(Balance -= EnemySword->Damage * 6 + StoredDamage, 0.5f, 1.5f);
        Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())->bCanSwitchToDeathblowCamera = false;
    }
    else // If the player take a normal hit
    {
        if (EnemySword->bIsLastHitOfCombo) EnemySword->bIsPawnReadyToAct = true;
        Balance = FMath::Clamp(Balance -= EnemySword->Damage + StoredDamage, 0.5f, 1.5f);
    }

    StoredDamage = 0;
    BlockedHits = 0;
}

void APawnSword::OnSwordHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    EnemySword->StopAnimation();
    StopAnimation();

   if (OtherComp->IsA(UStaticMeshComponent::StaticClass()))
   {
       if (bIsInDefenseState) // If the player does a parry
       {
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordClashSE, OtherComp->GetComponentLocation());
            BlockedHits++;
            if (EnemySword->BlockedHits + EnemySword->HitsForDeathBlow > 0) EnemySword->BlockedHits--;
            StoredDamage += EnemySword->Damage / 2;

            if (BlockedHits == HitsToParry) // If the player has parried the whole combo
            {
                Balance = FMath::Clamp(Balance += StoredDamage, 0.5f, 1.5f);
                EnemySword->bIsPawnReadyToAct = true;
                StoredDamage = 0;
                BlockedHits = 0;
            }
            else if (EnemySword->bIsLastHitOfCombo) // If has parried the last hit but not the whole combo
            {                
                Balance = FMath::Clamp(Balance -= StoredDamage, 0.5f, 1.5f);
                EnemySword->bIsPawnReadyToAct = true;
                StoredDamage = 0;
                BlockedHits = 0;
            }
       }
       else                    // If the player receive a parry
       { 
            UGameplayStatics::PlaySoundAtLocation(GetWorld(), SwordClashSE, OtherComp->GetComponentLocation());
            EnemySword->Balance = FMath::Clamp(EnemySword->Balance -= Damage / 2, 0.5f, 1.5f);
            EnemySword->BlockedHits++;
       }
   }
   else                         // If the enemy receive an hit
   {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), BodySlashSE, EnemySword->GetActorLocation());
        EnemySword->Balance = FMath::Clamp(EnemySword->Balance -= Damage, 0.5f, 1.5f); 
        
        if (bIsDoingDeathBlow) // If the enemy receive a deathblow (dies)
        {
            if (GetWorld()->GetName() == FString("ArenaLevel"))
            {
                Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())->Death(false);
            }                     
            
            bIsDoingDeathBlow = false;  
            MyPlayerController->EnableInput(MyPlayerController);
            if ( Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())) Cast<AMyLevelScript>(GetWorld()->GetLevelScriptActor())->bCanSwitchToDeathblowCamera = false; 
        }
   }
}

void APawnSword::SetEnemySword(AEnemySword* EnemySwordToSet)
{
    EnemySword = EnemySwordToSet;
}

