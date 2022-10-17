#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "ArenaAIController2.generated.h"

UCLASS()
class SWORD_API AArenaAIController2 : public AMyAIController
{
	GENERATED_BODY()

private:

// VARIABLES

	bool bDoOnceIncerementHits = true;
	TArray<float> HitDelaysArray;
	TArray<FVector> HitLocationArray;
	
// METHODS

	virtual void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	void SetupCombo() override;
};
