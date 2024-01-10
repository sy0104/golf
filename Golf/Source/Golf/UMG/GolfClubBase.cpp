#include "GolfClubBase.h"
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

	// Iron
	mIron5Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Iron5Button"))));
	mIron6Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Iron6Button"))));
	mIron7Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Iron7Button"))));
	mIron8Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Iron8Button"))));
	mIron9Button = Cast<UButton>(GetWidgetFromName(FName(TEXT("Iron9Button"))));

	mIron5Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnIron5ButtonClicked);
	mIron6Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnIron6ButtonClicked);
	mIron7Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnIron7ButtonClicked);
	mIron8Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnIron8ButtonClicked);
	mIron9Button->OnClicked.AddDynamic(this, &UGolfClubBase::OnIron9ButtonClicked);
}

void UGolfClubBase::OnDriverButtonClicked()
{
	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Driver);
}

void UGolfClubBase::OnWoodButtonClicked()
{
	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wood);
}

void UGolfClubBase::OnIronButtonClicked()
{
	SetIronButtonVisible(true);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Iron);
}

void UGolfClubBase::OnWedgeButtonClicked()
{
	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wedge);
}

void UGolfClubBase::OnPutterButtonClicked()
{
	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Putter);
}

void UGolfClubBase::OnIron5ButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron5);
}

void UGolfClubBase::OnIron6ButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron6);
}

void UGolfClubBase::OnIron7ButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron7);
}

void UGolfClubBase::OnIron8ButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron8);
}

void UGolfClubBase::OnIron9ButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron9);
}

void UGolfClubBase::SetPuttingClub(bool isPutting)
{
	if (isPutting)
	{
		mDriverButton->SetIsEnabled(false);
		mWoodButton->SetIsEnabled(false);
		mIronButton->SetIsEnabled(false);
		mWedgeButton->SetIsEnabled(false);

		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
			Ball->SetGolfClubType(EGolfClub::Putter);
	}
	else
	{
		mDriverButton->SetIsEnabled(true);
		mWoodButton->SetIsEnabled(true);
		mIronButton->SetIsEnabled(true);
		mWedgeButton->SetIsEnabled(true);
	}
}

void UGolfClubBase::SetIronButtonVisible(bool visible)
{
	if (visible)
	{
		mIron5Button->SetVisibility(ESlateVisibility::Visible);
		mIron6Button->SetVisibility(ESlateVisibility::Visible);
		mIron7Button->SetVisibility(ESlateVisibility::Visible);
		mIron8Button->SetVisibility(ESlateVisibility::Visible);
		mIron9Button->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		mIron5Button->SetVisibility(ESlateVisibility::Hidden);
		mIron6Button->SetVisibility(ESlateVisibility::Hidden);
		mIron7Button->SetVisibility(ESlateVisibility::Hidden);
		mIron8Button->SetVisibility(ESlateVisibility::Hidden);
		mIron9Button->SetVisibility(ESlateVisibility::Hidden);
	}
}
