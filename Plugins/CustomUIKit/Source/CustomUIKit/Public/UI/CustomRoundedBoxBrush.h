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
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", meta=(InlineEditConditionToggle))
    bool bUseOutline;

    // X = Top, Y = Right, Z = Bottom, W = Left
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", 
        meta=(EditCondition="bUseOutline", DisplayName="Border Width Per Edge"))
    FVector4f BorderWidth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance|Border", meta=(EditCondition="bUseOutline"))
    FLinearColor OutlineColor;

    // Helper methods
    FORCEINLINE float GetTopBorderWidth() const { return BorderWidth.X; }
    FORCEINLINE float GetRightBorderWidth() const { return BorderWidth.Y; }
    FORCEINLINE float GetBottomBorderWidth() const { return BorderWidth.Z; }
    FORCEINLINE float GetLeftBorderWidth() const { return BorderWidth.W; }
}; 