#pragma once

#include <Components/CheckBox.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallDetailBase.generated.h"

UCLASS()
class GOLF_API UBallDetailBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UCheckBox*	mPowerCheckBox;
	UCheckBox*	mSpinCheckBox;

public:
	UFUNCTION()
	void OnPowerCheckBoxChanged(bool IsChecked);

	UFUNCTION()
	void OnSpinCheckBoxChanged(bool IsChecked);

public:
	void SetPowerCheckBoxChecked(bool IsChecked);
	void SetSpinCheckBoxChecked(bool IsChecked);
};
