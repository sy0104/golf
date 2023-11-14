// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceBase.h"

void UDistanceBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));
}

void UDistanceBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UDistanceBase::SetDistanceText(float dis)
{
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dis: %f"), dis));
	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%f"), dis)));
}
