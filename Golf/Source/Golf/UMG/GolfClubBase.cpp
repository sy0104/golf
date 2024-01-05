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
}

void UGolfClubBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGolfClubBase::OnDriverButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Driver);
}

void UGolfClubBase::OnWoodButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wood);
}

void UGolfClubBase::OnIronButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Iron);
}

void UGolfClubBase::OnWedgeButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Wedge);
}

void UGolfClubBase::OnPutterButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetGolfClubType(EGolfClub::Putter);
}

void UGolfClubBase::SetPuttingClub(bool isPutting)
{
	if (isPutting)
	{
		mDriverButton->SetVisibility(ESlateVisibility::Hidden);
		mWoodButton->SetVisibility(ESlateVisibility::Hidden);
		mIronButton->SetVisibility(ESlateVisibility::Hidden);
		mWedgeButton->SetVisibility(ESlateVisibility::Hidden);

		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
			Ball->SetGolfClubType(EGolfClub::Putter);
	}
	else
	{
		mDriverButton->SetVisibility(ESlateVisibility::Visible);
		mWoodButton->SetVisibility(ESlateVisibility::Visible);
		mIronButton->SetVisibility(ESlateVisibility::Visible);
		mWedgeButton->SetVisibility(ESlateVisibility::Visible);
	}




}
