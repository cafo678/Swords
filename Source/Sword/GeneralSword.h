#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GeneralSword.generated.h"

class UBoxComponent;
class UCameraComponent;

UCLASS()
class SWORD_API AGeneralSword : public APawn
{
	GENERATED_BODY()

private:

//VARIABLES

	float CurrentAnimTime;		// Current time in the animation
	float AnimationTime; 		// Animation time in secs, this var changes based on the specific animation time of the desired attack
	float Perc; 				// % of the animation
	float AttackAnimationTime;	// Attack animation time of the sword
	float DefenseAnimationTime;	// Defense animation time of the sword
	float DeathblowAnimationTime = 1.f;	// Animation time of the Deathblow
	float MovementDefRate = 55.f;	// Movement offset in defense
	float MovementAttRate = 85.f;	// Movement offset in attack

	bool bDoOnceSetAnimationValues = true;	// bDoOnce used in SetAnimationValues()
	bool bCanPlayAnimation = false;	// True if the animation can start

	FVector LocationToReach;				// Location to reach at the end of the animation
	FVector StartLocation;					// Location in which the animation starts

//METHODS

	void PlayAnimation(float DeltaSeconds); // Play animation

protected:

//VARIABLES
	
	AController* MyController = nullptr;	// Controller of the sword

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BodyCollisionComponent = nullptr;	// Body Collision Component of the sword

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	UCurveFloat* Curve = nullptr;	// Curve of the animation

//METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:

	AGeneralSword();

//VARIABLES	

	float Balance = 1.f;	// Balance of the sword, value between 0.5 and 1.5
	
	bool bIsInDefenseState = true;	// True if the sword is in defense state
	bool bIsDoingDeathBlow = false;	// True if the sword is doing a deathblow 
	
	FVector DefaultSwordRelativeLocation;	// Default relative location of the sword
	FRotator DefaultSwordRelativeRotation;	// Deafault relative rotation of the sword
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SwordMeshComponent = nullptr;	// Sword mesh component

	UPROPERTY(EditAnywhere, Category = "Custom")
	float DefaultAttackAnimationTime;	// Default Animation time of attack in secs

	UPROPERTY(EditAnywhere, Category = "Custom")
	float DefaultDefenseAnimationTime;	// Default Animation time of defense in secs
	
	UPROPERTY(EditAnywhere, Category = "Custom")
	float Damage = 0.1f;	// Damage of the sword

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	float BalanceRecoveryForFrame; 	// Balance recovery every frame

//METHODS

	void StopAnimation(); // Stop the running animation
	void SetAnimationValues(bool bIsDeathBlow);	// Sets the animation values
	void SetDeathBlow();	// Sets camera and variables for deathblow

	FVector GetLocationForAttack(int32 XValue, int32 YValue);	// Get Location where to start attack

	UFUNCTION(BlueprintPure, Category = "Custom")
	float GetBalance();	// Get Balance used for UI

	UFUNCTION(BlueprintPure, Category = "Custom")
	FText GetState();	// Get State (attack or defense) used for UI
};
