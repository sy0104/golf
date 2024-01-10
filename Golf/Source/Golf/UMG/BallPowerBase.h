#pragma once

#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Components/Image.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallPowerBase.generated.h"

UCLASS()
class GOLF_API UBallPowerBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UProgressBar* mBallPowerBar;

	UTextBlock* mDistanceText;
	UTextBlock* mHalfDistanceText;

	UImage* mHoleMarkImage;

public:
	void SetBallPower(float ratio);
	void SetBallDistance(EGolfClub club, EIronType ironClub);
	void SetHoleMark(FVector ballPos, FVector destPos);

private:
	double progressDis;

};
