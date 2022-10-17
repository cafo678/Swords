#pragma once

#include "CoreMinimal.h"
#include "GeneralSword.h"
#include "EnemySword.generated.h"

class APawnSword;
class AMyAIController;
class AMyPlayerController;
class UUIWidget;

UCLASS()
class SWORD_API AEnemySword : public AGeneralSword
{
	GENERATED_BODY()

private:

// VARIABLES
	
	APawnSword* PlayerSword = nullptr;
	AMyPlayerController* PlayerController = nullptr;
	AMyAIController* MyAIController = nullptr;
	UUIWidget* Widget = nullptr; // Widget that displays balance, name and state

// METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:

// VARIABLES

	int32 FallibilityPercent; // Change that a defense fails
	int32 BlockedHits; // How many hits this sword has blocked
	bool bIsLastHitOfCombo = false; // If this hit is the last of the combo
	bool bIsPawnReadyToAct = true; // If the sword can start a new combo
	bool bDoOnceDeathBlow = true;	// bDoOnce used in Deathblow() in the controller
	bool bIsInDialog = true; // Used to start the AIController acts in the last fight after the dialogue

	UPROPERTY(EditAnywhere, Category = "Custom")
	int32 MinHits = 1;								// Min hits of a combo

	UPROPERTY(EditAnywhere, Category = "Custom")
	int32 MaxHits = 3;								// Max hits of a combo

	UPROPERTY(EditAnywhere, Category = "Custom")
	float MinDelayToStartCombo = 0.5f;				// In seconds

	UPROPERTY(EditAnywhere, Category = "Custom")
	float MaxDelayToStartCombo = 3.f;				// In seconds

	UPROPERTY(EditAnywhere, Category = "Custom")
	float MinDelayForHit = 0.5f;					// In seconds

	UPROPERTY(EditAnywhere, Category = "Custom")
	float MaxDelayForHit = 3.f;						// In seconds

	UPROPERTY(EditAnywhere, Category = "Custom")
	float EmitterAnticipationTime = 0.5f;			// In seconds, time in which the visual clue is spawned before an attack

	UPROPERTY(EditAnywhere, Category = "Custom")
	int32 DefaultFallibilityPercent;				

	UPROPERTY(EditAnywhere, Category = "Custom")
	FText Name;										

	UPROPERTY(EditAnywhere, Category = "Custom")
	int32 HitsForDeathBlow;							// After how many hits the sword reply with a deathblow

// METHODS

	UFUNCTION(BlueprintPure, Category = "Custom")
	FText GetName();

	UFUNCTION(BlueprintPure, Category = "Custom")
	int32 GetDeathBlowCountdown();
};