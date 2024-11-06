#include "UI/SCustomRoundedBox.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomRoundedBox::Construct(const FArguments& InArgs)
{
    Brush = InArgs._Brush;

    ChildSlot
    [
        InArgs._Content.Widget
    ];
}

int32 SCustomRoundedBox::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // Отрисовка основного фона
    DrawRoundedBox(OutDrawElements, AllottedGeometry, Brush.TintColor.GetSpecifiedColor(), 
        Brush.GetResourceObject() ? Brush.GetResourceObject()->GetResourceOpacity() : 1.0f, LayerId);

    // Отрисовка обводки, если она включена
    if (Brush.bUseOutline)
    {
        DrawOutline(OutDrawElements, AllottedGeometry, LayerId + 1);
    }

    // Отрисовка дочерних элементов
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, 
        LayerId + 2, InWidgetStyle, bParentEnabled);
}

void SCustomRoundedBox::SetBrush(const FCustomRoundedBoxBrush& InBrush)
{
    Brush = InBrush;
}

void SCustomRoundedBox::DrawRoundedBox(FSlateWindowElementList& OutDrawElements, 
    const FGeometry& AllottedGeometry, const FLinearColor& Color, float Opacity, int32 LayerId) const
{
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Создаем геометрию для каждого угла с учетом разных радиусов
    TArray<FSlateVertex> Vertices;
    TArray<SlateIndex> Indices;

    // Добавляем вершины для основного прямоугольника
    Vertices.SetNum(4);
    Vertices[0].Position = FVector2f(0, 0);
    Vertices[1].Position = FVector2f(LocalSize.X, 0);
    Vertices[2].Position = FVector2f(LocalSize.X, LocalSize.Y);
    Vertices[3].Position = FVector2f(0, LocalSize.Y);

    // Добавляем индексы для треугольников
    Indices.SetNum(6);
    Indices[0] = 0;
    Indices[1] = 1;
    Indices[2] = 2;
    Indices[3] = 2;
    Indices[4] = 3;
    Indices[5] = 0;

    // Создаем элемент отрисовки
    FSlateDrawElement::MakeCustom(
        OutDrawElements,
        LayerId,
        FPaintGeometry(
            AllottedGeometry.ToPaintGeometry()
        ),
        FSlateVertex(Vertices),
        Indices,
        nullptr,
        0,
        0
    );
}

void SCustomRoundedBox::DrawOutline(FSlateWindowElementList& OutDrawElements, 
    const FGeometry& AllottedGeometry, int32 LayerId) const
{
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Получаем толщины линий для каждой стороны
    const float TopWidth = Brush.bUseUniformOutlineWidth ? Brush.OutlineWidth : Brush.OutlineWidths.X;
    const float RightWidth = Brush.bUseUniformOutlineWidth ? Brush.OutlineWidth : Brush.OutlineWidths.Y;
    const float BottomWidth = Brush.bUseUniformOutlineWidth ? Brush.OutlineWidth : Brush.OutlineWidths.Z;
    const float LeftWidth = Brush.bUseUniformOutlineWidth ? Brush.OutlineWidth : Brush.OutlineWidths.W;

    // Отрисовка линий обводки
    // Верхняя линия
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(0, 0), FVector2D(LocalSize.X, TopWidth)),
        &Brush,
        ESlateDrawEffect::None,
        Brush.OutlineColor
    );

    // Правая линия
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(LocalSize.X - RightWidth, TopWidth), 
            FVector2D(RightWidth, LocalSize.Y - TopWidth - BottomWidth)),
        &Brush,
        ESlateDrawEffect::None,
        Brush.OutlineColor
    );

    // Нижняя линия
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(0, LocalSize.Y - BottomWidth), 
            FVector2D(LocalSize.X, BottomWidth)),
        &Brush,
        ESlateDrawEffect::None,
        Brush.OutlineColor
    );

    // Левая линия
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(FVector2D(0, TopWidth), 
            FVector2D(LeftWidth, LocalSize.Y - TopWidth - BottomWidth)),
        &Brush,
        ESlateDrawEffect::None,
        Brush.OutlineColor
    );
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION 