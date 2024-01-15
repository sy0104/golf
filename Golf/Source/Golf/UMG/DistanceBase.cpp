#include "DistanceBase.h"

void UDistanceBase::NativeConstruct()
{
	Super::NativeConstruct();

	mLeftDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("LeftDistanceText"))));
	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));
}

void UDistanceBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDistanceBase::SetLeftDistanceText(float dis)
{
	mLeftDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), dis)));
}

void UDistanceBase::SetDistanceText(float dis)
{
	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), dis)));
}
