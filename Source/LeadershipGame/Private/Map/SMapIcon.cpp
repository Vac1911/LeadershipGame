// Copyright Epic Games, Inc. All Rights Reserved.

#include "Map/SMapIcon.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Text/STextBlock.h"
#if WITH_ACCESSIBILITY
#include "Widgets/Accessibility/SlateAccessibleWidgets.h"
#include "Widgets/Accessibility/SlateAccessibleMessageHandler.h"
#endif

static FName SMapIconTypeName("SMapIcon");

SLATE_IMPLEMENT_WIDGET(SMapIcon)
void SMapIcon::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "BaseBorderForegroundColor", BorderForegroundColorAttribute, EInvalidateWidgetReason::Paint)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SMapIcon&>(Widget).UpdateForegroundColor();
			}));

	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "ContentPadding", ContentPaddingAttribute, EInvalidateWidgetReason::Layout)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SMapIcon&>(Widget).UpdatePadding();
			}));

	SLATE_ADD_MEMBER_ATTRIBUTE_DEFINITION_WITH_NAME(AttributeInitializer, "AppearPressed", AppearPressedAttribute, EInvalidateWidgetReason::Paint)
		.OnValueChanged(FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SMapIcon&>(Widget).UpdatePressStateChanged();
			}));

	AttributeInitializer.OverrideInvalidationReason("EnabledState", FSlateAttributeDescriptor::FInvalidateWidgetReasonAttribute{ EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint });

	AttributeInitializer.OverrideOnValueChanged("EnabledState"
		, FSlateAttributeDescriptor::ECallbackOverrideType::ExecuteAfterPrevious
		, FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SMapIcon&>(Widget).UpdateBorderImage();
			}));

	AttributeInitializer.OverrideOnValueChanged("Hovered"
		, FSlateAttributeDescriptor::ECallbackOverrideType::ExecuteAfterPrevious
		, FSlateAttributeDescriptor::FAttributeValueChangedDelegate::CreateLambda([](SWidget& Widget)
			{
				static_cast<SMapIcon&>(Widget).UpdateBorderImage();
				static_cast<SMapIcon&>(Widget).UpdateForegroundColor();
			}));
}

SMapIcon::SMapIcon()
	: BorderForegroundColorAttribute(*this)
	, ContentPaddingAttribute(*this)
	, AppearPressedAttribute(*this)
{
#if WITH_ACCESSIBILITY
	AccessibleBehavior = EAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

/**
 * Construct this widget
 *
 * @param	InArgs	The declaration data for this widget
 */
void SMapIcon::Construct(const FArguments& InArgs)
{
	bIsPressed = false;
	bIsSelected = false;
	bIsFocusable = InArgs._IsFocusable;

	BorderForegroundColorAttribute.Assign(*this, InArgs._ForegroundColor);

	OnClicked = InArgs._OnClicked;
	OnPressed = InArgs._OnPressed;
	OnReleased = InArgs._OnReleased;
	OnHovered = InArgs._OnHovered;
	OnUnhovered = InArgs._OnUnhovered;
	OnSelected = InArgs._OnSelected;
	OnDeselected = InArgs._OnDeselected;

	ClickMethod = InArgs._ClickMethod;
	TouchMethod = InArgs._TouchMethod;
	PressMethod = InArgs._PressMethod;

	HoveredSound = InArgs._HoveredSoundOverride.Get(InArgs._ButtonStyle->HoveredSlateSound);
	PressedSound = InArgs._PressedSoundOverride.Get(InArgs._ButtonStyle->PressedSlateSound);

	// Text overrides button content. If nothing is specified, put an null widget in the button.
	// Null content makes the button enter a special mode where it will ask to be as big as the image used for its border.
	struct
	{
		TSharedRef<SWidget> operator()(const FArguments& InOpArgs) const
		{
			if ((InOpArgs._Content.Widget == SNullWidget::NullWidget) && (InOpArgs._Text.IsBound() || !InOpArgs._Text.Get().IsEmpty()))
			{
				return SNew(STextBlock)
					.Visibility(EVisibility::HitTestInvisible)
					.Text(InOpArgs._Text)
					.TextStyle(InOpArgs._TextStyle)
					.TextShapingMethod(InOpArgs._TextShapingMethod)
					.TextFlowDirection(InOpArgs._TextFlowDirection);
			}
			else
			{
				return InOpArgs._Content.Widget;
			}
		}
	} DetermineContent;

	SBorder::Construct(SBorder::FArguments()
		.ContentScale(InArgs._ContentScale)
		.DesiredSizeScale(InArgs._DesiredSizeScale)
		.BorderBackgroundColor(InArgs._ButtonColorAndOpacity)
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		[
			DetermineContent(InArgs)
		]
	);

	SetContentPadding(InArgs._ContentPadding);
	SetButtonStyle(InArgs._ButtonStyle);

	// Only do this if we're exactly an SMapIcon
	if (GetType() == SMapIconTypeName)
	{
		SetCanTick(false);
	}
}

int32 SMapIcon::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	bool bEnabled = ShouldBeEnabled(bParentEnabled);
	bool bShowDisabledEffect = GetShowDisabledEffect();

	const FSlateBrush* BrushResource = !bShowDisabledEffect && !bEnabled ? &Style->Disabled : GetBorderImage();

	ESlateDrawEffect DrawEffects = bShowDisabledEffect && !bEnabled ? ESlateDrawEffect::DisabledEffect : ESlateDrawEffect::None;

	if (BrushResource && BrushResource->DrawAs != ESlateBrushDrawType::NoDrawType)
	{
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			BrushResource,
			DrawEffects,
			BrushResource->GetTint(InWidgetStyle) * InWidgetStyle.GetColorAndOpacityTint() * GetBorderBackgroundColor().GetColor(InWidgetStyle)
		);
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bEnabled);
}

