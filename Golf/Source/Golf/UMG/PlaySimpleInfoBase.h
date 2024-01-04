#pragma once

#include <Components\Image.h>
#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlaySimpleInfoBase.generated.h"

UCLASS()
class GOLF_API UPlaySimpleInfoBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();


private:
	UImage*			mPlayerImage;

	UTextBlock*		mPlayerNameText;
	UTextBlock*		mShotNumText;
	UTextBlock*		mScoreText;
	UTextBlock*		mTargetDistanceText;

public:
	void SetPlayerImage(const FString& path);
	void SetPlayerNameText(FString name);
	void SetPlayerShotNumText(int ShotNum);
	void SetPlayerScoreText(int score);
	void SetPlayerTargetDistanceText(float dis);
};
