#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class APawnSword;
class AEnemySword;

UCLASS()
class SWORD_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()	
	
private:

// VARIABLES

	float EnemyXLocation;
	float RotationRate = 90.f;
	float XAmount;
	float YAmount;

	AEnemySword* EnemySword = nullptr;
	APawnSword* PawnSword = nullptr;
	UStaticMeshComponent* SwordMeshComponent = nullptr;

// METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	void SwordRight(float Amount);
	void SwordUp(float Amount);
	void SetDefenseState(bool bDefenseStateToSet);
	void InterruptAnimation();
	void DeathBlow();
	void SetDefenseRotation(float PitchMultiplier, float YawMultiplier);
	void SetAttackLocationAndRotation(int32 XValue, int32 YValue);
	
public:

// VARIABLES

	bool bCanAct = true;

// METHODS

	void SetEnemySword(AEnemySword* EnemySwordToSet);
};