FMargin SMapIcon::GetCombinedPadding() const
{
	return (IsSelected())
		? ContentPaddingAttribute.Get() + Style->PressedPadding
		: ContentPaddingAttribute.Get() + Style->NormalPadding;
}

//~ Update when { ContentPaddingAttribute, IsSelected, Style }
void SMapIcon::UpdatePadding()
{
	SetPadding(GetCombinedPadding());
}

bool SMapIcon::GetShowDisabledEffect() const
{
	return Style->Disabled.DrawAs == ESlateBrushDrawType::NoDrawType;
}

//~ Update when { Style }
void SMapIcon::UpdateShowDisabledEffect()
{
	// Needs to be called when the style changed
	SetShowEffectWhenDisabled(GetShowDisabledEffect());
}

//~ Update when { GetShowDisabledEffect(Style), IsEnable(EnabledState), Pressed, Hovered, Style }
void SMapIcon::UpdateBorderImage()
{
	if (!GetShowDisabledEffect() && !IsInteractable())
	{
		SetBorderImage(&Style->Disabled);
	}
	else if (IsSelected())
	{
		SetBorderImage(&Style->Pressed);
	}
	else if (IsHovered())
	{
		SetBorderImage(&Style->Hovered);
	}
	else
	{
		SetBorderImage(&Style->Normal);
	}
}

//~ Update when { DefaultForegroundColorAttribute, Pressed, Hovered, Style }
void SMapIcon::UpdateForegroundColor()
{
	if (BorderForegroundColorAttribute.Get() == FSlateColor::UseStyle())
	{
		if (IsSelected())
		{
			SetForegroundColor(Style->PressedForeground);
		}
		else if (IsHovered())
		{
			SetForegroundColor(Style->HoveredForeground);
		}
		else
		{
			SetForegroundColor(Style->NormalForeground);
		}
	}
	else
	{
		SetForegroundColor(BorderForegroundColorAttribute.Get());
	}
}

//~ Update when { Style }
void SMapIcon::UpdateDisabledForegroundColor()
{
	Invalidate(EInvalidateWidgetReason::Paint);
}

bool SMapIcon::SupportsKeyboardFocus() const
{
	// Buttons are focusable by default
	return bIsFocusable;
}

void SMapIcon::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SBorder::OnFocusLost(InFocusEvent);

	Release();
}

FReply SMapIcon::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		Press();

		if (PressMethod == EButtonPressMethod::ButtonPress)
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}
	else
	{
		Reply = SBorder::OnKeyDown(MyGeometry, InKeyEvent);
	}

	//return the constructed reply
	return Reply;
}

FReply SMapIcon::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = FReply::Unhandled();

	if (IsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		const bool bWasPressed = bIsPressed;

		Release();

		//@Todo Slate: This should check focus, however we don't have that API yet, will be easier when focus is unified.
		if (PressMethod == EButtonPressMethod::ButtonRelease || (PressMethod == EButtonPressMethod::DownAndUp && bWasPressed))
		{
			//execute our "OnClicked" delegate, and get the reply
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else
		{
			Reply = FReply::Handled();
		}
	}

	//return the constructed reply
	return Reply;
}

