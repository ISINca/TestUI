#include "UI/CustomRoundedBoxWidget.h"
#include "UI/SCustomRoundedBox.h"

UCustomRoundedBoxWidget::UCustomRoundedBoxWidget()
{
    // Устанавливаем базовые параметры виджета
    Visibility = ESlateVisibility::Visible;
    bIsVariable = false;
}

void UCustomRoundedBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyRoundedBox.Reset();
}

TSharedRef<SWidget> UCustomRoundedBoxWidget::RebuildWidget()
{
    MyRoundedBox = SNew(SCustomRoundedBox)
        .Brush(Brush);

    if (GetChildrenCount() > 0)
    {
        MyRoundedBox->SetContent(GetContentSlot()->Content ? GetContentSlot()->Content->TakeWidget() : SNullWidget::NullWidget);
    }

    return MyRoundedBox.ToSharedRef();
}

void UCustomRoundedBoxWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

void UCustomRoundedBoxWidget::SetBrush(const FCustomRoundedBoxBrush& InBrush)
{
    Brush = InBrush;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

#if WITH_EDITOR
const FText UCustomRoundedBoxWidget::GetPaletteCategory()
{
    return NSLOCTEXT("UI", "Custom", "Custom");
}
#endif 