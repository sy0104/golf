#include "GamePlayBase.h"
#include "MainHUDBase.h"
#include "../GFGameModeBase.h"
#include "../Ball/Ball.h"
#include "../Manager/GameManager.h"
#include "../GFGameInstance.h"

void UGamePlayBase::NativeConstruct()
{
	Super::NativeConstruct();

	mNextButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("NextButton"))));
	mHomeButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("HomeButton"))));
	mExitButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ExitButton"))));

	mNextButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnNextButtonClicked);
	mHomeButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnHomeButtonClicked);
	mExitButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnExitButtonClicked);
}

void UGamePlayBase::OnNextButtonClicked()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	FPlayerInfo PlayerInfo;

	if (IsValid(GameManager))
	{
		GameManager->AddTurn(1);
		PlayerInfo = GameManager->GetPlayerInfo(EPlayer::Player1);
	}

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			MainHUD->SetTotalScoreVisible(false);
			MainHUD->SetVisibility(ESlateVisibility::Visible);
			MainHUD->SetPlayerScoreText(PlayerInfo.Score, true);	// player 1 score

			if (GameManager->GetPlayType() == EPlayType::Multi)
			{
				PlayerInfo = GameManager->GetPlayerInfo(EPlayer::Player2);
				MainHUD->SetPlayerScoreText(PlayerInfo.Score, false);	// player 2 score
			}
		}
	}

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->Init(false);

	SetVisibility(ESlateVisibility::Hidden);
}

void UGamePlayBase::OnHomeButtonClicked()
{
	// �÷��̾� �ʱ�ȭ
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
		GameManager->ClearPlayers();

	// �� �ʱ�ȭ
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->Init(true);

	// Lobby�� ���ư���
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}

void UGamePlayBase::OnExitButtonClicked()
{
	// �𸮾� ������ ����
	FGenericPlatformMisc::RequestExit(false);
}
