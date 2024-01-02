#include "ScoreBase.h"

void UScoreBase::NativeConstruct()
{
	Super::NativeConstruct();

	mScoreText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ScoreText"))));
	mConcedeText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("ConcedeText"))));
}

void UScoreBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UScoreBase::SetScoreText(FString ScoreText)
{
	mScoreText->SetVisibility(ESlateVisibility::Visible);
	mScoreText->SetText(FText::FromString(ScoreText));
}

void UScoreBase::SetConcedeTextVisible(bool visible)
{
	if (visible)
		mConcedeText->SetVisibility(ESlateVisibility::Visible);
	else
		mConcedeText->SetVisibility(ESlateVisibility::Hidden);
}
