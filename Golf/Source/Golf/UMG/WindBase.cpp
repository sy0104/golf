#include "WindBase.h"

void UWindBase::NativeConstruct()
{
	Super::NativeConstruct();

	mLeftWindText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("LeftWindText"))));
	mRightWindText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("RightWindText"))));
	mForwardWindText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ForwardWindText"))));
	mBackWindText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("BackWindText"))));
	mWindVelText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("WindVelText"))));
}

void UWindBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UWindBase::SetLeftWindTextVisible(bool visible)
{
	if (visible)
		mLeftWindText->SetVisibility(ESlateVisibility::Visible);
	else
		mLeftWindText->SetVisibility(ESlateVisibility::Hidden);
}

void UWindBase::SetRightWindTextVisible(bool visible)
{
	if (visible)
		mRightWindText->SetVisibility(ESlateVisibility::Visible);
	else
		mRightWindText->SetVisibility(ESlateVisibility::Hidden);
}

void UWindBase::SetForwardWindTextVisible(bool visible)
{
	if (visible)
		mForwardWindText->SetVisibility(ESlateVisibility::Visible);
	else
		mForwardWindText->SetVisibility(ESlateVisibility::Hidden);
}

void UWindBase::SetBackWindTextVisible(bool visible)
{
	if (visible)
		mBackWindText->SetVisibility(ESlateVisibility::Visible);
	else
		mBackWindText->SetVisibility(ESlateVisibility::Hidden);
}

void UWindBase::SetWindVelText(float vel)
{
	mWindVelText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), vel) + TEXT("m/s")));
}
