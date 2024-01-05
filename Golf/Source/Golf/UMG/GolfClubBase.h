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
	UButton*		mDriverButton;
	UButton*		mWoodButton;
	UButton*		mIronButton;
	UButton*		mWedgeButton;
	UButton*		mPutterButton;

public:
	UFUNCTION()
	void OnDriverButtonClicked();

	UFUNCTION()
	void OnWoodButtonClicked();

	UFUNCTION()
	void OnIronButtonClicked();

	UFUNCTION()
	void OnWedgeButtonClicked();

	UFUNCTION()
	void OnPutterButtonClicked();

public:
	void SetPuttingClub(bool isPutting);
};
