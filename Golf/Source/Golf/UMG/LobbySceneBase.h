#pragma once

#include <Components\Image.h>
#include <Components\Button.h>
#include <Components\TextBlock.h>

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
	UImage*			mPlayer2BackImage;
	UImage*			mPlayer2Image;
	UTextBlock*		mPlayer2Text;

	UButton*		mAddPlayer2Button;
	UButton*		mDeletePlayer2Button;

	UButton*		mStartButton;

	EPlayType		mPlayType;

public:
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnAddPlayer2ButtonClicked();

	UFUNCTION()
	void OnDeletePlayer2ButtonClicked();
};
