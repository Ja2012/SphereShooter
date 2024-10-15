// LICENSE.md

#include "UI/MainMenu/SsAddPlayerWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void USsAddPlayerWidget::NativeOnInitialized()
{
    AddButton->OnClicked.AddDynamic(this, &USsAddPlayerWidget::OnAddClicked);
}

void USsAddPlayerWidget::OnAddClicked() 
{
    FString Input = PlayerNameTextBox->GetText().ToString();
    Input.TrimStartAndEndInline();
   
    if (Input.IsEmpty() || Input.Len() > TextSizeLimit)
    {
        return;
    }
    OnPlayerAdd.Broadcast(Input);
}
