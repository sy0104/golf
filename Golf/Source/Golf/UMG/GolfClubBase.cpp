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

	mDriverButton->SetIsEnabled(false);

	// Club Frame
	mDriverFrame = Cast<UImage>(GetWidgetFromName(FName(TEXT("DriverFrame"))));
	mWoodFrame = Cast<UImage>(GetWidgetFromName(FName(TEXT("WoodFrame"))));
	mIronFrame = Cast<UImage>(GetWidgetFromName(FName(TEXT("IronFrame"))));
	mWedgeFrame = Cast<UImage>(GetWidgetFromName(FName(TEXT("WedgeFrame"))));
	mPutterFrame = Cast<UImage>(GetWidgetFromName(FName(TEXT("PutterFrame"))));

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

	// Iron Frame
	mIron5Frame = Cast<UImage>(GetWidgetFromName(FName(TEXT("Iron5Frame"))));
	mIron6Frame = Cast<UImage>(GetWidgetFromName(FName(TEXT("Iron6Frame"))));
	mIron7Frame = Cast<UImage>(GetWidgetFromName(FName(TEXT("Iron7Frame"))));
	mIron8Frame = Cast<UImage>(GetWidgetFromName(FName(TEXT("Iron8Frame"))));
	mIron9Frame = Cast<UImage>(GetWidgetFromName(FName(TEXT("Iron9Frame"))));

	mIron5Frame->SetVisibility(ESlateVisibility::Hidden);
	mIron6Frame->SetVisibility(ESlateVisibility::Hidden);
	mIron7Frame->SetVisibility(ESlateVisibility::Hidden);
	mIron8Frame->SetVisibility(ESlateVisibility::Hidden);
	mIron9Frame->SetVisibility(ESlateVisibility::Hidden);
}

void UGolfClubBase::OnDriverButtonClicked()
{
	mDriverButton->SetIsEnabled(false);
	mWoodButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(true);
	mWedgeButton->SetIsEnabled(true);
	mPutterButton->SetIsEnabled(true);

	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Driver);
}

void UGolfClubBase::OnWoodButtonClicked()
{
	mWoodButton->SetIsEnabled(false);
	mDriverButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(true);
	mWedgeButton->SetIsEnabled(true);
	mPutterButton->SetIsEnabled(true);

	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wood);
}

void UGolfClubBase::OnIronButtonClicked()
{
	mWoodButton->SetIsEnabled(true);
	mDriverButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(false);
	mWedgeButton->SetIsEnabled(true);
	mPutterButton->SetIsEnabled(true);

	SetIronButtonVisible(true);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Iron);
}

void UGolfClubBase::OnWedgeButtonClicked()
{
	mWoodButton->SetIsEnabled(true);
	mDriverButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(true);
	mWedgeButton->SetIsEnabled(false);
	mPutterButton->SetIsEnabled(true);

	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wedge);
}

void UGolfClubBase::OnPutterButtonClicked()
{
	mWoodButton->SetIsEnabled(true);
	mDriverButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(true);
	mWedgeButton->SetIsEnabled(true);
	mPutterButton->SetIsEnabled(false);

	SetIronButtonVisible(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Putter);
}

void UGolfClubBase::OnIron5ButtonClicked()
{
	mIron5Button->SetIsEnabled(false);
	mIron6Button->SetIsEnabled(true);
	mIron7Button->SetIsEnabled(true);
	mIron8Button->SetIsEnabled(true);
	mIron9Button->SetIsEnabled(true);


	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron5);
}

void UGolfClubBase::OnIron6ButtonClicked()
{
	mIron5Button->SetIsEnabled(true);
	mIron6Button->SetIsEnabled(false);
	mIron7Button->SetIsEnabled(true);
	mIron8Button->SetIsEnabled(true);
	mIron9Button->SetIsEnabled(true);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron6);
}

void UGolfClubBase::OnIron7ButtonClicked()
{
	mIron5Button->SetIsEnabled(true);
	mIron6Button->SetIsEnabled(true);
	mIron7Button->SetIsEnabled(false);
	mIron8Button->SetIsEnabled(true);
	mIron9Button->SetIsEnabled(true);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron7);
}

