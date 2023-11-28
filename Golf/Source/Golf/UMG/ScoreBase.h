// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\TextBlock.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "ScoreBase.generated.h"

UCLASS()
class GOLF_API UScoreBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UTextBlock* mScoreText;

public:
	void SetScoreText(FString ScoreText);
};
