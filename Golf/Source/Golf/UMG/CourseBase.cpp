#include "CourseBase.h"

void UCourseBase::NativeConstruct()
{
	Super::NativeConstruct();

	mCourseText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CourseText"))));

	mBorderImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("BorderImage"))));
	mFairwayImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("FairwayImage"))));
	mRoughImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("RoughImage"))));
	mBunkerImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("BunkerImage"))));
}

void UCourseBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCourseBase::SetCourseText(FString CourseText)
{
	mCourseText->SetText(FText::FromString(CourseText));
}

void UCourseBase::SetCourseImage(bool visible, EMaterialType CourseImage)
{
	mBorderImage->SetVisibility(ESlateVisibility::Hidden);
	mFairwayImage->SetVisibility(ESlateVisibility::Hidden);
	mRoughImage->SetVisibility(ESlateVisibility::Hidden);
	mBunkerImage->SetVisibility(ESlateVisibility::Hidden);

	if (visible)
	{
		mBorderImage->SetVisibility(ESlateVisibility::Visible);

		if (CourseImage == EMaterialType::Rough)
		{
			mRoughImage->SetVisibility(ESlateVisibility::Visible);
		}
		else if (CourseImage == EMaterialType::Bunker)
		{
			mBunkerImage->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			mFairwayImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
