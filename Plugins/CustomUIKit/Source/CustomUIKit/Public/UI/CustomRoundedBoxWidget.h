#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "UI/CustomRoundedBoxBrush.h"
#include "CustomRoundedBoxWidget.generated.h"

UCLASS()
class UCustomRoundedBoxWidget : public UContentWidget
{
    GENERATED_BODY()

public:
    UCustomRoundedBoxWidget();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FCustomRoundedBoxBrush Brush;

    /** Устанавливает новый брашинг */
    UFUNCTION(BlueprintCallable, Category="Appearance")
    void SetBrush(const FCustomRoundedBoxBrush& InBrush);

    /** Sets the brush tint color */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetBrushColor(FLinearColor Color);

    /** Gets the brush tint color */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    FLinearColor GetBrushColor() const;

    /** Sets the outline color */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetOutlineColor(FLinearColor Color);

    /** Gets the outline color */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    FLinearColor GetOutlineColor() const;

    /** Sets the corner radii for all corners (X=TopLeft, Y=TopRight, Z=BottomRight, W=BottomLeft) */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetCornerRadii(const FVector4& Radii);

    /** Gets the corner radii */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    FVector4 GetCornerRadii() const;

    /** Sets the border width for all edges (X=Top, Y=Right, Z=Bottom, W=Left) */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetBorderWidth(const FVector4f& Width);

    /** Gets the border width */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    FVector4f GetBorderWidth() const;

    /** Sets whether to use outline */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetUseOutline(bool bUse);

    /** Gets whether outline is used */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    bool GetUseOutline() const;

    /** Sets the brush resource (texture) */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void SetBrushFromTexture(UTexture2D* Texture);

    /** Gets the current texture resource */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    UTexture2D* GetBrushTexture() const;

protected:
    virtual TSharedRef<SWidget> RebuildWidget() override;
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    virtual void SynchronizeProperties() override;

#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

private:
    TSharedPtr<class SCustomRoundedBox> MyRoundedBox;
}; 