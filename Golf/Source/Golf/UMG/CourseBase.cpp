#include "CourseBase.h"

void UCourseBase::NativeConstruct()
{
	Super::NativeConstruct();

	mCourseText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("CourseText"))));
}

void UCourseBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCourseBase::SetCourseText(FString CourseText)
{
	mCourseText->SetText(FText::FromString(CourseText));
}
