#pragma once

#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "WindBase.generated.h"


UCLASS()
class GOLF_API UWindBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock*			mLeftWindText;
	UTextBlock*			mRightWindText;
	UTextBlock*			mForwardWindText;
	UTextBlock*			mBackWindText;

	UTextBlock*			mWindVelText;

public:
	void SetLeftWindTextVisible(bool visible);
	void SetRightWindTextVisible(bool visible);
	void SetForwardWindTextVisible(bool visible);
	void SetBackWindTextVisible(bool visible);
	void SetWindVelText(float vel);
};
