#include "BallController.h"

ABallController::ABallController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABallController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
	bShowMouseCursor = true;
}

void ABallController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallController::SetupInputComponent()
{
	Super::SetupInputComponent();
}