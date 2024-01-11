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
	class UDistanceBase*		mDistanceBase;
	class UBallDetailBase*		mBallDetailBase;
	class UBallPowerBase*		mBallPowerBase;
	class UBallSpinBase*		mBallSpinBase;
	class UGolfClubBase*		mGolfClubBase;
	class UScoreBase*			mScoreBase;
	class UBallStateBase*		mBallStateBase;
	class UCourseBase*			mCourseBase;
	class UMiniMap*				mMiniMap;
	class UWindBase*			mWindBase;
	class UHoleInfoBase*		mHoleInfoBase;
	class UPlayInfoBase*		mPlayInfoBase;
	class UPlaySimpleInfoBase*	mPlaySimpleInfoBase;
	class UTotalScoreBase*		mTotalScoreBase;
	class UGamePlayBase*		mGamePlayBase;
	class UMenuBase*			mMenuBase;
	class UGoodShotBase*		mGoodShotBase;
	class UBallInfoBase*		mBallInfoBase;
	
public:
	class UDistanceBase* GetDistanceBase()
	{
		return mDistanceBase;
	}

	// Distance
	void SetLeftDistanceText(float dis);
	void SetDistanceText(float dis);
	void SetDistanceVisible(bool visible);

	// Ball Detail
	void SetBallPower(float ratio);
	void SetBallSpin(float ratio);
	void SetSpinButtonsEnable();
	void SetPowerCheckBoxChecked(bool IsChecked);
	void SetSpinCheckBoxChecked(bool IsChecked);
	void SetBallDistance(EGolfClub club, EIronType ironClub);
	void SetHoleMark(FVector ballPos, FVector destPos);

	// Score UI
	void SetScoreText(FString ScoreText);
	void SetScoreTextVisible(bool visible);
	void SetConcedeTextVisible(bool visible);

	// Ball State
	void SetBallStateText(FString StateText);
	void SetBallStateVisible(bool visible);

	// Course
	void SetCourseText(FString CourseText);

	// Wind
	void SetWindVisible(bool visible);
	void SetWindTextVisible(EWindType WindType, bool visible);
	void SetWindVelText(float vel);

	// Play Info & Play Simple Info
	void SetPlayerImage(const FString& path, bool isDetail = true);
	void SetPlayerNameText(FString name, bool isDetail = true);
	void SetPlayerShotNumText(int shot, bool isDetail = true);
	void SetPlayerScoreText(int score, bool isDetail = true);
	void SetPlayerTargetDistanceText(float dis, bool isDetail = true);

	// Play Info
	void SetPlayInfoVisible(bool visible);

	// Play Simple Info
	void SetPlaySimpleInfoVisible(bool visible);

	// MiniMap
	void SetMiniMapBallCurrent(FVector position);
	void SetMiniMapBallTarget(FVector position, FVector direction, double targetDis);
	void SetMiniMapHoleImage(FVector position);
	void SetMiniMapVisible(bool visible);
	void SetMiniMapTargetDistanceText(float dis);
	void SetMiniMapHoleDistanceText(float dis);

	// Total Score
	void SetTotalScoreVisible(bool visible);
	void SetPlayerRankingText(EPlayer player, int ranking);
	void SetPlayerScoreText(EPlayer player, int idx, int score);
	void SetPlayerTotalScoreText(EPlayer player, int TotalShot, int score);

	// Ball Power, Ball Spin, Ball Club
	void SetBallInfoVisible(bool visible);

	// Gmae Play
	void SetGamePlayVisible(bool visible);

	// Menu
	void ShowMenu(bool visible);
	bool GetIsShowMenu() const;

	// GoodShot
	void SetGoodShotVisible(bool visible);

	// Putting
	void SetPuttingClub(bool isPutting);
	void SetPuttingInfo(FVector ballPos, FVector destPos);
	void SetPuttingInfoVisible(bool isPutting);

	// Hole Info
	void SetHoleNumText(int num);

	// Club
	void SetIronButtonVisible(bool visible);
};
