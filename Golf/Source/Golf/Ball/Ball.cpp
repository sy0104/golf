#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mStaticMesh->SetupAttachment(mRoot);

	mRoot->bVisualizeComponent = true;

	// Mesh
	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
		mStaticMesh->SetStaticMesh(mesh);

	else
		PrintViewport(1.f, FColor::Red, TEXT("NoMesh"));

	SetActorLocation(FVector(360.0, 10.0, 212.0));
	SetActorScale3D(FVector(5.0, 5.0, 5.0));
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	

}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintViewport(1.f, FColor::Red, TEXT("Tick"));

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

