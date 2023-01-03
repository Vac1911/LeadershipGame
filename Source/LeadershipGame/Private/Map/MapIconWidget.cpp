// Copyright Epic Games, Inc. All Rights Reserved.

#include "Map/MapIconWidget.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Map/SMapIcon.h"
#include "Components/ButtonSlot.h"
#include "Styling/UMGCoreStyle.h"
#include "Blueprint/WidgetTree.h"

#define LOCTEXT_NAMESPACE "UMG"

/////////////////////////////////////////////////////
// UMapIconWidget

static FButtonStyle* DefaultButtonStyle = nullptr;

#if WITH_EDITOR
static FButtonStyle* EditorButtonStyle = nullptr;
#endif 

UMapIconWidget::UMapIconWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	if (DefaultButtonStyle == nullptr)
	{
		DefaultButtonStyle = new FButtonStyle(FUMGCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button"));

		// Unlink UMG default colors.
		DefaultButtonStyle->UnlinkColors();
	}

	WidgetStyle = *DefaultButtonStyle;

#if WITH_EDITOR 
	if (EditorButtonStyle == nullptr)
	{
		EditorButtonStyle = new FButtonStyle(FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("EditorUtilityButton"));

		// Unlink UMG Editor colors from the editor settings colors.
		EditorButtonStyle->UnlinkColors();
	}

	if (IsEditorWidget())
	{
		WidgetStyle = *EditorButtonStyle;

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif // WITH_EDITOR

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;

#if WITH_EDITORONLY_DATA
	AccessibleBehavior = ESlateAccessibleBehavior::Summary;
	bCanChildrenBeAccessible = false;
#endif
}

UMapIconWidget* UMapIconWidget::CreateIcon(UUserWidget* InWidget, UMapWidget* MapWidget, FVector2D MapPosition)
{
	if ((!InWidget) || !MapWidget)
	{
		return nullptr;
	}

	UMapIconWidget* CreatedWidget = UMapIconWidget::ConstructIcon<UMapIconWidget>(InWidget, MapWidget);
	CreatedWidget->SetMapPoint(MapPosition);

	return CreatedWidget;
}


void UMapIconWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyMapIcon.Reset();
}

TSharedRef<SWidget> UMapIconWidget::RebuildWidget()
{
	MyMapIcon = SNew(SMapIcon)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnSelected(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleSelected))
		.OnDeselected(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleDeselected))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		//Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyMapIcon.ToSharedRef());
	}

	return MyMapIcon.ToSharedRef();
}

void UMapIconWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyMapIcon->SetColorAndOpacity(ColorAndOpacity);
	MyMapIcon->SetBorderBackgroundColor(BackgroundColor);
}

UClass* UMapIconWidget::GetSlotClass() const
{
	return UButtonSlot::StaticClass();
}

void UMapIconWidget::OnSlotAdded(UPanelSlot* InSlot)
{
	// Add the child to the live slot if it already exists
	if (MyMapIcon.IsValid())
	{
		//CastChecked<UButtonSlot>(InSlot)->BuildSlot(MyMapIcon.ToSharedRef());
	}
}

void UMapIconWidget::OnSlotRemoved(UPanelSlot* InSlot)
{
	// Remove the widget from the live slot if it exists.
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetContent(SNullWidget::NullWidget);
	}
}

void UMapIconWidget::SetMapPoint(FVector2D NextMapPoint)
{
	MapPoint = NextMapPoint;
	bool DidHit;
	WorldLocation = ParentMap->MapToWorldPoint(MapPoint, DidHit);

	RequestUpdate();
}

void UMapIconWidget::SetScreenPosition(FVector2D NextScreenPosition)
{
	SetMapPoint(ParentMap->ScreenToMapPoint(NextScreenPosition));
}

void UMapIconWidget::RequestUpdate()
{
	IsPendingUpdate = true;
}

