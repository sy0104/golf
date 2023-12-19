// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayInfoBase.h"

void UPlayInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("PlayerImage"))));

	mPlayerNameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("PlayerNameText"))));
	mTargetDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("TargetDistanceText"))));
	mShotNumText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ShotNumText"))));
	mScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ScoreText"))));

}

void UPlayInfoBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayInfoBase::SetPlayerImage(const FString& path)
{
	UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *path);

	if (IsValid(Tex2D))
		mPlayerImage->SetBrushFromTexture(Tex2D);
}

void UPlayInfoBase::SetPlayerNameText(FString name)
{
	mPlayerNameText->SetText(FText::FromString(name));
}

void UPlayInfoBase::SetTargetDistanceText(float dis)
{
	mTargetDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), dis) + TEXT("m")));
}

void UPlayInfoBase::SetShotNumText(int32 shot)
{
	FString ShotNumText;

	switch (shot)
	{
	case 1:
		ShotNumText = L"st";
		break;
	case 2:
		ShotNumText = L"nd";
		break;
	case 3:
		ShotNumText = L"rd";
		break;
	default:
		ShotNumText = L"th";
		break;
	}

	mShotNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), shot) + ShotNumText + TEXT(" shot")));
}

void UPlayInfoBase::SetScoreText(int32 score)
{
	mScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));
}
