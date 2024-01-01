// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include <Components/CanvasPanelSlot.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>


void UMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	mCurrentBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("CurrentBallImage"))));
	mTargetBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("TargetBallImage"))));
	mHoleImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("HoleImage"))));

	mCurrentBallSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mCurrentBallImage)->GetSize();
	mTargetBallSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mTargetBallImage)->GetSize();
	mHoleSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mHoleImage)->GetSize();

	mDistanceCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("DistanceCanvas"))));
	mLeftDistanceCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("LeftDistanceCanvas"))));

	mDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("DistanceText"))));
	mLeftDistanceText = Cast<UTextBlock>(GetWidgetFromName(FName(TEXT("LeftDistanceText"))));

	mDistanceCanvasSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mDistanceCanvas)->GetSize();
	mLeftDistanceCanvasSize = UWidgetLayoutLibrary::SlotAsCanvasSlot(mLeftDistanceCanvas)->GetSize();
}

void UMiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

int32 UMiniMap::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// current ~ target line
	UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(mCurrentPos) + mCurrentBallSize / 2, FVector2D(mTargetPos) + mTargetBallSize / 2, FLinearColor::Yellow, false, 1.5f);
	
	// target ~ hole dashed line
	UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(mTargetPos) + mTargetBallSize / 2, FVector2D(mHolePos) + mHoleSize / 2, FLinearColor::White, false, 2.0f);

	return int32();
}

void UMiniMap::SetBallCurrent(FVector position)
{
	mCurrentPos.X = position.Y * 3 / 500 + 150 - mCurrentBallSize.X / 2;
	mCurrentPos.Y = -position.X / 125 + 376 - mCurrentBallSize.Y / 2;
	mCurrentBallImage->SetRenderTranslation(FVector2D(mCurrentPos));
}

void UMiniMap::SetBallTarget(FVector position, FVector direction, EGolfClub club)
{
	double distance = 0.0;
	switch (club)
	{
	case EGolfClub::Driver:
		distance = 22000;
		break;
	case EGolfClub::Wood:
		distance = 18000;
		break;
	case EGolfClub::Iron:
		distance = 15000;
		break;
	case EGolfClub::Wedge:
		distance = 8000;
		break;
	case EGolfClub::Putter:
		distance = 300;
		break;
	}

	position = position + (distance * direction);

	mTargetPos.X = position.Y * 3 / 500 + 150 - mTargetBallSize.X / 2;
	mTargetPos.Y = -position.X / 125 + 376 - mTargetBallSize.Y / 2;
	mTargetBallImage->SetRenderTranslation(FVector2D(mTargetPos));

	FVector miniMapCurrentPos, miniMapTargetPos, miniMapHolePos;
	miniMapCurrentPos.X = mCurrentPos.X + mCurrentBallSize.X / 2;
	miniMapCurrentPos.Y = mCurrentPos.Y + mCurrentBallSize.Y / 2;
	miniMapTargetPos.X = mTargetPos.X + mTargetBallSize.X / 2;
	miniMapTargetPos.Y = mTargetPos.Y + mTargetBallSize.Y / 2;
	miniMapHolePos.X = mHolePos.X + mHoleSize.X / 2;
	miniMapHolePos.Y = mHolePos.Y + mHoleSize.Y / 2;

	FVector2D distanceCanvasPos, leftdistanceCavasPos;
	distanceCanvasPos.X = miniMapTargetPos.X - mDistanceCanvasSize.X - 30.0;
	distanceCanvasPos.Y = miniMapTargetPos.Y - mDistanceCanvasSize.Y / 2;
	leftdistanceCavasPos.X = miniMapHolePos.X - mLeftDistanceCanvasSize.X - 30.0;
	leftdistanceCavasPos.Y = miniMapHolePos.Y - mLeftDistanceCanvasSize.Y / 2;

	mDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), FVector::Dist(miniMapCurrentPos, miniMapTargetPos)) + TEXT("m")));
	mDistanceCanvas->SetRenderTranslation(distanceCanvasPos);

	mLeftDistanceText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), FVector::Dist(miniMapTargetPos, miniMapHolePos)) + TEXT("m")));
	mLeftDistanceCanvas->SetRenderTranslation(leftdistanceCavasPos);
}

void UMiniMap::SetHoleImage(FVector position)
{
	mHolePos.X = position.Y * 3 / 500 + 150 - mHoleSize.X / 2;
	mHolePos.Y = -position.X / 125 + 376 - mHoleSize.Y / 2;
	mHoleImage->SetRenderTranslation(FVector2D(mHolePos));
}
