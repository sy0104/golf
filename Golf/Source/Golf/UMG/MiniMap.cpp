// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Blueprint/WidgetBlueprintLibrary.h>

void UMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	mCurrentBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("CurrentBallImage"))));
	mTargetBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("TargetBallImage"))));
	mFlagImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("FlagImage"))));
	mMiniMapImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("MiniMapImage"))));

	mCurrentBallCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("CurrentBallCanvas"))));
	mTargetBallCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("TargetBallCanvas"))));
	mFlagCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("FlagCanvas"))));
	mMiniMapCanvas = Cast<UCanvasPanel>(GetWidgetFromName(FName(TEXT("MiniMapCanvas"))));

	mLocalStartPos = UWidgetLayoutLibrary::SlotAsCanvasSlot(mCurrentBallCanvas)->GetPosition() + 1.0;	// 1.0 : 공 이미지 반지름(센터)
	mLocalDestPos = UWidgetLayoutLibrary::SlotAsCanvasSlot(mFlagCanvas)->GetPosition() + 1.0;	// 1.0 : 공 이미지 반지름(센터)

}

void UMiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//PrintViewport(10, FColor::Red, FString::Printf(TEXT("mWorldStartPos.X : %f"), mWorldStartPos.X));

}

void UMiniMap::SetBallCurrent(FVector2D position)
{
	FVector2D newPosition(0.0, 0.0);

	newPosition.X = position.Y * (mLocalDestPos.X - mLocalStartPos.X) / mWorldDestPos.Y/* + mLocalStartPos.X*/;
	newPosition.Y = position.X * (mLocalDestPos.Y - mLocalStartPos.Y) / mWorldDestPos.X/*+ mLocalStartPos.Y*/;
	mCurrentBallCanvas->SetRenderTranslation(FVector2D(newPosition.X /*- mLocalStartPos.X*/, newPosition.Y /*- mLocalStartPos.Y*/));
	mCurrentPos = newPosition;

	//UWidgetBlueprintLibrary::DrawLine()
}

void UMiniMap::SetBallTarget(FVector2D direction, EGolfClub club)
{
	FVector2D position(0.0, 0.0), newPosition(0.0, 0.0);

	switch (club)
	{
	case EGolfClub::Driver:
		position.X = 22000;
		break;
	case EGolfClub::Wood:
		position.X = 18000;
		break;
	case EGolfClub::Iron:
		position.X = 15000;
		break;
	case EGolfClub::Wedge:
		position.X = 8000;
		break;
	case EGolfClub::Putter:
		position.X = 300;
		break;
	}
	
	newPosition.X = direction.Y * 10;
	newPosition.Y = position.X * (mLocalDestPos.Y - mLocalStartPos.Y) / mWorldDestPos.X /*+ mLocalStartPos.Y*/;

	if (direction.X < 0)
	{
		newPosition.Y = -newPosition.Y;
	}
	else if (direction.X == 1.0)
	{
		newPosition.X = newPosition.Y;
		newPosition.Y = 0;
	}
		
	mTargetBallCanvas->SetRenderTranslation(FVector2D(mCurrentPos.X + newPosition.X, mCurrentPos.Y + newPosition.Y /*- mLocalStartPos.Y*/));
}

void UMiniMap::SetInfo(FVector2D startPos, FVector2D destPos)
{
	mWorldStartPos = startPos;
	mWorldDestPos = destPos;
}

//void UMiniMap::SetHoleImage(FVector worldLocation)
//{
//	FVector2D holeLocation = FVector2D(20, 20);
//	mFlagImage->SetRenderTranslation(holeLocation);
//}
