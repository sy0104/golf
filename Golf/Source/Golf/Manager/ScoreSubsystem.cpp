#include "ScoreSubsystem.h"

UScoreSubsystem::UScoreSubsystem()
{
}

UScoreSubsystem::~UScoreSubsystem()
{
}

void UScoreSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

const FString UScoreSubsystem::GetScoreText(int score)
{
	FString ScoreText;

	switch (score)
	{
	case -3:
		ScoreText = L"Hole In One";
		break;
	case -2:
		ScoreText = L"Eagle";
		break;
	case -1:
		ScoreText = L"Birdie";
		break;
	case 0:
		ScoreText = L"Par";
		break;
	case 1:
		ScoreText = L"Bogey";
		break;
	case 2:
		ScoreText = L"Double Bogey";
		break;
	case 3:
		ScoreText = L"Triple Bogey";
		break;
	default:
		ScoreText = L"Double Par";
		break;
	}

	return ScoreText;
}
