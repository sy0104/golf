// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "PlayInfoBase.generated.h"


UCLASS()
class GOLF_API UPlayInfoBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock* mCourseDistanceText;
	UTextBlock* mTargetDistanceText;
	UTextBlock* mShotNumText;
	UTextBlock* mScoreText;

public:
	void SetCourseDistanceText(float dis);
	void SetTargetDistanceText(float dis);
	void SetShotNumText(int32 shot);
	void SetScoreText();

};
