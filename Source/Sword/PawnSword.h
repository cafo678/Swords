#pragma once

#include "CoreMinimal.h"
#include "GeneralSword.h"
#include "PawnSword.generated.h"

class UBoxComponent;
class AEnemySword;
class AMyPlayerController;

UCLASS()
class SWORD_API APawnSword : public AGeneralSword
{
	GENERATED_BODY()

private:

// VARIABLES
	
	float StoredDamage; // Damage stored, to inflict if the player fails a parry during the enemy combo
	int32 BlockedHits;	// Number of hits blocked by the player
	
	AMyPlayerController* MyPlayerController = nullptr;

	AEnemySword* EnemySword = nullptr;

// METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void OnBodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnSwordHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:

// VARIABLES

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	FVector DefCameraLocation = FVector(-300.f, 0.f, 0.f);	// Camera location in defense state

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	FVector AttCameraLocation = FVector(100.f, 0.f, 0.f);	// Camera location in attack state

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	USoundBase* SwordClashSE = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	USoundBase* BodySlashSE = nullptr;
	
// METHODS

	APawnSword();

public:

// VARIABLES

	int32 HitsToParry; // Number of the hits of the enemy combo
	bool bCanAttack = true;

// METHODS

	void SetEnemySword(AEnemySword* EnemySwordToSet);
};
