#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWidget.generated.h"

class APawnSword;
class AEnemySword;

UCLASS()
class SWORD_API UUIWidget : public UUserWidget
{
	GENERATED_BODY()

private:

// METHODS

	virtual void NativeConstruct() override;
	
protected:

// VARIABLES

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	APawnSword* PlayerSword = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Custom")
	AEnemySword* EnemySword = nullptr;

public:

//METHODS

	void SetEnemySword(AEnemySword* EnemySwordToSet);


	
};
