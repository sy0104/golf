// Fill out your copyright notice in the Description page of Project Settings.


#include "BallSpinBase.h"
#include "../GFGameModeBase.h"
#include "../Ball/BallController.h"
#include "../Ball/Ball.h"

void UBallSpinBase::NativeConstruct()
{
	Super::NativeConstruct();

	mSwingLeftButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingLeftButton"))));
	mSwingStraightButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingStraightButton"))));
	mSwingRightButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingRightButton"))));

	mSwingLeftButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingLeftButtonClicked);
	mSwingStraightButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingStraightButtonClicked);
	mSwingRightButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingRightButtonClicked);

}

void UBallSpinBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UBallSpinBase::OnSwingLeftButtonClicked()
{
	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallSwingType(EBallSwingType::Left);
			PrintViewport(1.f, FColor::Red, TEXT("Left Button"));
		}
	}
}

void UBallSpinBase::OnSwingStraightButtonClicked()
{
	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallSwingType(EBallSwingType::Straight);
		}
	}
}

void UBallSpinBase::OnSwingRightButtonClicked()
{
	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallSwingType(EBallSwingType::Right);
			PrintViewport(1.f, FColor::Red, TEXT("Right Button"));
		}
	}
}