bool UMapIconWidget::ShouldUpdatePosition()
{
	return IsPendingUpdate;
}

void UMapIconWidget::UpdatePosition()
{
	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot.Get());
	if (CanvasSlot)
	{
		FGeometry CanvasGeometry = CanvasSlot->Parent->GetCachedGeometry();
		FVector2D ScreenPoint = ParentMap->MapToScreenPoint(MapPoint);
		FVector2D LocalCoordinate = CanvasGeometry.AbsoluteToLocal(ScreenPoint);

		CanvasSlot->SetSize(FVector2D(16.f));
		CanvasSlot->SetAlignment(FVector2D(0.5f));
		CanvasSlot->SetAnchors(FAnchors(0.0f));
		CanvasSlot->SetPosition(LocalCoordinate);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Icon CanvasSlot not found");
	}
	IsPendingUpdate = false;
}

void UMapIconWidget::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetButtonStyle(&WidgetStyle);
	}
}

void UMapIconWidget::SetColorAndOpacity(FLinearColor InColorAndOpacity)
{
	ColorAndOpacity = InColorAndOpacity;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetColorAndOpacity(InColorAndOpacity);
	}
}

void UMapIconWidget::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool UMapIconWidget::IsPressed() const
{
	if (MyMapIcon.IsValid())
	{
		return MyMapIcon->IsPressed();
	}

	return false;
}

void UMapIconWidget::Select() const
{
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->Select();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Icon Invalid");
	}
}

void UMapIconWidget::Deselect() const
{
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->Deselect();
	}
}

void UMapIconWidget::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetClickMethod(ClickMethod);
	}
}

void UMapIconWidget::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetTouchMethod(TouchMethod);
	}
}

void UMapIconWidget::SetPressMethod(EButtonPressMethod::Type InPressMethod)
{
	PressMethod = InPressMethod;
	if (MyMapIcon.IsValid())
	{
		MyMapIcon->SetPressMethod(PressMethod);
	}
}

void UMapIconWidget::PostLoad()
{
	Super::PostLoad();

	if (GetChildrenCount() > 0)
	{
		//TODO UMG Pre-Release Upgrade, now buttons have slots of their own.  Convert existing slot to new slot.
		if (UPanelSlot* PanelSlot = GetContentSlot())
		{
			UButtonSlot* ButtonSlot = Cast<UButtonSlot>(PanelSlot);
			if (ButtonSlot == NULL)
			{
				ButtonSlot = NewObject<UButtonSlot>(this);
				ButtonSlot->Content = GetContentSlot()->Content;
				ButtonSlot->Content->Slot = ButtonSlot;
				Slots[0] = ButtonSlot;
			}
		}
	}

	if (GetLinkerUEVersion() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS && Style_DEPRECATED != nullptr)
	{
		const FButtonStyle* StylePtr = Style_DEPRECATED->GetStyle<FButtonStyle>();
		if (StylePtr != nullptr)
		{
			WidgetStyle = *StylePtr;
		}

		Style_DEPRECATED = nullptr;
	}
}

FReply UMapIconWidget::SlateHandleClicked()
{
	OnClicked.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "SlateHandleClicked");

	if (MyMapIcon->IsSelected())
	{
		Deselect();
	}
	else
	{
		Select();
	}

	return FReply::Handled();
}

void UMapIconWidget::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void UMapIconWidget::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void UMapIconWidget::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UMapIconWidget::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}

void UMapIconWidget::SlateHandleSelected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "SlateHandleSelected");
	ParentMap->SelectedIcons.Add(this);
	OnSelected.Broadcast();
}

void UMapIconWidget::SlateHandleDeselected()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "SlateHandleDeselected");
	ParentMap->SelectedIcons.Remove(this);
	OnDeselected.Broadcast();
}
#if WITH_EDITOR

const FText UMapIconWidget::GetPaletteCategory()
{
	return LOCTEXT("Common", "Common");
}

#endif

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
