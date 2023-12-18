// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/Image.h>

#include <Blueprint/UserWidget.h>
#include "MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class GOLF_API UMiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UImage* mCurrentBallImage;
	UImage* mTargetBallImage;
	UImage* mFlagImage;


public:

	void SetBallCurrent(/*FVector worldLocation*/);
	void SetBallTarget(/*FVector worldLocation*/);
	//void SetHoleImage(FVector worldLocation);


	
};
