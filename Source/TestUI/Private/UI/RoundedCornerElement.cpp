#include "UI/RoundedCornerElement.h"
#include "Framework/Application/SlateApplication.h"
#include "Rendering/DrawElements.h"
#include "Rendering/RenderingCommon.h"

FRoundedCornerElement::FRoundedCornerElement(
    const FGeometry& InGeometry,
    float InCornerRadius,
    float InBorderWidth,
    const FLinearColor& InOutlineColor,
    ECornerPosition InCornerPosition,
    const FCustomRoundedBoxBrush& InBrush)
    : CornerRadius(InCornerRadius)
    , BorderWidth(InBorderWidth)
    , OutlineColor(InOutlineColor)
    , CornerPosition(InCornerPosition)
    , Brush(InBrush)
{
}

void FRoundedCornerElement::DrawRoundedCorner(
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Определяем параметры для каждого угла
    FVector2D Center;
    float StartAngle = 0.0f;
    bool bReverseAngle = false;

    switch (CornerPosition)
    {
    case ECornerPosition::TopLeft:
        Center = FVector2D(CornerRadius, CornerRadius);
        StartAngle = -PI/2.0f;
        bReverseAngle = false;
        break;
    case ECornerPosition::TopRight:
        Center = FVector2D(LocalSize.X - CornerRadius, CornerRadius);
        StartAngle = -PI/2.0f;
        bReverseAngle = true;
        break;
    case ECornerPosition::BottomRight:
        Center = FVector2D(LocalSize.X - CornerRadius, LocalSize.Y - CornerRadius);
        StartAngle = 0.0f;
        bReverseAngle = true;
        break;
    case ECornerPosition::BottomLeft:
        Center = FVector2D(CornerRadius, LocalSize.Y - CornerRadius);
        StartAngle = PI;
        bReverseAngle = false;
        break;
    }

    const int32 NumSegments = 16;
    const float AngleStep = PI / (2.0f * NumSegments);
    
    TArray<FSlateVertex> Vertices;
    TArray<SlateIndex> Indices;

    // Получаем толщины граничащих граней
    float HorizontalBorderWidth = 0.0f;
    float VerticalBorderWidth = 0.0f;

    switch (CornerPosition)
    {
    case ECornerPosition::TopLeft:
    case ECornerPosition::TopRight:
        HorizontalBorderWidth = Brush.GetTopBorderWidth();
        VerticalBorderWidth = CornerPosition == ECornerPosition::TopLeft ? 
            Brush.GetLeftBorderWidth() : Brush.GetRightBorderWidth();
        break;
    case ECornerPosition::BottomLeft:
    case ECornerPosition::BottomRight:
        HorizontalBorderWidth = Brush.GetBottomBorderWidth();
        VerticalBorderWidth = CornerPosition == ECornerPosition::BottomLeft ? 
            Brush.GetLeftBorderWidth() : Brush.GetRightBorderWidth();
        break;
    }

    // Получаем трансформацию геометрии
    const FSlateRenderTransform& RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();

    // Создаем вершины
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float Angle = StartAngle + (bReverseAngle ? i : -i) * AngleStep;
        
        const float CosAngle = FMath::Cos(Angle);
        const float SinAngle = FMath::Sin(Angle);

        // Внешняя граница - сначала вычисляем базовую позицию
        FVector2D OuterPos(
            Center.X + CornerRadius * CosAngle,
            Center.Y + CornerRadius * SinAngle
        );

        // Если нужно, корректируем только одну координату
        if (VerticalBorderWidth > CornerRadius && (i == 0 || i == NumSegments))
        {
            const bool bIsRight = CornerPosition == ECornerPosition::TopRight || 
                                CornerPosition == ECornerPosition::BottomRight;
            const bool bIsBottom = CornerPosition == ECornerPosition::BottomLeft || 
                                 CornerPosition == ECornerPosition::BottomRight;

            if ((bIsBottom && i == NumSegments) || (!bIsBottom && i == 0))
            {
                OuterPos.X = bIsRight ? 
                    LocalSize.X - VerticalBorderWidth : 
                    VerticalBorderWidth;
            }
        }
        else if (HorizontalBorderWidth > CornerRadius && (i == 0 || i == NumSegments))
        {
            switch (CornerPosition)
            {
            case ECornerPosition::TopLeft:
                if (i == NumSegments)
                    OuterPos.Y = HorizontalBorderWidth;
                break;
            case ECornerPosition::TopRight:
                if (i == NumSegments)
                    OuterPos.Y = HorizontalBorderWidth;
                break;
            case ECornerPosition::BottomRight:
                if (i == 0)
                    OuterPos.Y = LocalSize.Y - HorizontalBorderWidth;
                break;
            case ECornerPosition::BottomLeft:
                if (i == 0)
                    OuterPos.Y = LocalSize.Y - HorizontalBorderWidth;
                break;
            }
        }

        // Внутренняя граница - эллиптическая
        float InnerRadiusX = CornerRadius - VerticalBorderWidth;
        float InnerRadiusY = CornerRadius - HorizontalBorderWidth;

        FVector2D InnerPos;
        if (VerticalBorderWidth > CornerRadius)
        {
            const bool bIsRight = CornerPosition == ECornerPosition::TopRight || 
                                CornerPosition == ECornerPosition::BottomRight;
            
            InnerPos = FVector2D(
                Center.X + (bIsRight ? InnerRadiusX : -InnerRadiusX),
                Center.Y
            );
        }
        else if (HorizontalBorderWidth > CornerRadius)
        {
            const bool bIsBottom = CornerPosition == ECornerPosition::BottomLeft || 
                                 CornerPosition == ECornerPosition::BottomRight;
            
            InnerPos = FVector2D(
                Center.X,
                Center.Y + (bIsBottom ? InnerRadiusY : -InnerRadiusY)
            );
        }
        else
        {
            InnerPos = FVector2D(
                Center.X + InnerRadiusX * CosAngle,
                Center.Y + InnerRadiusY * SinAngle
            );
        }

        // Применяем трансформацию
        const FVector2D TransformedOuterPos = RenderTransform.TransformPoint(OuterPos);
        const FVector2D TransformedInnerPos = RenderTransform.TransformPoint(InnerPos);

        // Добавляем вершины
        FSlateVertex OuterVertex;
        OuterVertex.Position = FVector2f(TransformedOuterPos);
        OuterVertex.Color = OutlineColor.ToFColor(true);
        OuterVertex.TexCoords[0] = OuterVertex.TexCoords[1] = 
        OuterVertex.TexCoords[2] = OuterVertex.TexCoords[3] = 0.0f;
        
        FSlateVertex InnerVertex;
        InnerVertex.Position = FVector2f(TransformedInnerPos);
        InnerVertex.Color = OutlineColor.ToFColor(true);
        InnerVertex.TexCoords[0] = InnerVertex.TexCoords[1] = 
        InnerVertex.TexCoords[2] = InnerVertex.TexCoords[3] = 0.0f;
        
        Vertices.Add(OuterVertex);
        Vertices.Add(InnerVertex);
    }
    
    // Создаем индексы для треугольников
    for (int32 i = 0; i < NumSegments; ++i)
    {
        const int32 BaseIdx = i * 2;
        
        // Первый треугольник
        Indices.Add(BaseIdx);
        Indices.Add(BaseIdx + 1);
        Indices.Add(BaseIdx + 2);
        
        // Второй треугольник
        Indices.Add(BaseIdx + 1);
        Indices.Add(BaseIdx + 3);
        Indices.Add(BaseIdx + 2);
    }
    
    // Отрисовываем геометрию
    FSlateDrawElement::MakeCustomVerts(
        OutDrawElements,
        LayerId + 1,
        FSlateResourceHandle(),
        Vertices,
        Indices,
        nullptr,
        0,
        0,
        ESlateDrawEffect::None
    );

    // Отрисока дебаг-точек для каждой вершины
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        const FVector2D VertexPos = FVector2D(Vertices[i].Position);
        const FVector2D DebugPointSize(4.0f, 4.0f);  // Увеличим размер для лучшей видимости

        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId + 2,
            FPaintGeometry(
                VertexPos - DebugPointSize * 0.5f,  // Центрируем точку
                DebugPointSize,
                1.0f
            ),
            FAppStyle::Get().GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            FLinearColor::Red
        );
    }
} 