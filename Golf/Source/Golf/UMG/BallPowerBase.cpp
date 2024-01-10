#include "BallPowerBase.h"
#include <Components/CanvasPanelSlot.h>
#include <Blueprint/WidgetLayoutLibrary.h>

void UBallPowerBase::NativeConstruct()
{
	Super::NativeConstruct();

	mBallPowerBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallPowerBar"))));
	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));
	mHalfDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HalfDistanceText"))));
	mHoleMarkImage = Cast<UImage>(GetWidgetFromName(FName("HoleMarkImage")));
}

void UBallPowerBase::SetBallPower(float ratio)
{
	mBallPowerBar->SetPercent(ratio);
}

void UBallPowerBase::SetBallDistance(EGolfClub club, EIronType ironClub)
{
	switch (club)
	{
	case EGolfClub::Driver:
		progressDis = 250;
		break;
	case EGolfClub::Wood:
		progressDis = 220;
		break;
	case EGolfClub::Iron:
		switch (ironClub)
		{
		case EIronType::Iron5:
			progressDis = 160;
			break;
		case EIronType::Iron6:
			progressDis = 150;
			break;
		case EIronType::Iron7:
			progressDis = 140;
			break;
		case EIronType::Iron8:
			progressDis = 130;
			break;
		case EIronType::Iron9:
			progressDis = 120;
			break;
		default:
			progressDis = 160;
			break;
		}
		break;
	case EGolfClub::Wedge:
		progressDis = 100;
		break;
	case EGolfClub::Putter:
		progressDis = 6;
		break;
	}

	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), progressDis) + TEXT("m")));
	mHalfDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), progressDis / 2) + TEXT("m")));
}

void UBallPowerBase::SetHoleMark(FVector ballPos, FVector destPos)
{
	FVector2D markPos = FVector2D(0.0f, 8.0f);
	double ballDis = FVector::Dist(ballPos, destPos) / 100.f;
	markPos.X = FMath::Min(ballDis * 600 / progressDis, 600);

	FVector2D holeMarkSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mHoleMarkImage)->GetSize();
	markPos.X -= holeMarkSize.X / 2;

	UWidgetLayoutLibrary::SlotAsCanvasSlot(mHoleMarkImage)->SetPosition(markPos);
	//mHoleMarkImage->SetRenderTranslation(FVector2D(0.0f, 12.0f));
}
