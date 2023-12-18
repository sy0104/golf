// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"

void UMiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	mCurrentBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("CurrentBallImage"))));
	mTargetBallImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("TargetBallImage"))));
	mFlagImage = Cast<UImage>(GetWidgetFromName(FName(TEXT("FlagImage"))));

}

void UMiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMiniMap::SetBallCurrent(/*FVector worldLocation*/)
{
	FVector2D currentBallLocation = FVector2D(0.0, 0.0);
	mCurrentBallImage->SetRenderTranslation(currentBallLocation);
}

void UMiniMap::SetBallTarget(/*FVector worldLocation*/)
{
	FVector2D targetBallLocation = FVector2D(0.0, 0.0);
	mTargetBallImage->SetRenderTranslation(targetBallLocation);
}

//void UMiniMap::SetHoleImage(FVector worldLocation)
//{
//	FVector2D holeLocation = FVector2D(20, 20);
//	mFlagImage->SetRenderTranslation(holeLocation);
//}
