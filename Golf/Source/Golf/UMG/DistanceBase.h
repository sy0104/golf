// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\TextBlock.h>


#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "DistanceBase.generated.h"

UCLASS()
class GOLF_API UDistanceBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock*		mLeftDistanceText;
	UTextBlock*		mDistanceText;

public:
	void SetLeftDistanceText(float dis);
	void SetDistanceText(float dis);
};
