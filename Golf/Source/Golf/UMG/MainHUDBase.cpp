// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "DistanceBase.h"
#include "BallDetailBase.h"
#include "GolfClubBase.h"
#include "ScoreBase.h"
#include "BallStateBase.h"
#include "CourseBase.h"
#include "BallSpinBase.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceBase = Cast<UDistanceBase>(GetWidgetFromName(FName(TEXT("DistanceUI"))));
	mBallDetailBase = Cast<UBallDetailBase>(GetWidgetFromName(FName(TEXT("BallDetailUI"))));
	mGolfClubBase = Cast<UGolfClubBase>(GetWidgetFromName(FName(TEXT("GolfClubUI"))));
	mScoreBase = Cast<UScoreBase>(GetWidgetFromName(FName(TEXT("ScoreUI"))));
	mBallStateBase = Cast<UBallStateBase>(GetWidgetFromName(FName(TEXT("BallStateUI"))));
	mCourseBase = Cast<UCourseBase>(GetWidgetFromName(FName(TEXT("CourseUI"))));
	mBallSpinBase = Cast<UBallSpinBase>(GetWidgetFromName(FName(TEXT("BallSpinUI"))));
}

void UMainHUDBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMainHUDBase::SetLeftDistanceText(float dis)
{
	mDistanceBase->SetLeftDistanceText(dis);
}

void UMainHUDBase::SetDistanceText(float dis)
{
	mDistanceBase->SetDistanceText(dis);
}

void UMainHUDBase::SetBallPower(float ratio)
{
	mBallDetailBase->SetBallPower(ratio);
}

void UMainHUDBase::SetScoreText(FString ScoreText)
{
	mScoreBase->SetVisibility(ESlateVisibility::Visible);
	mScoreBase->SetScoreText(ScoreText);
}

void UMainHUDBase::SetConcedeTextVisible(bool visible)
{
	if (visible)
		mScoreBase->SetConcedeTextVisible(true);
	else
		mScoreBase->SetConcedeTextVisible(false);
}

void UMainHUDBase::SetBallStateText(FString StateText)
{
	mBallStateBase->SetBallStateText(StateText);
}

void UMainHUDBase::SetBallStateVisible(bool visible)
{
	if (visible)
		mBallStateBase->SetVisibility(ESlateVisibility::Visible);
	else
		mBallStateBase->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::SetCourseText(FString CourseText)
{
	mCourseBase->SetCourseText(CourseText);
}
