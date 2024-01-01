#pragma once

#include <Components\TextBlock.h>
#include <Components\Border.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "TotalScoreBase.generated.h"

UCLASS()
class GOLF_API UTotalScoreBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	TArray<UBorder*>		mPlayer2Borders;
	TArray<UTextBlock*>		mPlayer1ScoreTexts;
	TArray<UTextBlock*>		mPlayer2ScoreTexts;

	UBorder*		mPlayer2LankingBorder;
	UBorder*		mPlayer2NameBorder;
	UBorder*		mPlayer2TotalScoreBorder;

	UTextBlock*		mPlayer1LankingText;
	UTextBlock*		mPlayer2LankingText;
	
	UTextBlock*		mPlayer1NameText;
	UTextBlock*		mPlayer2NameText;

	UTextBlock*		mPlayer1TotalScoreText;
	UTextBlock*		mPlayer2TotalScoreText;

public:
	void SetPlayerRankingText(EPlayer player, int ranking);
	void SetPlayerScoreText(EPlayer player, int idx, int score);
	void SetPlayerTotalScoreText(EPlayer player, int score);
};
