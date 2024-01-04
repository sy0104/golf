#include "BallDetailBase.h"
#include <Components/CanvasPanelSlot.h>
#include <Blueprint/WidgetLayoutLibrary.h>

void UBallDetailBase::NativeConstruct()
{
	Super::NativeConstruct();

	mBallPowerBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallPowerBar"))));
	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));
	mHalfDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HalfDistanceText"))));
	mHoleMarkImage = Cast<UImage>(GetWidgetFromName(FName("HoleMarkImage")));
}

void UBallDetailBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBallDetailBase::SetBallPower(float ratio)
{
	mBallPowerBar->SetPercent(ratio);
}

void UBallDetailBase::SetBallDistance(EGolfClub club)
{
	switch (club)
	{
	case EGolfClub::Driver:
		progressDis = 270;
		//progressDis = 180;
		break;
	case EGolfClub::Wood:
		progressDis = 260;
		//progressDis = 150;
		break;
	case EGolfClub::Iron:
		progressDis = 250;
		//progressDis = 120;
		break;
	case EGolfClub::Wedge:
		progressDis = 70;
		//progressDis = 70;
		break;
	case EGolfClub::Putter:
		progressDis = 7;
		//progressDis = 30;
		break;
	}

	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), progressDis) + TEXT("m")));
	mHalfDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), progressDis / 2) + TEXT("m")));
}

void UBallDetailBase::SetHoleMark(FVector ballPos, FVector destPos)
{
	FVector2D markPos = FVector2D(0.0f, 8.0f);
	double ballDis = FVector::Dist(ballPos, destPos) / 100.f;
	markPos.X = FMath::Min(ballDis * 600 / progressDis, 600);

	FVector2D holeMarkSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mHoleMarkImage)->GetSize();
	markPos.X -= holeMarkSize.X / 2;

	UWidgetLayoutLibrary::SlotAsCanvasSlot(mHoleMarkImage)->SetPosition(markPos);
	//mHoleMarkImage->SetRenderTranslation(FVector2D(0.0f, 12.0f));

}
