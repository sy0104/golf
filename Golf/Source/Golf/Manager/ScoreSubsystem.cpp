// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"

const FString UScoreSubsystem::GetScoreText(int score)
{
	FString ScoreText;

	switch (score)
	{
	case -4:
		ScoreText = L"Hole In One";
		break;
	case -3:
		ScoreText = L"Albatross";
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
	case 4:
		ScoreText = L"Double Par";
		break;
	}

	return ScoreText;
}
