// Fill out your copyright notice in the Description page of Project Settings.


#include "GolfClubBase.h"
#include "../GFGameModeBase.h"
#include "../Ball/BallController.h"
#include "../Ball/Ball.h"

void UGolfClubBase::NativeConstruct()
{
	Super::NativeConstruct();

	mDriverButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("DriverButton"))));
	mWoodButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("WoodButton"))));
	mIronButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("IronButton"))));
	mWedgeButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("WedgeButton"))));
	mPutterButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("PutterButton"))));

	mDriverButton->OnClicked.AddDynamic(this, &UGolfClubBase::OnDriverButtonClicked);
	mWoodButton->OnClicked.AddDynamic(this, &UGolfClubBase::OnWoodButtonClicked);
	mIronButton->OnClicked.AddDynamic(this, &UGolfClubBase::OnIronButtonClicked);
	mWedgeButton->OnClicked.AddDynamic(this, &UGolfClubBase::OnWedgeButtonClicked);
	mPutterButton->OnClicked.AddDynamic(this, &UGolfClubBase::OnPutterButtonClicked);
}

void UGolfClubBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGolfClubBase::OnDriverButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Driver"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetGolfClubType(EGolfClub::Driver);
		}
	}
}

void UGolfClubBase::OnWoodButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Wood"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetGolfClubType(EGolfClub::Wood);
		}
	}
}

void UGolfClubBase::OnIronButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Iron"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetGolfClubType(EGolfClub::Iron);
		}
	}
}

void UGolfClubBase::OnWedgeButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Wedge"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetGolfClubType(EGolfClub::Wedge);
		}
	}
}

void UGolfClubBase::OnPutterButtonClicked()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Putter"));

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			Ball->SetGolfClubType(EGolfClub::Putter);
		}
	}
}
