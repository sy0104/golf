#pragma once

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "GoodShotBase.generated.h"

UCLASS()
class GOLF_API UGoodShotBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct();

};
