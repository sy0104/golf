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
	class UPlayInfoBase*	mPlayInfoBase;
	//class UMiniMap*			mMiniMap;
	class UBallSpinBase*	mBallSpinBase;
	class UWindBase* mWindBase;
	
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

	// Play Information
	void SetCourseDistanceText(float dis);
	void SetTargetDistanceText(float dis);
	void SetShotNumText(int32 shot);
	void SetScoreText();
	void SetPlayInfoVisible(bool visible);

	// MiniMap
	void SetMapImage();

	// Wind
	void SetWindTextVisible(EWindType WindType, bool visible);
};
