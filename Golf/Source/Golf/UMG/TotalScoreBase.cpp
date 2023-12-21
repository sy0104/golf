#include "TotalScoreBase.h"
#include "../GFGameInstance.h"
#include "../Manager/GameManager.h"

void UTotalScoreBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayer1LankingText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player1LankingText"))));
	mPlayer2LankingText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player2LankingText"))));

	mPlayer1NameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player1NameText"))));
	mPlayer2NameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player2NameText"))));

	mPlayer2LankingBorder = Cast<UBorder>(GetWidgetFromName(FName(TEXT("Player2LankingBorder"))));
	mPlayer2NameBorder = Cast<UBorder>(GetWidgetFromName(FName(TEXT("Player2NameBorder"))));
	mPlayer2TotalScoreBorder = Cast<UBorder>(GetWidgetFromName(FName(TEXT("Player2TotalScoreBorder"))));

	mPlayer1TotalScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player1TotalScore"))));
	mPlayer2TotalScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("Player2TotalScore"))));

	for (int i = 0; i < 18; ++i)
	{
		// Player2 Borders
		FName BorderName = FName(FString("P2ScoreBorder") + FString::Printf(TEXT("%d"), i + 1));
		UBorder* border = Cast<UBorder>(GetWidgetFromName(BorderName));
		mPlayer2Borders.Add(border);

		// Player1 Score Texts
		FName P1ScoreName = FName(FString("Player1Score") + FString::Printf(TEXT("%d"), i + 1));
		UTextBlock* tb1 = Cast<UTextBlock>(GetWidgetFromName(P1ScoreName));
		mPlayer1ScoreTexts.Add(tb1);

		// Player2 Score Texts
		FName P2ScoreName = FName(FString("Player2Score") + FString::Printf(TEXT("%d"), i + 1));
		UTextBlock* tb2 = Cast<UTextBlock>(GetWidgetFromName(P2ScoreName));
		mPlayer2ScoreTexts.Add(tb2);
	}

	// Single Play
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		EPlayType PlayType = GameManager->GetPlayType();
		if (PlayType == EPlayType::Single)
		{
			for (int i = 0; i < 18; ++i)
				mPlayer2Borders[i]->SetVisibility(ESlateVisibility::Hidden);

			mPlayer2LankingBorder->SetVisibility(ESlateVisibility::Hidden);
			mPlayer2NameBorder->SetVisibility(ESlateVisibility::Hidden);
			mPlayer2TotalScoreBorder->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UTotalScoreBase::SetPlayerRankingText(EPlayer player, int score)
{
	if (player == EPlayer::Player1)
	{
		mPlayer1LankingText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}

	else
	{
		mPlayer2LankingText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}
}

void UTotalScoreBase::SetPlayerScoreText(EPlayer player, int idx, int score)
{
	if (player == EPlayer::Player1)
	{
		mPlayer1ScoreTexts[idx]->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}

	else
	{
		mPlayer2ScoreTexts[idx]->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}
}

void UTotalScoreBase::SetPlayerTotalScoreText(EPlayer player, int score)
{
	if (player == EPlayer::Player1)
	{
		mPlayer1TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}

	else
	{
		mPlayer2TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
	}
}
