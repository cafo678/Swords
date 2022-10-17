#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "TutorialScriptActor.generated.h"

class APawnSword;
class AEnemySword;
class UTutorialWidget;
class UUIWidget;

UCLASS()
class SWORD_API ATutorialScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

private:

//VARIABLES

	FVector EnemyLocation = FVector(500.f, 0.f, 0.f);

	APawnSword* PlayerSword = nullptr;
	AEnemySword* NixSword = nullptr;

// METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> DialogueTemplate;	// Widget to set in the editor

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TSubclassOf<UUserWidget> UITemplate;		// Widget to set in the editor

	UPROPERTY(BlueprintReadOnly)
	UTutorialWidget* TutorialWidget = nullptr;	// Widget with tutorial dialogues

	UPROPERTY(BlueprintReadOnly)
	UUIWidget* UIWidget = nullptr;				// Widget with balance, name and state

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<AEnemySword> EnemyBlueprint;	// Enemy in the tutorial (Nix)
};
