#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MyLevelScript.generated.h"

class UUIWidget;
class AEnemySword;
class APawnSword;
class UWinLoseWidget;
class UTutorialWidget;
class USoundBase;
class UEndMenuWidget;

UCLASS()
class SWORD_API AMyLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
private:

// VARIABLES

	int32 EnemiesIndex = 0;
	bool bDoOnceShowDialogue = true; // Show the dialogue of the last fight only one time
	FVector EnemyLocation = FVector(500.f, 0.f, 0.f);

	APawnSword* PlayerSword = nullptr;
	AEnemySword* EnemySword = nullptr;
	UUIWidget* UIWidget = nullptr; // Widget that displays balance, name and state
	UEndMenuWidget* EndMenuWidget = nullptr; 
	

// METHODS

	virtual void BeginPlay() override;
	void EndGame();

protected:

//VARIABLES:

 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> UITemplate;	// Widget to set in the editor

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UWinLoseWidget> WinLoseTemplate;	// Widget to set in the editor

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> DialogueTemplate;	// Widget to set in the editor

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> EndMenuTemplate;	// Widget to set in the editor

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<TSubclassOf<AEnemySword>> EnemiesBlueprint;	// Array of the enemies, to set in the editor

	UPROPERTY(BlueprintReadOnly)
	UWinLoseWidget* WinLoseWidget = nullptr;	// Widget that prompt the user every level or when he loses

	UPROPERTY(BlueprintReadOnly)
	UTutorialWidget* TutorialWidget = nullptr;	// Widget with tutorial dialogues

	UPROPERTY(EditDefaultsOnly, Category = "Custom")
	USoundBase* BossFightMusic = nullptr;

// METHODS

	UFUNCTION (BlueprintCallable, Category = "Custom")
	void SetNewLevel();	// Set the next fight

	UFUNCTION()
	void StartLastFight(); // Method that start the IA of the last enemy after the event in the widget that tells the dialogue is over

public:

// VARIABLES

	UPROPERTY(BlueprintReadOnly)
	bool bCanSwitchToDeathblowCamera;	// If true change the camera for deathblow

// METHODS

	void Death(bool bIsPlayerDeath);	// When the player or the enemy dies
	void StartFight(); 					// Start the last fight
};
