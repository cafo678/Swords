#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWidgetEvent);

UCLASS()
class SWORD_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

// METHODS

	UFUNCTION (BlueprintCallable, Category = "Custom")
	void CallStartLastFightEvent();	// Call the event OnDialogueOver

public:

// VARIABLES

	UPROPERTY(BlueprintReadWrite)
	int32 Index = 0;

// METHODS

	FWidgetEvent OnDialogueOver;	
};
