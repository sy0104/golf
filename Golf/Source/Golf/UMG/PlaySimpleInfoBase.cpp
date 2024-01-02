#include "PlaySimpleInfoBase.h"

void UPlaySimpleInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPlayerImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("PlayerImage"))));

	mPlayerNameText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("PlayerNameText"))));
	mShotNumText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ShotNumText"))));
	mScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ScoreText"))));
}

void UPlaySimpleInfoBase::SetPlayerImage(const FString& path)
{
	UTexture2D* Tex2D = LoadObject<UTexture2D>(nullptr, *path);

	if (IsValid(Tex2D))
		mPlayerImage->SetBrushFromTexture(Tex2D);
}

void UPlaySimpleInfoBase::SetPlayerNameText(FString name)
{
	mPlayerNameText->SetText(FText::FromString(name));
}

void UPlaySimpleInfoBase::SetPlayerShotNumText(int ShotNum)
{
	FString ShotNumText;

	switch (ShotNum)
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

	mShotNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), ShotNum) + ShotNumText + TEXT(" shot")));
}

void UPlaySimpleInfoBase::SetPlayerScoreText(int score)
{
	mScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), score)));

}

