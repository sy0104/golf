#include "FixedCamera.h"

AFixedCamera::AFixedCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	// root
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	// camera
	mFixedCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FixedCamera"));
	mFixedCamera->SetupAttachment(mRoot);
	mFixedCamera->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	mFixedCamera->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	//mFixedCamera->bConstrainAspectRatio = true;
}

void AFixedCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFixedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

