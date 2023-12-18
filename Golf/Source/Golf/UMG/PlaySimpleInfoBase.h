// Fill out your copyright notice in the Description page of Project Settings.

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

public:
	void SetPlayerImage(const FString& path);
	void SetPlayerNameText(FString name);
	void SetShotNumText(int ShotNum);
	void SetScoreText(int score);
};
