#pragma once

#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "HoleInfoBase.generated.h"

UCLASS()
class GOLF_API UHoleInfoBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UTextBlock*		mHoleNumText;

public:
	void SetHoleNumText(int num);
};
