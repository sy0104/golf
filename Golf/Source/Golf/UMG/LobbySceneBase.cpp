#include "LobbySceneBase.h"
#include "../GFGameInstance.h"
#include "../Manager/GameManager.h"
#include "../Ball/Ball.h"

void ULobbySceneBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayer2Image = Cast<UImage>(GetWidgetFromName(FName(TEXT("Player2Image"))));
	mPlayer2Text = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player2Text"))));

	mStartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartButton"))));
	mAddPlayer2Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("AddPlayer2Button"))));
	mDeletePlayer2Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("DeletePlayer2Button"))));

	mStartButton->OnClicked.AddDynamic(this, &ULobbySceneBase::OnStartButtonClicked);
	mAddPlayer2Button->OnClicked.AddDynamic(this, &ULobbySceneBase::OnAddPlayer2ButtonClicked);
	mDeletePlayer2Button->OnClicked.AddDynamic(this, &ULobbySceneBase::OnDeletePlayer2ButtonClicked);
}

void ULobbySceneBase::OnStartButtonClicked()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
		GameManager->CreatePlayers(mPlayType);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetPlayType(mPlayType);

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
}

void ULobbySceneBase::OnAddPlayer2ButtonClicked()
{
	FString path = L"/Script/Engine.Texture2D'/Game/UMG/UI_IMAGE/Player2Image.Player2Image";
	UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *path);

	if (IsValid(Tex2D))
		mPlayer2Image->SetBrushFromTexture(Tex2D);

	mPlayer2Image->SetVisibility(ESlateVisibility::Visible);
	mPlayer2Text->SetVisibility(ESlateVisibility::Visible);

	mAddPlayer2Button->SetVisibility(ESlateVisibility::Hidden);
	mDeletePlayer2Button->SetVisibility(ESlateVisibility::Visible);

	// multi
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		mPlayType = EPlayType::Multi;
		GameManager->SetPlayType(EPlayType::Multi);
	}
}

void ULobbySceneBase::OnDeletePlayer2ButtonClicked()
{
	mPlayer2Image->SetVisibility(ESlateVisibility::Hidden);
	mPlayer2Text->SetVisibility(ESlateVisibility::Hidden);

	mAddPlayer2Button->SetVisibility(ESlateVisibility::Visible);
	mDeletePlayer2Button->SetVisibility(ESlateVisibility::Hidden);

	// single
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		mPlayType = EPlayType::Single;
		GameManager->SetPlayType(EPlayType::Single);
	}
}
