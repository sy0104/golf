// Fill out your copyright notice in the Description page of Project Settings.


#include "BallInfoBase.h"

void UBallInfoBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDistanceCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("DistanceCanvas"))));
	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));

}

void UBallInfoBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	mDistanceCanvasSize = mDistanceCanvas->GetDesiredSize();
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FVector2D canvasPos;
	if (PlayerController->ProjectWorldLocationToScreen(mdestPos, canvasPos))
	{
		float leftDis = FVector::Dist(mballPos, mdestPos);
		if (leftDis < 1000)
		{
			//canvasPos.X += mDistanceCanvasSize.X * 1.3;
			//canvasPos.Y += mDistanceCanvasSize.Y * 1.7;
			canvasPos = FVector2D(1050, 550);
		}
		else
		{
			//canvasPos.X += mDistanceCanvasSize.X * 1.3;
			//canvasPos.Y += mDistanceCanvasSize.Y * 1.5;
			canvasPos = FVector2D(1000, 500);
		}

		mDistanceCanvas->SetRenderTranslation(canvasPos);
	}
}

void UBallInfoBase::SetPuttingInfo(FVector ballPos, FVector destPos)
{
	mballPos = ballPos;
	mdestPos = destPos;

	float leftDis = FVector::Dist(mballPos, mdestPos);
	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), leftDis / 100.f) + TEXT("m")));
}
