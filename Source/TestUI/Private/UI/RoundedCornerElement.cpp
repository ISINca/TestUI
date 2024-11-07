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
    ESide InSide)
    : CornerRadius(InCornerRadius)
    , BorderWidth(InBorderWidth)
    , OutlineColor(InOutlineColor)
    , bRightCorner(bIsRightCorner)
    , Side(InSide)
{
}

void FRoundedCornerElement::DrawRoundedCorner(
    const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect,
    FSlateWindowElementList& OutDrawElements,
    int32 LayerId) const
{
    if (CornerRadius <= 0.0f || BorderWidth <= 0.0f)
    {
        return;
    }

    const int32 NumSegments = 16;
    const float AngleStep = PI / (2.0f * NumSegments);
    
    TArray<FSlateVertex> Vertices;
    TArray<SlateIndex> Indices;
    
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Определяем параметры для каждой стороны
    FVector2D Center;
    float StartAngle = 0.0f;
    bool bReverseThickness = false;

    // Определяем позиции и углы для каждого угла
    switch (Side)
    {
    case ESide::Top:
        if (bRightCorner)
        {
            // Верхний правый угол (1-я четверть)
            Center = FVector2D(LocalSize.X - CornerRadius, CornerRadius);
            StartAngle = -PI/2.0f;
            bReverseThickness = true;
        }
        else
        {
            // Верхний левый угол (1-я четверть)
            Center = FVector2D(CornerRadius, CornerRadius);
            StartAngle = -PI/2.0f;
            bReverseThickness = true;
        }
        break;

    case ESide::Right:
        if (bRightCorner)
        {
            // Нижний правый угол (2-я четверть)
            Center = FVector2D(LocalSize.X - CornerRadius, LocalSize.Y - CornerRadius);
            StartAngle = 0.0f;
            bReverseThickness = true;
        }
        else
        {
            // Верхний правый угол (2-я четверть)
            Center = FVector2D(LocalSize.X - CornerRadius, CornerRadius);
            StartAngle = 0.0f;
            bReverseThickness = true;
        }
        break;

    case ESide::Bottom:
        if (bRightCorner)
        {
            // Нижний правый угол (2-я четверть)
            Center = FVector2D(LocalSize.X - CornerRadius, LocalSize.Y - CornerRadius);
            StartAngle = 0.0f;
            bReverseThickness = false;
        }
        else
        {
            // Нижний левый угол (4-я четверть)
            Center = FVector2D(CornerRadius, LocalSize.Y - CornerRadius);
            StartAngle = PI;
            bReverseThickness = false;
        }
        break;

    case ESide::Left:
        if (bRightCorner)
        {
            // Нижний левый угол (меняем на 3-ю четверть)
            Center = FVector2D(CornerRadius, LocalSize.Y - CornerRadius);
            StartAngle = PI/2.0f;  // Угол для 3-й четверти
            bReverseThickness = false;
        }
        else
        {
            // Верхний левый угол (1-я четверть)
            Center = FVector2D(CornerRadius, CornerRadius);
            StartAngle = -PI/2.0f;
            bReverseThickness = false;
        }
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

        // Вычисляем коэффициент уменьшения толщины
        const float ThicknessScale = bReverseThickness ? 
            FMath::Sin((NumSegments - i) * AngleStep) : 
            FMath::Sin(i * AngleStep);
        
        // Вычисляем текущую толщину обводки
        const float CurrentBorderWidth = BorderWidth * ThicknessScale;
        
        // Вычисляем радиусы с учетом текущей толщины
        const float OuterRadius = CornerRadius;
        const float InnerRadius = CornerRadius - CurrentBorderWidth;
        
        // Вычисляем позиции в локальных координатах
        const FVector2D OuterPos(
            Center.X + OuterRadius * CosAngle,
            Center.Y + OuterRadius * SinAngle
        );
        
        const FVector2D InnerPos(
            Center.X + InnerRadius * CosAngle,
            Center.Y + InnerRadius * SinAngle
        );

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