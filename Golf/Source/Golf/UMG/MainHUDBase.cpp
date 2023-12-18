// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUDBase.h"
#include "DistanceBase.h"
#include "BallDetailBase.h"
#include "GolfClubBase.h"
#include "ScoreBase.h"
#include "BallStateBase.h"
#include "CourseBase.h"
#include "PlayInfoBase.h"
#include "MiniMap.h"
#include "BallSpinBase.h"
#include "WindBase.h"


void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceBase = Cast<UDistanceBase>(GetWidgetFromName(FName(TEXT("DistanceUI"))));
	mBallDetailBase = Cast<UBallDetailBase>(GetWidgetFromName(FName(TEXT("BallDetailUI"))));
	mGolfClubBase = Cast<UGolfClubBase>(GetWidgetFromName(FName(TEXT("GolfClubUI"))));
	mScoreBase = Cast<UScoreBase>(GetWidgetFromName(FName(TEXT("ScoreUI"))));
	mBallStateBase = Cast<UBallStateBase>(GetWidgetFromName(FName(TEXT("BallStateUI"))));
	mCourseBase = Cast<UCourseBase>(GetWidgetFromName(FName(TEXT("CourseUI"))));
	mPlayInfoBase = Cast<UPlayInfoBase>(GetWidgetFromName(FName(TEXT("PlayInfoUI"))));
	mMiniMap = Cast<UMiniMap>(GetWidgetFromName(FName(TEXT("MiniMapUI"))));
	mBallSpinBase = Cast<UBallSpinBase>(GetWidgetFromName(FName(TEXT("BallSpinUI"))));
	mWindBase = Cast<UWindBase>(GetWidgetFromName(FName(TEXT("WindUI"))));
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

void UMainHUDBase::SetCourseDistanceText(float dis)
{
	mPlayInfoBase->SetCourseDistanceText(dis);
}

void UMainHUDBase::SetTargetDistanceText(float dis)
{
	mPlayInfoBase->SetTargetDistanceText(dis);
}

void UMainHUDBase::SetShotNumText(int32 shot)
{
	mPlayInfoBase->SetShotNumText(shot);
}

void UMainHUDBase::SetScoreText()
{
	mPlayInfoBase->SetScoreText();
}

void UMainHUDBase::SetPlayInfoVisible(bool visible)
{
	if (visible)
		mPlayInfoBase->SetVisibility(ESlateVisibility::Visible);
	else
		mPlayInfoBase->SetVisibility(ESlateVisibility::Hidden);

}

void UMainHUDBase::SetMiniMapBallCurrent(/*FVector loc*/)
{
	mMiniMap->SetBallCurrent(/*loc*/);
}

void UMainHUDBase::SetMiniMapBallTarget(/*FVector loc*/)
{
	mMiniMap->SetBallTarget(/*loc*/);
}

//void UMainHUDBase::SetMiniMapHoleImage(FVector loc)
//{
//	mMiniMap->SetHoleImage(loc);
//}

void UMainHUDBase::SetMiniMapVisible(bool visible)
{
	if (visible)
		mMiniMap->SetVisibility(ESlateVisibility::Visible);
	else
		mMiniMap->SetVisibility(ESlateVisibility::Hidden);
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
