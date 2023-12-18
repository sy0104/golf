// Fill out your copyright notice in the Description page of Project Settings.

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


};
