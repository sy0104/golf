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
	class UScoreBase*		mScoreBase;
	class UBallStateBase*	mBallStateBase;
	class UCourseBase*		mCourseBase;
	class UMiniMap*			mMiniMap;
	class UBallSpinBase*	mBallSpinBase;
	class UWindBase*		mWindBase;
	class UHoleInfoBase* mHoleInfoBase;
	class UPlayInfoBase* mPlayInfoBase;
	class UPlaySimpleInfoBase* mPlaySimpleInfoBase;
	
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

	// Score UI
	void SetScoreText(FString ScoreText);
	void SetConcedeTextVisible(bool visible);

	// Ball State
	void SetBallStateText(FString StateText);
	void SetBallStateVisible(bool visible);

	// Course
	void SetCourseText(FString CourseText);

	// Wind
	void SetWindTextVisible(EWindType WindType, bool visible);

	// Play Info & Play Simple Info
	void SetPlayerImage(const FString& path, bool isDetail = true);
	void SetPlayerNameText(FString name, bool isDetail = true);
	void SetShotNumText(int shot, bool isDetail = true);
	void SetScoreText(int score, bool isDetail = true);

	// Play Info
	void SetTargetDistanceText(float dis);
	void SetPlayInfoVisible(bool visible);

	// MiniMap
	void SetMiniMapBallCurrent(/*FVector loc*/);
	void SetMiniMapBallTarget(/*FVector loc*/);
	//void SetMiniMapHoleImage(FVector loc);
	void SetMiniMapVisible(bool visible);
};
