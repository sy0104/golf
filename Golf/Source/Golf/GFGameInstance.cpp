// Fill out your copyright notice in the Description page of Project Settings.


#include "GFGameInstance.h"
#include "Manager/ScoreSubsystem.h"

UGFGameInstance::UGFGameInstance()
{
	mScoreSubsystem = nullptr;
}

UGFGameInstance::~UGFGameInstance()
{
}

void UGFGameInstance::Init()
{
	Super::Init();

	mScoreSubsystem = GetSubsystem<UScoreSubsystem>();
	if (IsValid(mScoreSubsystem))
	{

	}
}
