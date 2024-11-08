#include "UI/CustomRoundedBoxWidget.h"
#include "UI/SCustomRoundedBox.h"

UCustomRoundedBoxWidget::UCustomRoundedBoxWidget()
{
    SetVisibility(ESlateVisibility::Visible);
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

void UCustomRoundedBoxWidget::SetBrushColor(FLinearColor Color)
{
    Brush.TintColor = Color;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

FLinearColor UCustomRoundedBoxWidget::GetBrushColor() const
{
    return Brush.TintColor.GetSpecifiedColor();
}

void UCustomRoundedBoxWidget::SetOutlineColor(FLinearColor Color)
{
    Brush.OutlineColor = Color;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

FLinearColor UCustomRoundedBoxWidget::GetOutlineColor() const
{
    return Brush.OutlineColor;
}

void UCustomRoundedBoxWidget::SetBorderWidth(const FVector4f& Width)
{
    Brush.BorderWidth = Width;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

FVector4f UCustomRoundedBoxWidget::GetBorderWidth() const
{
    return Brush.BorderWidth;
}

void UCustomRoundedBoxWidget::SetUseOutline(bool bUse)
{
    Brush.bUseOutline = bUse;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

bool UCustomRoundedBoxWidget::GetUseOutline() const
{
    return Brush.bUseOutline;
}

void UCustomRoundedBoxWidget::SetBrushFromTexture(UTexture2D* Texture)
{
    if (Texture)
    {
        Brush.SetResourceObject(Texture);
        if (MyRoundedBox.IsValid())
        {
            MyRoundedBox->SetBrush(Brush);
        }
    }
}

UTexture2D* UCustomRoundedBoxWidget::GetBrushTexture() const
{
    return Cast<UTexture2D>(Brush.GetResourceObject());
}

void UCustomRoundedBoxWidget::SetCornerRadii(const FVector4& Radii)
{
    Brush.OutlineSettings.CornerRadii = Radii;
    if (MyRoundedBox.IsValid())
    {
        MyRoundedBox->SetBrush(Brush);
    }
}

FVector4 UCustomRoundedBoxWidget::GetCornerRadii() const
{
    return Brush.OutlineSettings.CornerRadii;
}

#if WITH_EDITOR
const FText UCustomRoundedBoxWidget::GetPaletteCategory()
{
    return NSLOCTEXT("UI", "Custom", "Custom");
}
#endif 