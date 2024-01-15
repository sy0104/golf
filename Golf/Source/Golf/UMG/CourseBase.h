#pragma once

#include <Components\TextBlock.h>
#include <Components/Image.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "CourseBase.generated.h"

UCLASS()
class GOLF_API UCourseBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock*		mCourseText;

	UImage* mBorderImage;
	UImage* mFairwayImage;
	UImage* mRoughImage;
	UImage* mBunkerImage;

public:
	void SetCourseText(FString CourseText);
	void SetCourseImage(bool visible, EMaterialType CourseImage);
};
