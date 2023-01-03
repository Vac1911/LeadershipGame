#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Styling/SlateColor.h"
#include "Layout/Margin.h"
#include "Sound/SlateSound.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Framework/SlateDelegates.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Widgets/Layout/SBorder.h"

/**
 * MapIcons are clickable Widgets that can contain arbitrary widgets as its Content().
 */
class FPaintArgs;
class FSlateWindowElementList;
enum class ETextFlowDirection : uint8;
enum class ETextShapingMethod : uint8;

/**
 * Slate's Buttons are clickable Widgets that can contain arbitrary widgets as its Content().
 */
class LEADERSHIPGAME_API SMapIcon : public SBorder
{
	SLATE_DECLARE_WIDGET(SMapIcon, SBorder)

public:

	SLATE_BEGIN_ARGS(SMapIcon)
		: _Content()
		, _ButtonStyle(&FCoreStyle::Get().GetWidgetStyle< FButtonStyle >("Button"))
		, _TextStyle(&FCoreStyle::Get().GetWidgetStyle< FTextBlockStyle >("ButtonText"))
		, _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _ContentPadding(FMargin(4.0, 2.0))
		, _Text()
		, _ClickMethod(EButtonClickMethod::DownAndUp)
		, _TouchMethod(EButtonTouchMethod::DownAndUp)
		, _PressMethod(EButtonPressMethod::DownAndUp)
		, _DesiredSizeScale(FVector2D(1, 1))
		, _ContentScale(FVector2D(1, 1))
		, _ButtonColorAndOpacity(FLinearColor::White)
		, _ForegroundColor(FSlateColor::UseStyle())
		, _IsFocusable(true)
	{
	}

	/** Slot for this icon's content (optional) */
	SLATE_DEFAULT_SLOT(FArguments, Content)

		/** The visual style of the icon */
		SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)

		/** The text style of the icon */
		SLATE_STYLE_ARGUMENT(FTextBlockStyle, TextStyle)

		/** Horizontal alignment */
		SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

		/** Vertical alignment */
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)

		/** Spacing between icon's border and the content. */
		SLATE_ATTRIBUTE(FMargin, ContentPadding)

		/** The text to display in this icon, if no custom content is specified */
		SLATE_ATTRIBUTE(FText, Text)

		/** Called when the icon is clicked */
		SLATE_EVENT(FOnClicked, OnClicked)

		/** Called when the icon is pressed */
		SLATE_EVENT(FSimpleDelegate, OnPressed)

		/** Called when the icon is released */
		SLATE_EVENT(FSimpleDelegate, OnReleased)

		/** Called when the icon is selected */
		SLATE_EVENT(FSimpleDelegate, OnSelected)

		/** Called when the icon is deselected */
		SLATE_EVENT(FSimpleDelegate, OnDeselected)

		SLATE_EVENT(FSimpleDelegate, OnHovered)

		SLATE_EVENT(FSimpleDelegate, OnUnhovered)

		/** Sets the rules to use for determining whether the icon was clicked.  This is an advanced setting and generally should be left as the default. */
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)

		/** How should the icon be clicked with touch events? */
		SLATE_ARGUMENT(EButtonTouchMethod::Type, TouchMethod)

		/** How should the icon be clicked with keyboard/controller icon events? */
		SLATE_ARGUMENT(EButtonPressMethod::Type, PressMethod)

		SLATE_ATTRIBUTE(FVector2D, DesiredSizeScale)

		SLATE_ATTRIBUTE(FVector2D, ContentScale)

		SLATE_ATTRIBUTE(FSlateColor, ButtonColorAndOpacity)

		SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)

		/** Sometimes a icon should only be mouse-clickable and never keyboard focusable. */
		SLATE_ARGUMENT(bool, IsFocusable)

		/** The sound to play when the icon is pressed */
		SLATE_ARGUMENT(TOptional<FSlateSound>, PressedSoundOverride)

		/** The sound to play when the icon is hovered */
		SLATE_ARGUMENT(TOptional<FSlateSound>, HoveredSoundOverride)

		/** Which text shaping method should we use? (unset to use the default returned by GetDefaultTextShapingMethod) */
		SLATE_ARGUMENT(TOptional<ETextShapingMethod>, TextShapingMethod)

		/** Which text flow direction should we use? (unset to use the default returned by GetDefaultTextFlowDirection) */
		SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

		SLATE_END_ARGS()

