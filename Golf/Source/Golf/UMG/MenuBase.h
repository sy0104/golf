#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

UCLASS()
class GOLF_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UButton*		mHomeButton;
	UButton*		mExitButton;

public:
	UFUNCTION()
	void OnHomeButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();
};
