#include "BallDetailBase.h"
#include "../Ball/Ball.h"

void UBallDetailBase::NativeConstruct()
{
	Super::NativeConstruct();

	mPowerCheckBox = Cast<UCheckBox>(GetWidgetFromName(FName(TEXT("PowerCheckBox"))));
	mSpinCheckBox = Cast<UCheckBox>(GetWidgetFromName(FName(TEXT("SpinCheckBox"))));

	mPowerCheckBox->OnCheckStateChanged.AddDynamic(this, &UBallDetailBase::OnPowerCheckBoxChanged);
	mSpinCheckBox->OnCheckStateChanged.AddDynamic(this, &UBallDetailBase::OnSpinCheckBoxChanged);
}

void UBallDetailBase::OnPowerCheckBoxChanged(bool IsChecked)
{
	mSpinCheckBox->SetIsChecked(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
	{
		if (IsChecked)
		{
			Ball->SetIsAddPower(true);
			Ball->SetIsAddSpin(false);
		}

		else
			Ball->SetIsAddPower(false);
	}
}

void UBallDetailBase::OnSpinCheckBoxChanged(bool IsChecked)
{
	mPowerCheckBox->SetIsChecked(false);

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
	{
		if (IsChecked)
		{
			Ball->SetIsAddSpin(true);
			Ball->SetIsAddPower(false);
		}

		else
			Ball->SetIsAddSpin(false);
	}
}

void UBallDetailBase::SetPowerCheckBoxChecked(bool IsChecked)
{
	mPowerCheckBox->SetIsChecked(IsChecked);
}

void UBallDetailBase::SetSpinCheckBoxChecked(bool IsChecked)
{
	mSpinCheckBox->SetIsChecked(IsChecked);
}
