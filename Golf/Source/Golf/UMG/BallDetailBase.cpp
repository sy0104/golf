#include "BallDetailBase.h"

void UBallDetailBase::NativeConstruct()
{
	Super::NativeConstruct();

	mBallPowerBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallPowerBar"))));
}

void UBallDetailBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBallDetailBase::SetBallPower(float ratio)
{
	mBallPowerBar->SetPercent(ratio);
}
