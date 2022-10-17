#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

class AEnemySword;
class APawnSword;
class UCurveFloat;

UCLASS()
class SWORD_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:

//VARIABLES
	
	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	UParticleSystem* LightParticle = nullptr;

	FRotator RotationTowardsPlayer;

	AEnemySword* PossessedSword = nullptr;
	APawnSword* PlayerSword = nullptr;

	UStaticMeshComponent* PossessedSwordMeshComponent = nullptr;
	UStaticMeshComponent* PlayerSwordMeshComponent = nullptr;

//METHODS

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void SpawnEmitter(FVector SpawnLocation);	// Spawns the visual clue attack

	UFUNCTION()
	void Attack(FVector AttackLocation, bool bIsLastComboHit);

	virtual void BeginPlay() override;

	void TurnVersusPlayer();
	void DelayToAttack();	// Wait to start combo
	virtual void SetupCombo();		
	void DeathBlow();

public:

//METHODS

	void SetPossessedSword(AEnemySword* PossessedSwordToSet);
};