void UGolfClubBase::OnIron8ButtonClicked()
{
	mIron5Button->SetIsEnabled(true);
	mIron6Button->SetIsEnabled(true);
	mIron7Button->SetIsEnabled(true);
	mIron8Button->SetIsEnabled(false);
	mIron9Button->SetIsEnabled(true);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron8);
}

void UGolfClubBase::OnIron9ButtonClicked()
{
	mIron5Button->SetIsEnabled(true);
	mIron6Button->SetIsEnabled(true);
	mIron7Button->SetIsEnabled(true);
	mIron8Button->SetIsEnabled(true);
	mIron9Button->SetIsEnabled(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetIronType(EIronType::Iron9);
}

void UGolfClubBase::SetPuttingClub(bool isPutting)
{
	if (isPutting)
	{
		mDriverButton->SetVisibility(ESlateVisibility::Hidden);
		mWoodButton->SetVisibility(ESlateVisibility::Hidden);
		mIronButton->SetVisibility(ESlateVisibility::Hidden);
		mWedgeButton->SetVisibility(ESlateVisibility::Hidden);

		mDriverFrame->SetVisibility(ESlateVisibility::Hidden);
		mWoodFrame->SetVisibility(ESlateVisibility::Hidden);
		mIronFrame->SetVisibility(ESlateVisibility::Hidden);
		mWedgeFrame->SetVisibility(ESlateVisibility::Hidden);

		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
			Ball->SetGolfClubType(EGolfClub::Putter);

		mPutterButton->SetIsEnabled(false);
	}
	else
	{
		mDriverButton->SetVisibility(ESlateVisibility::Visible);
		mWoodButton->SetVisibility(ESlateVisibility::Visible);
		mIronButton->SetVisibility(ESlateVisibility::Visible);
		mWedgeButton->SetVisibility(ESlateVisibility::Visible);

		mDriverFrame->SetVisibility(ESlateVisibility::Visible);
		mWoodFrame->SetVisibility(ESlateVisibility::Visible);
		mIronFrame->SetVisibility(ESlateVisibility::Visible);
		mWedgeFrame->SetVisibility(ESlateVisibility::Visible);
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

		mIron5Frame->SetVisibility(ESlateVisibility::Visible);
		mIron6Frame->SetVisibility(ESlateVisibility::Visible);
		mIron7Frame->SetVisibility(ESlateVisibility::Visible);
		mIron8Frame->SetVisibility(ESlateVisibility::Visible);
		mIron9Frame->SetVisibility(ESlateVisibility::Visible);
	}

	else
	{
		mIron5Button->SetVisibility(ESlateVisibility::Hidden);
		mIron6Button->SetVisibility(ESlateVisibility::Hidden);
		mIron7Button->SetVisibility(ESlateVisibility::Hidden);
		mIron8Button->SetVisibility(ESlateVisibility::Hidden);
		mIron9Button->SetVisibility(ESlateVisibility::Hidden);

		mIron5Frame->SetVisibility(ESlateVisibility::Hidden);
		mIron6Frame->SetVisibility(ESlateVisibility::Hidden);
		mIron7Frame->SetVisibility(ESlateVisibility::Hidden);
		mIron8Frame->SetVisibility(ESlateVisibility::Hidden);
		mIron9Frame->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGolfClubBase::ResetClubButtons()
{
	mWoodButton->SetIsEnabled(true);
	mDriverButton->SetIsEnabled(true);
	mIronButton->SetIsEnabled(true);
	mWedgeButton->SetIsEnabled(true);
	mPutterButton->SetIsEnabled(true);

	mIron5Button->SetIsEnabled(true);
	mIron6Button->SetIsEnabled(true);
	mIron7Button->SetIsEnabled(true);
	mIron8Button->SetIsEnabled(true);
	mIron9Button->SetIsEnabled(true);

	SetIronButtonVisible(false);
}
