#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "LobbySceneBase.generated.h"

UCLASS()
class GOLF_API ULobbySceneBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UButton*		mStartButton;

public:
	UFUNCTION()
	void OnStartButtonClicked();
};
