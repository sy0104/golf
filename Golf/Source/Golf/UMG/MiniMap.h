// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/Image.h>
#include "../GameInfo.h"
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
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;


private:
	UImage* mCurrentBallImage;
	UImage* mTargetBallImage;
	UImage* mHoleImage;

	FVector2D mCurrentPos;
	FVector2D mTargetPos;
	FVector2D mHolePos;

	FVector2D mCurrentBallSize;
	FVector2D mTargetBallSize;
	FVector2D mHoleSize;


public:
	void SetBallCurrent(FVector2D position);
	void SetBallTarget(FVector2D position, FVector2D direction, EGolfClub club);
	void SetHoleImage(FVector2D position);
};
