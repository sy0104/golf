#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mStaticMesh->SetupAttachment(mRoot);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mCamera->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	// Mesh
	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
		mStaticMesh->SetStaticMesh(mesh);

	mRoot->SetSphereRadius(2.25f);

	// (X=3180.000000,Y=-800.000000,Z=214.250000)

	SetActorLocation(FVector(0.0, 0.0, 0.0));
	SetActorScale3D(FVector(10.0, 10.0, 10.0));

	mCamera->SetRelativeLocation(FVector(-70.0, 0.0, 20.0));
	mCamera->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PrintViewport(1.f, FColor::Red, TEXT("Tick"));

}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

