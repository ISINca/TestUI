#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UI/CustomRoundedBoxBrush.h"
#include "UI/RoundedCornerElement.h"

class SCustomRoundedBox : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCustomRoundedBox)
        : _Content()
        , _Brush()
    {}
        SLATE_DEFAULT_SLOT(FArguments, Content)
        SLATE_ARGUMENT(FCustomRoundedBoxBrush, Brush)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);
    void SetContent(const TSharedRef<SWidget>& InContent);
    void SetBrush(const FCustomRoundedBoxBrush& InBrush);

    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
    FCustomRoundedBoxBrush Brush;
    
    void DrawRoundedBox(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
        int32 LayerId) const;
        
    void DrawOutline(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
        int32 LayerId) const;

    void DrawCornerOutline(
        FSlateWindowElementList& OutDrawElements,
        const FGeometry& AllottedGeometry,
        int32 LayerId,
        ECornerPosition Corner) const;
}; 