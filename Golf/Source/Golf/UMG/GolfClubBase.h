// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components\Button.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "GolfClubBase.generated.h"


UCLASS()
class GOLF_API UGolfClubBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	UButton*		mClub1Button;
	UButton*		mClub2Button;
	UButton*		mClub3Button;

public:
	UFUNCTION()
	void OnClub1ButtonClicked();

	UFUNCTION()
	void OnClub2ButtonClicked();

	UFUNCTION()
	void OnClub3ButtonClicked();
};
