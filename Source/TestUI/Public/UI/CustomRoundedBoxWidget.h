#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "UI/CustomRoundedBoxBrush.h"
#include "CustomRoundedBoxWidget.generated.h"

UCLASS()
class TESTUI_API UCustomRoundedBoxWidget : public UContentWidget
{
    GENERATED_BODY()

public:
    UCustomRoundedBoxWidget();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FCustomRoundedBoxBrush Brush;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    
    // Создаем Slate виджет
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    
    // Обновляем свойства при их изменении
    virtual void SynchronizeProperties() override;

private:
    TSharedPtr<class SCustomRoundedBox> MyRoundedBox;
}; 