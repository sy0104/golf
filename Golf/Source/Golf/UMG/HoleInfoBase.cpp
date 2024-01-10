#include "HoleInfoBase.h"

void UHoleInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHoleNumText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("HoleNumText"))));
}

void UHoleInfoBase::SetHoleNumText(int num)
{
	mHoleNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), num)));
}
