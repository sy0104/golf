// Fill out your copyright notice in the Description page of Project Settings.


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
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dis: %f"), dis));
	mLeftDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%f"), dis)));
}

void UDistanceBase::SetDistanceText(float dis)
{
	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%f"), dis)));
}
