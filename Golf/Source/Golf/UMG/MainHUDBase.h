// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint\UserWidget.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDBase.generated.h"

UCLASS()
class GOLF_API UMainHUDBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

private:
	class UDistanceBase*	mDistanceBase;
	class UBallDetailBase*	mBallDetailBase;
	class UGolfClubBase*	mGolfClubBase;
	
public:
	class UDistanceBase* GetDistanceBase()
	{
		return mDistanceBase;
	}

	// Distance
	void SetLeftDistanceText(float dis);
	void SetDistanceText(float dis);

	// Ball Detail
	void SetBallPower(float ratio);
	void SetBallDir(float ratio);
};
