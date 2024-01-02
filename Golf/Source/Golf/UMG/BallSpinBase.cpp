#include "BallSpinBase.h"
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
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Left);
}

void UBallSpinBase::OnSwingStraightButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Straight);
}

void UBallSpinBase::OnSwingRightButtonClicked()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetBallSwingType(EBallSwingType::Right);
}
