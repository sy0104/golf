#include "BallSpinBase.h"
#include "../Ball/Ball.h"

void UBallSpinBase::NativeConstruct()
{
	Super::NativeConstruct();

	mSwingLeftButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingLeftButton"))));
	mSwingStraightButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingStraightButton"))));
	mSwingRightButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("SwingRightButton"))));
	mBallSpinPowerBar = Cast<UProgressBar>(GetWidgetFromName(FName(TEXT("BallSpinPowerBar"))));

	mSwingLeftButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingLeftButtonClicked);
	mSwingStraightButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingStraightButtonClicked);
	mSwingRightButton->OnClicked.AddDynamic(this, &UBallSpinBase::OnSwingRightButtonClicked);
}

void UBallSpinBase::OnSwingLeftButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Left);

	mSwingLeftButton->SetIsEnabled(false);
	mSwingStraightButton->SetIsEnabled(true);
	mSwingRightButton->SetIsEnabled(true);
}

void UBallSpinBase::OnSwingStraightButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Straight);

	mSwingStraightButton->SetIsEnabled(false);
	mSwingLeftButton->SetIsEnabled(true);
	mSwingRightButton->SetIsEnabled(true);
}

void UBallSpinBase::OnSwingRightButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Right);

	mSwingRightButton->SetIsEnabled(false);
	mSwingStraightButton->SetIsEnabled(true);
	mSwingLeftButton->SetIsEnabled(true);
}

void UBallSpinBase::SetBallSpin(float ratio)
{
	mBallSpinPowerBar->SetPercent(ratio);
}

void UBallSpinBase::SetSpinButtonsEnable()
{
	mSwingLeftButton->SetIsEnabled(true);
	mSwingStraightButton->SetIsEnabled(true);
	mSwingRightButton->SetIsEnabled(true);
}
