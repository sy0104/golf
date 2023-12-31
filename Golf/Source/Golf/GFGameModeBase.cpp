#include "GFGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallController.h"
#include "UMG/StartSceneBase.h"
#include "UMG/MainHUDBase.h"
#include "UMG/LobbySceneBase.h"

AGFGameModeBase::AGFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FClassFinder<ABall> BallFinder(
	//	TEXT("/Script/Engine.Blueprint'/Game/Ball/BPC_Ball.BPC_Ball_C'"));

	//if (BallFinder.Succeeded())
	//	DefaultPawnClass = BallFinder.Class;
	//else
	//	DefaultPawnClass = ABall::StaticClass();

	DefaultPawnClass = ABall::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();

	// Start Scene UI Class
	ConstructorHelpers::FClassFinder<UUserWidget>
		StartFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_StartScene.UI_StartScene_C'"));
	if (StartFinder.Succeeded())
		mStartSceneUIClass = StartFinder.Class;

	// Main UI
	ConstructorHelpers::FClassFinder<UUserWidget> 
		MainFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (MainFinder.Succeeded())
		mMainHUDClass = MainFinder.Class;

	// Lobby Scene UI Class
	ConstructorHelpers::FClassFinder<UUserWidget>
		LobbyFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_LobbyScene.UI_LobbyScene_C'"));
	if (LobbyFinder.Succeeded())
		mLobbySceneUIClass = LobbyFinder.Class;
}

void AGFGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Start" && IsValid(mStartSceneUIClass))
	{
		mStartSceneUI = Cast<UStartSceneBase>(CreateWidget(GetWorld(), mStartSceneUIClass));

		if (IsValid(mStartSceneUI))
			mStartSceneUI->AddToViewport();
	}

	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Main" && IsValid(mMainHUDClass))
	{
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
			mMainHUD->AddToViewport();
	}

	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Lobby" && IsValid(mLobbySceneUIClass))
	{
		mLobbySceneUI = Cast<ULobbySceneBase>(CreateWidget(GetWorld(), mLobbySceneUIClass));

		if (IsValid(mLobbySceneUI))
			mLobbySceneUI->AddToViewport();
	}
}