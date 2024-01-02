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
#include "HoleInfoBase.h"
#include "PlaySimpleInfoBase.h"
#include "TotalScoreBase.h"
#include "GamePlayBase.h"
#include "MenuBase.h"
#include "../GFGameInstance.h"
#include "../Manager/GameManager.h"

void UMainHUDBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceBase = Cast<UDistanceBase>(GetWidgetFromName(FName(TEXT("DistanceUI"))));
	mBallDetailBase = Cast<UBallDetailBase>(GetWidgetFromName(FName(TEXT("BallDetailUI"))));
	mGolfClubBase = Cast<UGolfClubBase>(GetWidgetFromName(FName(TEXT("GolfClubUI"))));
	mScoreBase = Cast<UScoreBase>(GetWidgetFromName(FName(TEXT("ScoreUI"))));
	mBallStateBase = Cast<UBallStateBase>(GetWidgetFromName(FName(TEXT("BallStateUI"))));
	mCourseBase = Cast<UCourseBase>(GetWidgetFromName(FName(TEXT("CourseUI"))));
	mMiniMap = Cast<UMiniMap>(GetWidgetFromName(FName(TEXT("MiniMapUI"))));
	mBallSpinBase = Cast<UBallSpinBase>(GetWidgetFromName(FName(TEXT("BallSpinUI"))));
	mWindBase = Cast<UWindBase>(GetWidgetFromName(FName(TEXT("WindUI"))));
	mHoleInfoBase = Cast<UHoleInfoBase>(GetWidgetFromName(FName(TEXT("HoleInfoUI"))));
	mPlayInfoBase = Cast<UPlayInfoBase>(GetWidgetFromName(FName(TEXT("PlayInfoUI"))));
	mPlaySimpleInfoBase = Cast<UPlaySimpleInfoBase>(GetWidgetFromName(FName(TEXT("PlaySimpleInfoUI"))));
	mTotalScoreBase = Cast<UTotalScoreBase>(GetWidgetFromName(FName(TEXT("TotalScoreUI"))));
	mGamePlayBase = Cast<UGamePlayBase>(GetWidgetFromName(FName(TEXT("GamePlayUI"))));
	mMenuBase = Cast<UMenuBase>(GetWidgetFromName(FName(TEXT("MenuUI"))));

	// Multi Set
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		if (GameManager->GetIsCreatePlayer())
		{
			EPlayType PlayType = GameManager->GetPlayType();
			TArray<FPlayerInfo> players = GameManager->GetPlayers();

			mPlayInfoBase->SetPlayerImage(players[(int)EPlayer::Player1].ImagePath);

			if (PlayType == EPlayType::Multi)
			{
				mPlaySimpleInfoBase->SetVisibility(ESlateVisibility::Visible);
				mPlaySimpleInfoBase->SetPlayerImage(players[(int)EPlayer::Player2].ImagePath);
				mPlaySimpleInfoBase->SetPlayerNameText(players[(int)EPlayer::Player2].Name);
			}
		}

		// [test] main에서 바로 시작
		else
		{
			GameManager->CreatePlayers(EPlayType::Single);
		}
	}
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

void UMainHUDBase::SetScoreTextVisible(bool visible)
{
	if (visible)
		mScoreBase->SetVisibility(ESlateVisibility::Visible);
	else
		mScoreBase->SetVisibility(ESlateVisibility::Hidden);
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

void UMainHUDBase::SetPlayerShotNumText(int shot, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetPlayerShotNumText(shot);

	else
		mPlaySimpleInfoBase->SetPlayerShotNumText(shot);
}

void UMainHUDBase::SetPlayerScoreText(int score, bool isDetail)
{
	if (isDetail)
		mPlayInfoBase->SetPlayerScoreText(score);

	else
		mPlaySimpleInfoBase->SetPlayerScoreText(score);
}

void UMainHUDBase::SetPlayerTargetDistanceText(float dis)
{
	mPlayInfoBase->SetPlayerTargetDistanceText(dis);
}

void UMainHUDBase::SetPlayInfoVisible(bool visible)
{
	if (visible)
		mPlayInfoBase->SetVisibility(ESlateVisibility::Visible);
	else
		mPlayInfoBase->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::SetPlaySimpleInfoVisible(bool visible)
{
	if (visible)
		mPlaySimpleInfoBase->SetVisibility(ESlateVisibility::Visible);
	else
		mPlaySimpleInfoBase->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::SetMiniMapBallCurrent(FVector position)
{
	mMiniMap->SetBallCurrent(position);
}

void UMainHUDBase::SetMiniMapBallTarget(FVector position, FVector direction, EGolfClub club)
{
	mMiniMap->SetBallTarget(position, direction, club);
}

void UMainHUDBase::SetMiniMapHoleImage(FVector position)
{
	mMiniMap->SetHoleImage(position);
}

void UMainHUDBase::SetMiniMapVisible(bool visible)
{
	if (visible)
		mMiniMap->SetVisibility(ESlateVisibility::Visible);
	else
		mMiniMap->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::SetTotalScoreVisible(bool visible)
{
	if (visible)
		mTotalScoreBase->SetVisibility(ESlateVisibility::Visible);
	else
		mTotalScoreBase->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::SetPlayerRankingText(EPlayer player, int ranking)
{
	mTotalScoreBase->SetPlayerRankingText(player, ranking);
}

void UMainHUDBase::SetPlayerScoreText(EPlayer player, int idx, int score)
{
	mTotalScoreBase->SetPlayerScoreText(player, idx, score);
}

void UMainHUDBase::SetPlayerTotalScoreText(EPlayer player, int TotalShot, int score)
{
	mTotalScoreBase->SetPlayerTotalScoreText(player, TotalShot, score);
}

void UMainHUDBase::SetBallInfoVisible(bool visible)
{
	if (visible)
	{
		mBallDetailBase->SetVisibility(ESlateVisibility::Visible);
		mBallSpinBase->SetVisibility(ESlateVisibility::Visible);
		mGolfClubBase->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		mBallDetailBase->SetVisibility(ESlateVisibility::Hidden);
		mBallSpinBase->SetVisibility(ESlateVisibility::Hidden);
		mGolfClubBase->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUDBase::SetGamePlayVisible(bool visible)
{
	if (visible)
		mGamePlayBase->SetVisibility(ESlateVisibility::Visible);
	else
		mGamePlayBase->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDBase::ShowMenu(bool visible)
{
	if (visible)
		mMenuBase->SetVisibility(ESlateVisibility::Visible);
	else
		mMenuBase->SetVisibility(ESlateVisibility::Hidden);
}

bool UMainHUDBase::GetIsShowMenu() const
{
	if (mMenuBase->IsVisible())
		return true;

	return false;
}
