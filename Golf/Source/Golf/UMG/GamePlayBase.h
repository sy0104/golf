#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "GamePlayBase.generated.h"

UCLASS()
class GOLF_API UGamePlayBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

public:
	UFUNCTION()
	void OnNextButtonClicked();

	UFUNCTION()
	void OnHomeButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

private:
	UButton*	mNextButton;
	UButton*	mHomeButton;
	UButton*	mExitButton;
};
