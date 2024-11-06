#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "UI/CustomRoundedBoxBrush.h"

class TESTUI_API SCustomRoundedBox : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SCustomRoundedBox)
        : _Brush()
        , _Content()
    {}
        SLATE_ARGUMENT(FCustomRoundedBoxBrush, Brush)
        SLATE_DEFAULT_SLOT(FArguments, Content)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    // Переопределяем метод отрисовки
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
        int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    // Метод для обновления кисти
    void SetBrush(const FCustomRoundedBoxBrush& InBrush);

private:
    FCustomRoundedBoxBrush Brush;
    
    // Вспомогательные методы для отрисовки
    void DrawRoundedBox(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
        const FLinearColor& Color, float Opacity, int32 LayerId) const;
        
    void DrawOutline(FSlateWindowElementList& OutDrawElements, const FGeometry& AllottedGeometry,
        int32 LayerId) const;
}; 