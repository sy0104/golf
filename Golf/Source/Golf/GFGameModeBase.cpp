// Fill out your copyright notice in the Description page of Project Settings.


#include "GFGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallController.h"

AGFGameModeBase::AGFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FClassFinder<ABall> BallFinder(
	//	TEXT("/Script/Engine.Blueprint'/Game/Ball/BPC_Ball.BPC_Ball_C'"));

	//if (BallFinder.Succeeded())
	//	DefaultPawnClass = BallFinder.Class;

	//else
	//	DefaultPawnClass = ABall::StaticClass();

	DefaultPawnClass = ABall::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();

	//ABall* ball = NewObject<ABall>();
}
