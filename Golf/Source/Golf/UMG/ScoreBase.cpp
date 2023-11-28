// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreBase.h"

void UScoreBase::NativeConstruct()
{
	Super::NativeConstruct();

	mScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ScoreText"))));
}

void UScoreBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UScoreBase::SetScoreText(FString ScoreText)
{
	mScoreText->SetText(FText::FromString(ScoreText));
}
