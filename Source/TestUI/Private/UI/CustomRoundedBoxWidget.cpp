#include "UI/CustomRoundedBoxWidget.h"
#include "UI/SCustomRoundedBox.h"

UCustomRoundedBoxWidget::UCustomRoundedBoxWidget()
{
    // Инициализация базовых свойств виджета
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

void UCustomRoundedBoxWidget::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyRoundedBox.Reset();
}

void UCustomRoundedBoxWidget::SynchronizeProperties()
{
    Super::SynchronizeProperties();

    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
} 