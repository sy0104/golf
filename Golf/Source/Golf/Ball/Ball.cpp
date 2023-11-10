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
	mSpringArm->SetupAttachment(mStaticMesh);
	//mSpringArm->TargetArmLength
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 8.0));
	mCamera->SetupAttachment(mSpringArm);

	// Projectile
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(mRoot);


	SetActorLocation(FVector(0.0, 0.0, 0.0));
	//SetActorScale3D(FVector(5.0, 5.0, 5.0));
	SetActorScale3D(FVector(3.0, 3.0, 3.0));

	// Collision
	mRoot->SetCollisionProfileName(TEXT("Ball"));

	mRoot->SetSimulatePhysics(true);
	mRoot->SetLinearDamping(0.0f);
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
	PrintViewport(1.f, FColor::Red, TEXT("Ball::Swing"));

	FVector StartLoc = GetActorLocation();
	//FVector TargetLoc = FVector(500, 0, 0);
	FVector TargetLoc = FVector(500, 500, 0);
	float arcValue = 0.5f;
	FVector outVelocity = FVector::ZeroVector;

	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartLoc, TargetLoc, GetWorld()->GetGravityZ(), arcValue))
	{
		FPredictProjectilePathParams predictParams(20.0f, StartLoc, outVelocity, 15.0f);
		predictParams.DrawDebugTime = 15.0f;
		predictParams.DrawDebugType = EDrawDebugTrace::Type::ForDuration;
		predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();

		FPredictProjectilePathResult result;
		UGameplayStatics::PredictProjectilePath(this, predictParams, result);
	}

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

