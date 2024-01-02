#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallSpinBase.generated.h"

UCLASS()
class GOLF_API UBallSpinBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UButton*		mSwingLeftButton;
	UButton*		mSwingStraightButton;
	UButton*		mSwingRightButton;

public:
	UFUNCTION()
	void OnSwingLeftButtonClicked();

	UFUNCTION()
	void OnSwingStraightButtonClicked();

	UFUNCTION()
	void OnSwingRightButtonClicked();
};
