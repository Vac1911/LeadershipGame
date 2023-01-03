// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Map/MapWidget.h"
#include "UObject/ObjectMacros.h"
#include "Input/Reply.h"
#include "Styling/SlateTypes.h"
#include "Widgets/SWidget.h"
#include "Components/ContentWidget.h"
#include "MapIconWidget.generated.h"

class SMapIcon;
class USlateWidgetStyleAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonClickedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonReleasedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonHoverEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonSelectedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonDeselectedEvent);

/**
 * The button is a click-able primitive widget to enable basic interaction, you
 * can place any other widget inside a button to make a more complex and
 * interesting click-able element in your UI.
 *
 * * Single Child
 * * Clickable
 */
UCLASS()
class LEADERSHIPGAME_API UMapIconWidget : public UContentWidget
{
	GENERATED_UCLASS_BODY()


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Map")
	UMapWidget* ParentMap;

	/** The template style asset, used to seed the mutable instance of the style. */
	UPROPERTY()
	TObjectPtr<USlateWidgetStyleAsset> Style_DEPRECATED;

	/** The button style used at runtime */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Appearance", meta = (DisplayName = "Style"))
	FButtonStyle WidgetStyle;

	/** The color multiplier for the button content */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
	FLinearColor ColorAndOpacity;

	/** The color multiplier for the button background */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance", meta = (sRGB = "true"))
	FLinearColor BackgroundColor;

	/** The type of mouse action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** The type of touch action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** The type of keyboard/gamepad button press action required by the user to trigger the buttons 'Click' */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction", AdvancedDisplay)
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Sometimes a button should only be mouse-clickable and never keyboard focusable. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool IsFocusable;

protected:
	/** True if there is a pending update as a result of calling `RequestUpdate()`. */
	bool IsPendingUpdate = true;

public:

	/** Called when the button is clicked */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonClickedEvent OnClicked;

	/** Called when the button is pressed */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonPressedEvent OnPressed;

	/** Called when the button is released */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonReleasedEvent OnReleased;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent OnHovered;

	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonHoverEvent OnUnhovered;

	/** Called when the icon is selected */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonSelectedEvent OnSelected;

	/** Called when the icon is unselected */
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnButtonDeselectedEvent OnDeselected;

public:
	UFUNCTION(BlueprintCallable, Category = "Map")
	static UMapIconWidget* CreateIcon(UUserWidget* InWidget, UMapWidget* MapWidget, FVector2D MapPosition);

	void SetMapPoint(FVector2D NextMapPoint);

	void SetScreenPosition(FVector2D NextScreenPosition);

	void RequestUpdate();

	virtual bool ShouldUpdatePosition();

	virtual void UpdatePosition();

	UFUNCTION(BlueprintCallable, Category = "Button")
	void Select() const;

	UFUNCTION(BlueprintCallable, Category = "Button")
	void Deselect() const;

	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
	void SetStyle(const FButtonStyle& InStyle);

	/** Sets the color multiplier for the button content */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
	void SetColorAndOpacity(FLinearColor InColorAndOpacity);

	/** Sets the color multiplier for the button background */
	UFUNCTION(BlueprintCallable, Category = "Button|Appearance")
	void SetBackgroundColor(FLinearColor InBackgroundColor);

	/**
	 * Returns true if the user is actively pressing the button.  Do not use this for detecting 'Clicks', use the OnClicked event instead.
	 */
	UFUNCTION(BlueprintCallable, Category = "Button")
	bool IsPressed() const;

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetClickMethod(EButtonClickMethod::Type InClickMethod);

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

public:

	template <typename IconT>
	FORCEINLINE_DEBUGGABLE static IconT* ConstructIcon(UUserWidget* InWidget, UMapWidget* MapWidget, TSubclassOf<UWidget> IconClass = IconT::StaticClass())
	{
		static_assert(TIsDerivedFrom<IconT, UMapIconWidget>::IsDerived, "MapIconWidget::ConstructIcon can only create UMapIconWidget objects.");

		UMapIconWidget* CreatedWidget = InWidget->WidgetTree->ConstructWidget<UMapIconWidget>(IconClass);
		CreatedWidget->ParentMap = MapWidget;
		CreatedWidget->SetStyle(MapWidget->IconStyle);

		if (MapWidget->GetCanvasSlot())
		{
			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MapWidget->GetCanvasSlot()->Parent->AddChild(CreatedWidget));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Map CanvasSlot not found");
		}

		MapWidget->Icons.Add(CreatedWidget);
		UE_LOG(LogTemp, Warning, TEXT("ConstructIcon"));

		return Cast<IconT>(CreatedWidget);
	}

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	//~ Begin UObject Interface
	virtual void PostLoad() override;
	//~ End UObject Interface

	FORCEINLINE_DEBUGGABLE FVector2D GetMapPoint() {
		return MapPoint;
	}

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif

protected:
	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* Slot) override;
	virtual void OnSlotRemoved(UPanelSlot* Slot) override;
	// End UPanelWidget

protected:
	/** Handle the actual click event from slate and forward it on */
	FReply SlateHandleClicked();
	void SlateHandlePressed();
	void SlateHandleReleased();
	void SlateHandleHovered();
	void SlateHandleUnhovered();
	void SlateHandleSelected();
	void SlateHandleDeselected();

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
#if WITH_EDITOR
	virtual TSharedRef<SWidget> RebuildDesignWidget(TSharedRef<SWidget> Content) override { return Content; }
#endif
	//~ End UWidget Interface

protected:
	/** Cached pointer to the underlying slate button owned by this UWidget */
	TSharedPtr<SMapIcon> MyMapIcon;

	/** World Location where icon appears */
	FVector WorldLocation;

	/** World Location where icon appears */
	FVector2D MapPoint;
};
