// Fill out your copyright notice in the Description page of Project Settings.


#include "BallDetailBase.h"

void UBallDetailBase::NativeConstruct()
{
	Super::NativeConstruct();

	mBallPowerBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallPowerBar"))));
	mBallHeightBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallHeightBar"))));
}

void UBallDetailBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBallDetailBase::SetBallPower(float ratio)
{
	mBallPowerBar->SetPercent(ratio);
}

void UBallDetailBase::SetBallHeight(float ratio)
{
	mBallHeightBar->SetPercent(ratio);
}
