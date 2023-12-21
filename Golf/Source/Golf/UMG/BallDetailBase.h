#pragma once

#include <Components\ProgressBar.h>

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

public:
	void SetBallPower(float ratio);
};
