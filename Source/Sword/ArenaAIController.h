#pragma once

#include "CoreMinimal.h"
#include "MyAIController.h"
#include "ArenaAIController.generated.h"

UCLASS()
class SWORD_API AArenaAIController : public AMyAIController
{
	GENERATED_BODY()

private:

// METHODS

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};
