// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ScoreSubsystem.generated.h"

UCLASS()
class GOLF_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	const FString GetScoreText(int score);
};
