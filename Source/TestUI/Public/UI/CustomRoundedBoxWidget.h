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

    /** Устанавливает новый брашинг */
    UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrush(const FCustomRoundedBoxBrush& InBrush);

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    virtual void SynchronizeProperties() override;

#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

private:
    TSharedPtr<class SCustomRoundedBox> MyRoundedBox;
}; 