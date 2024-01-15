#include "GameManager.h"
#include "../GFGameModeBase.h"
#include "../UMG/MainHUDBase.h"

UGameManager::UGameManager()
{
	mTurn = 1;
	mCurPlayer = EPlayer::Player1;
	mIsCreatePlayer = false;
}

UGameManager::~UGameManager()
{
}

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UGameManager::Deinitialize()
{
	Super::Deinitialize();

}

void UGameManager::CreatePlayers(EPlayType PlayType)
{
	// Single - 한명만 생성
	FPlayerInfo player;
	player.Name = L"Player 1";
	player.ImagePath = L"/Script/Engine.Texture2D'/Game/UMG/UI_IMAGE/Player1Image.Player1Image'";
	player.Score = 0;
	player.TotalShot = 0;
	player.Shot = 0;
	player.LeftDistance = FVector::Dist(FVector(0.0, 0.0, 13.5), FVector(37303.0, -998.0, 0.0));
	player.MaterialType = EMaterialType::Tee;
	player.TurnEnd = false;

	mPlayers.Add(player);

	// Multi - 두명 생성
	if (PlayType == EPlayType::Multi)
	{
		player.Name = L"Player 2";
		player.ImagePath = L"/Script/Engine.Texture2D'/Game/UMG/UI_IMAGE/Player2Image.Player2Image'";

		mPlayers.Add(player);
	}

	mIsCreatePlayer = true;
}

void UGameManager::ClearPlayers()
{
	mTurn = 1;
	mIsCreatePlayer = false;
	mPlayType = EPlayType::Single;

	mPlayers.Reset();
}

void UGameManager::Init()
{
	int PlayerSize = mPlayers.Num();
	
	for (int i = 0; i < PlayerSize; ++i)
	{
		mPlayers[i].BallPos = FVector(0.0, 0.0, 13.5);
		mPlayers[i].Score = mPlayers[i].Shot - 4;
		mPlayers[i].Shot = 0;
		//mPlayers[i].LeftDistance = FVector::Dist(FVector(0.0, 0.0, 13.5), FVector(37303.0, -998.0, 0.0));
		mPlayers[i].LeftDistance = 373.16;
		mPlayers[i].MaterialType = EMaterialType::Tee;
		mPlayers[i].TurnEnd = false;
	}
}
