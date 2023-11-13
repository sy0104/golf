#include "Ball.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;
	mRoot->SetSphereRadius(2.25f);

	// Static Mesh
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mStaticMesh->SetupAttachment(mRoot);

	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
		mStaticMesh->SetStaticMesh(mesh);

	// Camera & Spring Arm
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//mSpringArm->TargetArmLength = 100.f;
	//mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 8.0));
	//mSpringArm->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	//
	//mSpringArm->SetupAttachment(mRoot);
	//mCamera->SetupAttachment(mSpringArm);

	// Projectile
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(mRoot);
	mProjectile->MaxSpeed = 100000.f;

	SetActorLocation(FVector(0.0, 0.0, 16.5));
	//SetActorScale3D(FVector(5.0, 5.0, 5.0));
	SetActorScale3D(FVector(3.0, 3.0, 3.0));

	//APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	//mCamera->bUsePawnControlRotation = true;
	//mSpringArm->bUsePawnControlRotation = true;

	// Collision
	mRoot->SetCollisionProfileName(TEXT("Ball"));
	mRoot->SetSimulatePhysics(true);
	mRoot->SetLinearDamping(0.0f);
	
	// Camera Offset
	mCameraOffset = FVector(-120.0, 0.0, 45.0);
	mCameraRotation = FRotator(0.0, 0.0, 0.0);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//PlayerController->PlayerCameraManager->ViewPitchMin = 0.f;
	//PlayerController->PlayerCameraManager->ViewPitchMax = 0.f;
	//PlayerController->PlayerCameraManager->ViewRollMin = 0.f;
	//PlayerController->PlayerCameraManager->ViewRollMax = 0.f;
	//PlayerController->PlayerCameraManager->ViewYawMin = 0.f;
	//PlayerController->PlayerCameraManager->ViewYawMax = 0.f;

}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PrintViewport(1.f, FColor::Red, TEXT("Tick"));

	SetCamera();
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<ABall>(TEXT("Swing"), EInputEvent::IE_Pressed, this, &ABall::Swing);
}

void ABall::SetCamera()
{
	FVector location = GetActorLocation() + mCameraOffset;

	mCamera->SetRelativeLocation(location);
	mCamera->SetRelativeRotation(mCameraRotation);
}

void ABall::Swing()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Ball::Swing"));

	mProjectile->InitialSpeed = 80000.f;
	
	FVector StartLoc = GetActorLocation();
	//FVector TargetLoc = FVector(500, 0, 0);
	FVector TargetLoc = FVector(10000, 0, 0);
	float arcValue = 0.3f;
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartLoc, TargetLoc, GetWorld()->GetGravityZ(), arcValue);

	//if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(
	//	this, outVelocity, StartLoc, TargetLoc, GetWorld()->GetGravityZ(), arcValue))
	//{
	//	FPredictProjectilePathParams predictParams(20.0f, StartLoc, outVelocity, 15.0f);
	//	predictParams.DrawDebugTime = 15.0f;
	//	predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
	//	predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();

	//	FPredictProjectilePathResult result;
	//	UGameplayStatics::PredictProjectilePath(this, predictParams, result);
	//}

	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Velocity X: %f"), outVelocity.X));
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Velocity Y: %f"), outVelocity.Y));
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Velocity Z: %f"), outVelocity.Z));

	mRoot->AddImpulse(outVelocity);

	//mProjectile->ProjectileGravityScale = 0.1f;
	//mProjectile->bShouldBounce = true;
}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

