#pragma once

#include <Components/Image.h>
#include <Components/CanvasPanel.h>
#include <Components/TextBlock.h>
#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "BallInfoBase.generated.h"

UCLASS()
class GOLF_API UBallInfoBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UCanvasPanel* mDistanceCanvas;
	UTextBlock* mDistanceText;

	FVector mballPos;
	FVector mdestPos;

	FVector2D mDistanceCanvasSize;

public:
	void SetPuttingInfo(FVector ballPos, FVector destPos);
	
	
};
