#pragma once

#include <Components/Button.h>
#include <Components/ProgressBar.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallSpinBase.generated.h"

UCLASS()
class GOLF_API UBallSpinBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UButton*		mSwingLeftButton;
	UButton*		mSwingStraightButton;
	UButton*		mSwingRightButton;

	UProgressBar*	mBallSpinPowerBar;

public:
	UFUNCTION()
	void OnSwingLeftButtonClicked();

	UFUNCTION()
	void OnSwingStraightButtonClicked();

	UFUNCTION()
	void OnSwingRightButtonClicked();

public:
	void SetBallSpin(float ratio);
	void SetSpinButtonsEnable();
};
