#include "GameManager.h"
#include "../GFGameModeBase.h"
#include "../UMG/MainHUDBase.h"

UGameManager::UGameManager()
{
	mTurn = 0;
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
	player.ScoreDetail = 0;
	player.Shot = 0;
	player.LeftDistance = 0.f;
	player.TurnEnd = false;


	mPlayers.Add(player);

	// Multi - 두명 생성
	if (PlayType == EPlayType::Multi)
	{
		player.Name = L"Player 2";
		player.ImagePath = L"/Script/Engine.Texture2D'/Game/UMG/UI_IMAGE/Player2Image.Player2Image'";
		player.Score = 0;
		player.ScoreDetail = 0;
		player.Shot = 0;
		player.LeftDistance = 0.f;
		player.TurnEnd = false;

		mPlayers.Add(player);

		//AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		//if (IsValid(GameMode))
		//{
		//	PrintViewport(1.f, FColor::Red, TEXT("GameMode Valid"));

		//	UMainHUDBase* MainHUD = GameMode->GetMainHUD();
		//	MainHUD->SetPlaySimpleInfoVisible(true);
		//}
	}
}