FReply SMapIcon::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsEnabled() && (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent()))
	{
		Press();
		PressedScreenSpacePosition = MouseEvent.GetScreenSpacePosition();

		EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);

		if (InputClickMethod == EButtonClickMethod::MouseDown)
		{
			//get the reply from the execute function
			Reply = ExecuteOnClick();

			//You should ALWAYS handle the OnClicked event.
			ensure(Reply.IsEventHandled() == true);
		}
		else if (InputClickMethod == EButtonClickMethod::PreciseClick)
		{
			// do not capture the pointer for precise taps or clicks
			// 
			Reply = FReply::Handled();
		}
		else
		{
			//we need to capture the mouse for MouseUp events
			Reply = FReply::Handled().CaptureMouse(AsShared());
		}
	}

	//return the constructed reply
	return Reply;
}

FReply SMapIcon::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = SBorder::OnMouseButtonDoubleClick(MyGeometry, MouseEvent);
	if (Reply.IsEventHandled())
	{
		return Reply;
	}

	// We didn't handle the double click, treat it as single click
	return OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SMapIcon::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	const EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);
	const bool bMustBePressed = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));

	if (bMeetsPressedRequirements && ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())))
	{
		Release();

		if (IsEnabled())
		{
			if (InputClickMethod == EButtonClickMethod::MouseDown)
			{
				// NOTE: If we're configured to click on mouse-down/precise-tap, then we never capture the mouse thus
				//       may never receive an OnMouseButtonUp() call.  We make sure that our bIsPressed
				//       state is reset by overriding OnMouseLeave().
			}
			else
			{
				bool bEventOverButton = IsHovered();

				if (!bEventOverButton && MouseEvent.IsTouchEvent())
				{
					bEventOverButton = MyGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition());
				}

				if (bEventOverButton)
				{
					// If we asked for a precise tap, all we need is for the user to have not moved their pointer very far.
					const bool bTriggerForTouchEvent = InputClickMethod == EButtonClickMethod::PreciseClick;

					// If we were asked to allow the button to be clicked on mouse up, regardless of whether the user
					// pressed the button down first, then we'll allow the click to proceed without an active capture
					const bool bTriggerForMouseEvent = (InputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

					if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						Reply = ExecuteOnClick();
					}
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}

	//If the user hasn't requested a new mouse captor and the button still has mouse capture,
	//then the default behavior of the button is to release mouse capture.
	if (Reply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		Reply.ReleaseMouseCapture();
	}

	return Reply;
}

FReply SMapIcon::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsPressed() && IsPreciseTapOrClick(MouseEvent) && FSlateApplication::Get().HasTraveledFarEnoughToTriggerDrag(MouseEvent, PressedScreenSpacePosition))
	{
		Release();
	}

	return FReply::Unhandled();
}

void SMapIcon::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();

	SBorder::OnMouseEnter(MyGeometry, MouseEvent);

	if (!bWasHovered && IsHovered())
	{
		ExecuteHoverStateChanged(true);
	}
}

void SMapIcon::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	const bool bWasHovered = IsHovered();

	// Call parent implementation
	SBorder::OnMouseLeave(MouseEvent);

	// If we're setup to click on mouse-down, then we never capture the mouse and may not receive a
	// mouse up event, so we need to make sure our pressed state is reset properly here
	if (ClickMethod == EButtonClickMethod::MouseDown || IsPreciseTapOrClick(MouseEvent))
	{
		Release();
	}

	if (bWasHovered && !IsHovered())
	{
		ExecuteHoverStateChanged(true);
	}
}

void SMapIcon::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Release();
}

FReply SMapIcon::ExecuteOnClick()
{
	if (OnClicked.IsBound())
	{
		FReply Reply = OnClicked.Execute();
#if WITH_ACCESSIBILITY
		FSlateApplicationBase::Get().GetAccessibleMessageHandler()->OnWidgetEventRaised(AsShared(), EAccessibleEvent::Activate);
#endif
		return Reply;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnClicked Not Bound");
		return FReply::Handled();
	}
}

void SMapIcon::Select()
{
	if (!bIsSelected)
	{
		bIsSelected = true;
		if (OnSelected.IsBound())
		{
			OnSelected.Execute();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "SMapIcon::Select");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnSelected Not Bound");
		}
		UpdatePressStateChanged();
	}
}

