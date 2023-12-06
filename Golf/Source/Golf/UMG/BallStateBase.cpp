// Fill out your copyright notice in the Description page of Project Settings.


#include "BallStateBase.h"

void UBallStateBase::NativeConstruct()
{
	Super::NativeConstruct();

	mBallStateText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("BallStateText"))));
}

void UBallStateBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UBallStateBase::SetBallStateText(FString StateText)
{
	mBallStateText->SetText(FText::FromString(StateText));
}
