// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "DistanceBase.h"
#include "BallDetailBase.h"
#include "GolfClubBase.h"
#include "ScoreBase.h"
#include "BallStateBase.h"
#include "CourseBase.h"
#include "BallSpinBase.h"
#include "WindBase.h"
#include "HoleInfoBase.h"
#include "PlayInfoBase.h"
#include "PlaySimpleInfoBase.h"

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
	mWindBase = Cast<UWindBase>(GetWidgetFromName(FName(TEXT("WindUI"))));
	mHoleInfoBase = Cast<UHoleInfoBase>(GetWidgetFromName(FName(TEXT("HoleInfoUI"))));
	mPlayInfoBase = Cast<UPlayInfoBase>(GetWidgetFromName(FName(TEXT("PlayInfoUI"))));
	mPlaySimpleInfoBase = Cast<UPlaySimpleInfoBase>(GetWidgetFromName(FName(TEXT("PlaySimpleInfoUI"))));
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

void UMainHUDBase::SetWindTextVisible(EWindType WindType, bool visible)
{
	switch (WindType)
	{
	case EWindType::Left:
		mWindBase->SetLeftWindTextVisible(visible);
		break;
	case EWindType::Right:
		mWindBase->SetRightWindTextVisible(visible);
		break;
	case EWindType::Forward:
		mWindBase->SetForwardWindTextVisible(visible);
		break;
	case EWindType::Back:
		mWindBase->SetBackWindTextVisible(visible);
		break;
	}
}

void UMainHUDBase::SetPlayerImage(const FString& path, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetPlayerImage(path);

	else
		mPlaySimpleInfoBase->SetPlayerImage(path);
}

void UMainHUDBase::SetPlayerNameText(FString name, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetPlayerNameText(name);

	else
		mPlaySimpleInfoBase->SetPlayerNameText(name);
}

void UMainHUDBase::SetShotNumText(int shot, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetShotNumText(shot);

	else
		mPlaySimpleInfoBase->SetShotNumText(shot);
}

void UMainHUDBase::SetScoreText(int score, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetScoreText(score);

	else
		mPlaySimpleInfoBase->SetScoreText(score);
}

void UMainHUDBase::SetTargetDistanceText(float dis)
{
	mPlayInfoBase->SetTargetDistanceText(dis);
}
