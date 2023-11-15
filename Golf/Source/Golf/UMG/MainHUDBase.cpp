// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "DistanceBase.h"
#include "BallDetailBase.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceBase = Cast<UDistanceBase>(GetWidgetFromName(FName(TEXT("DistanceUI"))));
	mBallDetailBase = Cast<UBallDetailBase>(GetWidgetFromName(FName(TEXT("BallDetailUI"))));
}

void UMainHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUDBase::SetDistanceText(float dis)
{
	mDistanceBase->SetDistanceText(dis);
}

void UMainHUDBase::SetBallPower(float ratio)
{
	mBallDetailBase->SetBallPower(ratio);
}
