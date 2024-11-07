#include "UI/RoundedCornerElement.h"
#include "Framework/Application/SlateApplication.h"
#include "Rendering/DrawElements.h"
#include "Rendering/RenderingCommon.h"

FRoundedCornerElement::FRoundedCornerElement(
    const FGeometry& InGeometry,
    float InCornerRadius,
    float InBorderWidth,
    const FLinearColor& InOutlineColor,
    bool bIsRightCorner,
    ESide InSide,
    const FCustomRoundedBoxBrush& InBrush)
    : CornerRadius(InCornerRadius)
    , BorderWidth(InBorderWidth)
    , OutlineColor(InOutlineColor)
    , bRightCorner(bIsRightCorner)
    , Side(InSide)
    , Brush(InBrush)
{
}

void FRoundedCornerElement::DrawRoundedCorner(
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    // Определяем толщины граничащих граней
    float HorizontalBorderWidth = 0.0f;
    float VerticalBorderWidth = 0.0f;

    switch (Side)
    {
    case ESide::Top:
        HorizontalBorderWidth = Brush.GetTopBorderWidth();
        VerticalBorderWidth = bRightCorner ? Brush.GetRightBorderWidth() : Brush.GetLeftBorderWidth();
        break;
    case ESide::Right:
        HorizontalBorderWidth = bRightCorner ? Brush.GetBottomBorderWidth() : Brush.GetTopBorderWidth();
        VerticalBorderWidth = Brush.GetRightBorderWidth();
        break;
    case ESide::Bottom:
        HorizontalBorderWidth = Brush.GetBottomBorderWidth();
        VerticalBorderWidth = bRightCorner ? Brush.GetRightBorderWidth() : Brush.GetLeftBorderWidth();
        break;
    case ESide::Left:
        HorizontalBorderWidth = bRightCorner ? Brush.GetBottomBorderWidth() : Brush.GetTopBorderWidth();
        VerticalBorderWidth = Brush.GetLeftBorderWidth();
        break;
    }

    // Пропускаем отрисовку, если обе граничащие грани имеют нулевую толщину
    if (HorizontalBorderWidth == 0.0f && VerticalBorderWidth == 0.0f)
    {
        return;
    }

    const int32 NumSegments = 16;
    const float AngleStep = PI / (2.0f * NumSegments);
    
    TArray<FSlateVertex> Vertices;
    TArray<SlateIndex> Indices;
    
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Определяем позиции и углы для каждого угла
    FVector2D Center;
    float StartAngle = 0.0f;

    switch (Side)
    {
    case ESide::Top:
        Center = bRightCorner ? 
            FVector2D(LocalSize.X - CornerRadius, CornerRadius) : 
            FVector2D(CornerRadius, CornerRadius);
        StartAngle = -PI/2.0f;
        break;
    case ESide::Right:
        Center = bRightCorner ? 
            FVector2D(LocalSize.X - CornerRadius, LocalSize.Y - CornerRadius) : 
            FVector2D(LocalSize.X - CornerRadius, CornerRadius);
        StartAngle = bRightCorner ? 0.0f : 0.0f;
        break;
    case ESide::Bottom:
        Center = bRightCorner ? 
            FVector2D(LocalSize.X - CornerRadius, LocalSize.Y - CornerRadius) : 
            FVector2D(CornerRadius, LocalSize.Y - CornerRadius);
        StartAngle = bRightCorner ? 0.0f : PI;
        break;
    case ESide::Left:
        Center = bRightCorner ? 
            FVector2D(CornerRadius, LocalSize.Y - CornerRadius) : 
            FVector2D(CornerRadius, CornerRadius);
        StartAngle = bRightCorner ? PI/2.0f : -PI/2.0f;
        break;
    }

    // Получаем трансформацию геометрии
    const FSlateRenderTransform& RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();

    // Создаем вершины
    for (int32 i = 0; i <= NumSegments; ++i)
    {
        float Angle = StartAngle + (bRightCorner ? i : -i) * AngleStep;
        
        const float CosAngle = FMath::Cos(Angle);
        const float SinAngle = FMath::Sin(Angle);

        // Внешняя граница совпадает с скруглением основного бокса
        const FVector2D OuterPos(
            Center.X + CornerRadius * CosAngle,
            Center.Y + CornerRadius * SinAngle
        );

        // Внутренняя граница - эллиптическая
        float InnerRadiusX = CornerRadius - VerticalBorderWidth;
        float InnerRadiusY = CornerRadius - HorizontalBorderWidth;

        FVector2D InnerPos;
        if (VerticalBorderWidth > CornerRadius)
        {
            float XOffset = InnerRadiusX;
            switch (Side)
            {
            case ESide::Left:
                XOffset = -InnerRadiusX;
                break;
            case ESide::Right:
                XOffset = InnerRadiusX;
                break;
            case ESide::Top:
            case ESide::Bottom:
                XOffset = bRightCorner ? InnerRadiusX : -InnerRadiusX;
                break;
            }
            InnerPos = FVector2D(Center.X + XOffset, Center.Y);
        }
        else if (HorizontalBorderWidth > CornerRadius)
        {
            float YOffset = InnerRadiusY;
            switch (Side)
            {
            case ESide::Top:
                YOffset = -InnerRadiusY;
                break;
            case ESide::Bottom:
                YOffset = InnerRadiusY;
                break;
            case ESide::Left:
            case ESide::Right:
                YOffset = bRightCorner ? InnerRadiusY : -InnerRadiusY;
                break;
            }
            InnerPos = FVector2D(Center.X, Center.Y + YOffset);
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
} 