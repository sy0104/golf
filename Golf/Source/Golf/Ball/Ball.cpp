#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	mStaticMesh->SetupAttachment(mRoot);

	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->SetupAttachment(mStaticMesh);
	mCamera->SetupAttachment(mSpringArm);

	mRoot->bVisualizeComponent = true;
	mRoot->SetSphereRadius(2.25f);

	// Mesh
	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
		mStaticMesh->SetStaticMesh(mesh);

	SetActorLocation(FVector(0.0, 0.0, 0.0));
	SetActorScale3D(FVector(10.0, 10.0, 10.0));

	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 8.0));
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

	PlayerInputComponent->BindAction<ABall>(TEXT("Swing"), EInputEvent::IE_Pressed, this, &ABall::Swing);
}

void ABall::Swing()
{
	PrintViewport(1.f, FColor::Red, TEXT("Swing"));
	
}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

