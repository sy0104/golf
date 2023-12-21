// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/Image.h>
#include <Components/CanvasPanelSlot.h>
#include <Components/CanvasPanel.h>
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

private:
	UImage* mMiniMapImage;
	UImage* mCurrentBallImage;
	UImage* mTargetBallImage;
	UImage* mFlagImage;

	UCanvasPanel* mCurrentBallCanvas;
	UCanvasPanel* mTargetBallCanvas;
	UCanvasPanel* mFlagCanvas;
	UCanvasPanel* mMiniMapCanvas;

	FVector2D mWorldStartPos;
	FVector2D mWorldDestPos;
	FVector2D mLocalStartPos;
	FVector2D mLocalDestPos;
	FVector2D mCurrentPos;

public:
	void SetBallCurrent(FVector2D position);
	void SetBallTarget(FVector2D direction, EGolfClub club);
	//void SetHoleImage(FVector2D position);
	void SetInfo(FVector2D startPos, FVector2D destPos);

};
