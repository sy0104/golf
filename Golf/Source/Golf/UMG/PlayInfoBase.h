#pragma once

#include <Components/Image.h>
#include <Components/TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayInfoBase.generated.h"


UCLASS()
class GOLF_API UPlayInfoBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);


private:
	UImage* mPlayerImage;

	UTextBlock* mPlayerNameText;
	UTextBlock* mCourseDistanceText;
	UTextBlock* mTargetDistanceText;
	UTextBlock* mShotNumText;
	UTextBlock* mScoreText;

public:
	void SetPlayerImage(const FString& path);

	void SetPlayerNameText(FString name);
	void SetPlayerTargetDistanceText(float dis);
	void SetPlayerShotNumText(int32 shot);
	void SetPlayerScoreText(int32 score);
};
