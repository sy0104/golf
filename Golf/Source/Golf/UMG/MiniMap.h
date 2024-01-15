#pragma once

#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/TextBlock.h>
#include "../GameInfo.h"
#include <Blueprint/UserWidget.h>
#include "MiniMap.generated.h"

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

	FVector mCurrentPos;
	FVector mTargetPos;
	FVector mHolePos;

	FVector2D mCurrentBallSize;
	FVector2D mTargetBallSize;
	FVector2D mHoleSize;

	UCanvasPanel* mDistanceCanvas;
	UCanvasPanel* mLeftDistanceCanvas;
	
	UTextBlock* mDistanceText;
	UTextBlock* mLeftDistanceText;

	FVector2D mDistanceCanvasSize;
	FVector2D mLeftDistanceCanvasSize;


public:
	void SetBallCurrent(FVector position);
	void SetBallTarget(FVector position, FVector direction, double targetDis);
	void SetHoleImage(FVector position);
	void SetTargetDistanceText(float dis);
	void SetHoleDistanceText(float dis);
};
