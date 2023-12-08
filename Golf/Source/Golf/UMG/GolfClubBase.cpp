// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfClubBase.h"
#include "../GFGameModeBase.h"
#include "../Ball/BallController.h"
#include "../Ball/Ball.h"

void UGolfClubBase::NativeConstruct()
{
	Super::NativeConstruct();

	mClub1Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Club1Button"))));
	mClub2Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Club2Button"))));
	mClub3Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Club3Button"))));

	mClub1Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnClub1ButtonClicked);
	mClub2Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnClub2ButtonClicked);
	mClub3Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnClub3ButtonClicked);
}

void UGolfClubBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGolfClubBase::OnClub1ButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("1"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallMinPower(0.0);
			Ball->SetBallMaxPower(280000.0);	// 250m
			Ball->SetGolfClubType(EGolfClub::Club1);
		}
	}
}

void UGolfClubBase::OnClub2ButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("2"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallMinPower(0.0);
			Ball->SetBallMaxPower(250000.0);	// 200m
			Ball->SetGolfClubType(EGolfClub::Club2);
		}
	}
}

void UGolfClubBase::OnClub3ButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("3"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetBallMinPower(0.0);
			Ball->SetBallMaxPower(200000.0);	// 130m
			Ball->SetGolfClubType(EGolfClub::Club3);
		}
	}
}