void SMapIcon::Deselect()
{
	if (bIsSelected)
	{
		bIsSelected = false;
		if (OnDeselected.IsBound())
		{
			OnDeselected.Execute();
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "SMapIcon::Deselect");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "OnDeselected Not Bound");
		}
		UpdatePressStateChanged();
	}
}

void SMapIcon::Press()
{
	if (!bIsPressed)
	{
		bIsPressed = true;
		PlayPressedSound();
		OnPressed.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SMapIcon::Release()
{
	if (bIsPressed)
	{
		bIsPressed = false;
		OnReleased.ExecuteIfBound();
		UpdatePressStateChanged();
	}
}

void SMapIcon::UpdatePressStateChanged()
{
	UpdatePadding();
	UpdateBorderImage();
	UpdateForegroundColor();
}

bool SMapIcon::IsInteractable() const
{
	return IsEnabled();
}

TEnumAsByte<EButtonClickMethod::Type> SMapIcon::GetClickMethodFromInputType(const FPointerEvent& MouseEvent) const
{
	if (MouseEvent.IsTouchEvent())
	{
		switch (TouchMethod)
		{
		case EButtonTouchMethod::Down:
			return EButtonClickMethod::MouseDown;
		case EButtonTouchMethod::DownAndUp:
			return EButtonClickMethod::DownAndUp;
		case EButtonTouchMethod::PreciseTap:
			return EButtonClickMethod::PreciseClick;
		}
	}

	return ClickMethod;
}

bool SMapIcon::IsPreciseTapOrClick(const FPointerEvent& MouseEvent) const
{
	return GetClickMethodFromInputType(MouseEvent) == EButtonClickMethod::PreciseClick;
}

void SMapIcon::PlayPressedSound() const
{
	FSlateApplication::Get().PlaySound(PressedSound);
}

void SMapIcon::PlayHoverSound() const
{
	FSlateApplication::Get().PlaySound(HoveredSound);
}

FVector2D SMapIcon::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	// When there is no widget in the button, it sizes itself based on
	// the border image specified by the style.
	if (ChildSlot.GetWidget() == SNullWidget::NullWidget)
	{
		return GetBorderImage()->ImageSize;
	}
	else
	{
		return SBorder::ComputeDesiredSize(LayoutScaleMultiplier);
	}
}

void SMapIcon::SetContentPadding(TAttribute<FMargin> InContentPadding)
{
	ContentPaddingAttribute.Assign(*this, MoveTemp(InContentPadding));
}

void SMapIcon::SetHoveredSound(TOptional<FSlateSound> InHoveredSound)
{
	HoveredSound = InHoveredSound.Get(Style->HoveredSlateSound);
}

void SMapIcon::SetPressedSound(TOptional<FSlateSound> InPressedSound)
{
	PressedSound = InPressedSound.Get(Style->PressedSlateSound);
}

void SMapIcon::SetOnClicked(FOnClicked InOnClicked)
{
	OnClicked = InOnClicked;
}

void SMapIcon::SetOnHovered(FSimpleDelegate InOnHovered)
{
	OnHovered = InOnHovered;
}

void SMapIcon::SetOnUnhovered(FSimpleDelegate InOnUnhovered)
{
	OnUnhovered = InOnUnhovered;
}

void SMapIcon::ExecuteHoverStateChanged(bool bPlaySound)
{
	if (IsHovered())
	{
		if (bPlaySound)
		{
			PlayHoverSound();
		}
		OnHovered.ExecuteIfBound();
	}
	else
	{
		OnUnhovered.ExecuteIfBound();
	}
}

void SMapIcon::SetButtonStyle(const FButtonStyle* InButtonStyle)
{
	if (InButtonStyle == nullptr)
	{
		ensureAlwaysMsgf(false, TEXT("The Style is not valid."));
		return;
	}

	/* Get pointer to the button Style */
	Style = InButtonStyle;

	HoveredSound = Style->HoveredSlateSound;
	PressedSound = Style->PressedSlateSound;

	UpdatePadding();
	UpdateShowDisabledEffect();
	UpdateBorderImage(); // Must be after UpdateShowDisabledEffect()
	UpdateForegroundColor();
	UpdateDisabledForegroundColor();
}

void SMapIcon::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
}

void SMapIcon::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
}

void SMapIcon::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
}

#if !UE_BUILD_SHIPPING
void SMapIcon::SimulateClick()
{
	ExecuteOnClick();
}
#endif // !UE_BUILD_SHIPPING

FSlateColor SMapIcon::GetDisabledForegroundColor() const
{
	return Style->DisabledForeground;
}