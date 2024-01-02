#pragma once

#include <Components\ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallDetailBase.generated.h"

UCLASS()
class GOLF_API UBallDetailBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UProgressBar*	mBallPowerBar;

	UTextBlock* mDistanceText;
	UTextBlock* mHalfDistanceText;

	UImage* mHoleMarkImage;

public:
	void SetBallPower(float ratio);
	void SetBallDistance(EGolfClub club);
	void SetHoleMark(FVector ballPos, FVector destPos);

private:
	double progressDis;
};
