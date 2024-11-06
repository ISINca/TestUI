#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateBrush.h"
#include "CustomRoundedBoxBrush.generated.h"

USTRUCT(BlueprintType)
struct FCustomRoundedBoxBrush : public FSlateBrush
{
    GENERATED_BODY()

public:
    FCustomRoundedBoxBrush();

    // Базовые параметры закругления
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Corners")
    float CornerRadius;
    
    // X = TopLeft, Y = TopRight, Z = BottomRight, W = BottomLeft
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Corners", 
        meta=(DisplayName="Corner Radii (TL, TR, BR, BL)"))
    FVector4f CornerRadii;

    // Параметры обводки
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border")
    FLinearColor OutlineColor;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", meta=(InlineEditConditionToggle))
    bool bUseOutline;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", meta=(EditCondition="bUseOutline"))
    bool bUseUniformOutlineWidth;
    
    // Uniform outline width
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", 
        meta=(EditCondition="bUseOutline && bUseUniformOutlineWidth"))
    float OutlineWidth;
    
    // X = Top, Y = Right, Z = Bottom, W = Left
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", 
        meta=(EditCondition="bUseOutline && !bUseUniformOutlineWidth", 
              DisplayName="Outline Widths (T, R, B, L)"))
    FVector4f OutlineWidths;

    // Дополнительные эффекты
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Effects")
    float Opacity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Effects")
    bool bUseSoftEdges;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Effects")
    float SoftEdgeSize;

    // Вспомогательные методы для доступа к отдельным значениям
    FORCEINLINE float GetTopLeftRadius() const { return CornerRadii.X; }
    FORCEINLINE float GetTopRightRadius() const { return CornerRadii.Y; }
    FORCEINLINE float GetBottomRightRadius() const { return CornerRadii.Z; }
    FORCEINLINE float GetBottomLeftRadius() const { return CornerRadii.W; }

    FORCEINLINE float GetTopOutlineWidth() const { return OutlineWidths.X; }
    FORCEINLINE float GetRightOutlineWidth() const { return OutlineWidths.Y; }
    FORCEINLINE float GetBottomOutlineWidth() const { return OutlineWidths.Z; }
    FORCEINLINE float GetLeftOutlineWidth() const { return OutlineWidths.W; }
}; 