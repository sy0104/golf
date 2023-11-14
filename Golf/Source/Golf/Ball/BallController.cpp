// Fill out your copyright notice in the Description page of Project Settings.

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

void ABallController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	//PrintViewport(1.f, FColor::Red, TEXT("OnPossess"));
}

void ABallController::OnUnPossess()
{
	Super::OnUnPossess();

	//PrintViewport(1.f, FColor::Red, TEXT("OnUnPossess"));
}
