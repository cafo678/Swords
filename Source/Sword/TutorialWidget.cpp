#include "TutorialWidget.h"

void UTutorialWidget::CallStartLastFightEvent()
{
    OnDialogueOver.Broadcast();
}

