// Fill out your copyright notice in the Description page of Project Settings.


#include "GFGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallController.h"
#include "UMG/StartSceneBase.h"
#include "UMG/MainHUDBase.h"

AGFGameModeBase::AGFGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ABall::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();

	// Start Scene UI Class
	ConstructorHelpers::FClassFinder<UUserWidget>
		StartFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_StartScene.UI_StartScene_C'"));
	if (StartFinder.Succeeded())
		mStartSceneUIClass = StartFinder.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> 
		MainFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (MainFinder.Succeeded())
		mMainHUDClass = MainFinder.Class;
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

	else if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Main" && IsValid(mMainHUDClass))
	{
		mMainHUD = Cast<UMainHUDBase>(CreateWidget(GetWorld(), mMainHUDClass));

		if (IsValid(mMainHUD))
		{
			mMainHUD->AddToViewport();
		}
	}
}