protected:
	SMapIcon();

public:
	/**
	 * @return An image that represents this icon's border
	 */
	UE_DEPRECATED(5.0, "GetBorder is deprecated. Use SetBorderImage or GetBorderImage")
		virtual const FSlateBrush* GetBorder() const
	{
		return GetBorderImage();
	}

	/** @return the Foreground color that this widget sets; unset options if the widget does not set a foreground color */
	virtual FSlateColor GetForegroundColor() const final
	{
		return Super::GetForegroundColor();
	}

	/** @return the Foreground color that this widget sets when this widget or any of its ancestors are disabled; unset options if the widget does not set a foreground color */
	virtual FSlateColor GetDisabledForegroundColor() const final;

	/**
	 * Returns true if this icon is currently pressed
	 *
	 * @return	True if pressed, otherwise false
	 * @note IsPressed used to be virtual. Use SetAppearPressed to assign an attribute if you need to override the default behavior.
	 */
	bool IsPressed() const
	{
		return bIsPressed || AppearPressedAttribute.Get();
	}

	/**
	 * Returns true if this icon is currently selected
	 *
	 * @return	True if selected, otherwise false
	 */
	bool IsSelected() const
	{
		return bIsSelected;
	}

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	/** See ContentPadding attribute */
	void SetContentPadding(TAttribute<FMargin> InContentPadding);

	/** See HoveredSound attribute */
	void SetHoveredSound(TOptional<FSlateSound> InHoveredSound);

	/** See PressedSound attribute */
	void SetPressedSound(TOptional<FSlateSound> InPressedSound);

	/** See OnClicked event */
	void SetOnClicked(FOnClicked InOnClicked);

	/** Set OnHovered event */
	void SetOnHovered(FSimpleDelegate InOnHovered);

	/** Set OnUnhovered event */
	void SetOnUnhovered(FSimpleDelegate InOnUnhovered);

	/** See ButtonStyle attribute */
	void SetButtonStyle(const FButtonStyle* ButtonStyle);

	void SetClickMethod(EButtonClickMethod::Type InClickMethod);
	void SetTouchMethod(EButtonTouchMethod::Type InTouchMethod);
	void SetPressMethod(EButtonPressMethod::Type InPressMethod);

#if !UE_BUILD_SHIPPING
	void SimulateClick();
#endif // !UE_BUILD_SHIPPING

public:
	//~ SWidget overrides
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual bool SupportsKeyboardFocus() const override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual bool IsInteractable() const override;

private:
	virtual FVector2D ComputeDesiredSize(float) const override;
	//~ SWidget

public:
	/** Select the icon */
	virtual void Select();

	/** Deselect the icon */
	virtual void Deselect();

protected:
	/** Press the icon */
	virtual void Press();

	/** Release the icon */
	virtual void Release();

	/** Execute the "OnClicked" delegate, and get the reply */
	FReply ExecuteOnClick();

	/** @return combines the user-specified margin and the icon's internal margin. */
	FMargin GetCombinedPadding() const;

	/** @return True if the disabled effect should be shown. */
	bool GetShowDisabledEffect() const;

	/** Utility function to translate other input click methods to regular ones. */
	TEnumAsByte<EButtonClickMethod::Type> GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const;

	/** Utility function to determine if the incoming mouse event is for a precise tap or click */
	bool IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const;

	/** Play the pressed sound */
	void PlayPressedSound() const;

	/** Play the hovered sound */
	void PlayHoverSound() const;

	/** Set if this icon can be focused */
	void SetIsFocusable(bool bInIsFocusable)
	{
		bIsFocusable = bInIsFocusable;
	}

	void ExecuteHoverStateChanged(bool bPlaySound);

