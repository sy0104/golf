// Fill out your copyright notice in the Description page of Project Settings.


#include "GFGameModeBase.h"
#include "Ball/Ball.h"
#include "Ball/BallController.h"
#include "UMG/StartSceneBase.h"
#include "UMG/MainHUDBase.h"
#include "Camera/FixedCamera.h"

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

	// Start Scene UI Class
	ConstructorHelpers::FClassFinder<UUserWidget>
		StartFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_StartScene.UI_StartScene_C'"));
	if (StartFinder.Succeeded())
		mStartSceneUIClass = StartFinder.Class;

	// Main UI
	ConstructorHelpers::FClassFinder<UUserWidget> 
		MainFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UMG/UI_MainHUD.UI_MainHUD_C'"));
	if (MainFinder.Succeeded())
		mMainHUDClass = MainFinder.Class;

	// Fixed Camera
	mFixedCameraClass = AFixedCamera::StaticClass();
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

	// Fixed Camera
	mFixedCamera = NewObject<AFixedCamera>();
	mFixedCamera->SetActorLocation(FVector(-1500.0, 0.0, 1050.0));
	mFixedCamera->SetActorRotation(FRotator(-5.0, 0.0, 0.0));

	//if (IsValid(mFixedCamera))
	//{
	//	PrintViewport(1.f, FColor::Red, TEXT("Fixed Camera Valid"));
	//}
}