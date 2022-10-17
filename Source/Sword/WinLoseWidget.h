#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinLoseWidget.generated.h"


UCLASS()
class SWORD_API UWinLoseWidget : public UUserWidget
{
	GENERATED_BODY()

public:

//VARIABLES

	UPROPERTY (BlueprintReadOnly, Category = "Custom")
	FText WinLoseText;

	UPROPERTY (BlueprintReadOnly, Category = "Custom")
	FText QuestionText;

	UPROPERTY (BlueprintReadWrite, Category = "Custom")
	bool bIsNoSelected = false;
};