protected:
	/** @return the BorderForegroundColor attribute. */
	TSlateAttributeRef<FSlateColor> GetBorderForegroundColorAttribute() const { return TSlateAttributeRef<FSlateColor>(SharedThis(this), BorderForegroundColorAttribute); }

	/** @return the ContentPadding attribute. */
	TSlateAttributeRef<FMargin> GetContentPaddingAttribute() const { return TSlateAttributeRef<FMargin>(SharedThis(this), ContentPaddingAttribute); }

	/** Set the AppearPressed look. */
	void SetAppearPressed(TAttribute<bool> InValue)
	{
		AppearPressedAttribute.Assign(*this, MoveTemp(InValue));
	}

	/** @return the AppearPressed attribute. */
	TSlateAttributeRef<bool> GetAppearPressedAttribute() const { return TSlateAttributeRef<bool>(SharedThis(this), AppearPressedAttribute); }

private:
	void UpdatePressStateChanged();

	void UpdatePadding();
	void UpdateShowDisabledEffect();
	void UpdateBorderImage();
	void UpdateForegroundColor();
	void UpdateDisabledForegroundColor();

protected:
#if WITH_EDITORONLY_DATA
	UE_DEPRECATED(5.0, "Direct access to ContentPadding is now deprecated. Use the setter or getter.")
		TSlateDeprecatedTAttribute<FMargin> ContentPadding;
	/** Brush resource that represents a icon */
	UE_DEPRECATED(5.0, "NormalImage is now deprecated. Use the ButtonStyle.")
		const FSlateBrush* NormalImage;
	/** Brush resource that represents a icon when it is hovered */
	UE_DEPRECATED(5.0, "HoverImage is now deprecated. Use the ButtonStyle.")
		const FSlateBrush* HoverImage;
	/** Brush resource that represents a icon when it is pressed */
	UE_DEPRECATED(5.0, "PressedImage is now deprecated. Use the ButtonStyle.")
		const FSlateBrush* PressedImage;
	/** Brush resource that represents a icon when it is disabled */
	UE_DEPRECATED(5.0, "DisabledImage is now deprecated. Use the ButtonStyle.")
		const FSlateBrush* DisabledImage;
	/** Padding that accounts for the icon border */
	UE_DEPRECATED(5.0, "BorderPadding is now deprecated. Use the ButtonStyle.")
		FMargin BorderPadding;
	/** Padding that accounts for the icon border when pressed */
	UE_DEPRECATED(5.0, "PressedBorderPadding is now deprecated. Use the ButtonStyle.")
		FMargin PressedBorderPadding;
#endif

private:
	/** The location in screenspace the icon was pressed */
	FVector2D PressedScreenSpacePosition;

	/** Style resource for the icon */
	const FButtonStyle* Style;
	/** The delegate to execute when the icon is clicked */
	FOnClicked OnClicked;

	/** The delegate to execute when the icon is pressed */
	FSimpleDelegate OnPressed;

	/** The delegate to execute when the icon is released */
	FSimpleDelegate OnReleased;

	/** The delegate to execute when the icon is selected */
	FSimpleDelegate OnSelected;

	/** The delegate to execute when the icon is deselected */
	FSimpleDelegate OnDeselected;

	/** The delegate to execute when the icon is hovered */
	FSimpleDelegate OnHovered;

	/** The delegate to execute when the icon exit the hovered state */
	FSimpleDelegate OnUnhovered;

	/** The Sound to play when the icon is hovered  */
	FSlateSound HoveredSound;

	/** The Sound to play when the icon is pressed */
	FSlateSound PressedSound;

	/** Sets whether a click should be triggered on mouse down, mouse up, or that both a mouse down and up are required. */
	TEnumAsByte<EButtonClickMethod::Type> ClickMethod;

	/** How should the icon be clicked with touch events? */
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** How should the icon be clicked with keyboard/controller icon events? */
	TEnumAsByte<EButtonPressMethod::Type> PressMethod;

	/** Can this icon be focused? */
	uint8 bIsFocusable : 1;

	/** True if this icon is currently in a pressed state */
	uint8 bIsPressed : 1;

	/** True if this icon is currently in a selected state */
	uint8 bIsSelected : 1;

private:
	/** Optional foreground color that will be inherited by all of this widget's contents */
	TSlateAttribute<FSlateColor> BorderForegroundColorAttribute;
	/** Padding specified by the user; it will be combind with the icon's internal padding. */
	TSlateAttribute<FMargin> ContentPaddingAttribute;
	/** Optional foreground color that will be inherited by all of this widget's contents */
	TSlateAttribute<bool> AppearPressedAttribute;
};
