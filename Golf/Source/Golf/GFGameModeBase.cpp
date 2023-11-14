// Fill out your copyright notice in the Description page of Project Settings.


#include "GFGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallController.h"
#include "UMG/StartSceneBase.h"

AGFGameModeBase::AGFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ABall::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();

	// Start Scene UI Class
	ConstructorHelpers::FClassFinder<UUserWidget>
		Finder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_StartScene.UI_StartScene_C'"));
	if (Finder.Succeeded())
		mStartSceneUIClass = Finder.Class;
}

void AGFGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Start" && IsValid(mStartSceneUIClass))
	{
		mStartSceneUI = Cast<UStartSceneBase>(CreateWidget(GetWorld(), mStartSceneUIClass));

		if (IsValid(mStartSceneUI))
		{
			mStartSceneUI->AddToViewport();
		}
	}
}