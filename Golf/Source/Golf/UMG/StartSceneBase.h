// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "StartSceneBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLF_API UStartSceneBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UButton*	mStartButton;

public:
	UFUNCTION()
	void OnStartButtonClicked();
};
