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
	mProjectile->Friction = 0.8f;

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
	mCameraOffset = FVector(-150.0, 0.0, 30.0);
	mCameraRotation = FRotator(0.0, 0.0, 0.0);

	// Swing
	mSwingArc = 0.3f;
	mStartPos = FVector(0.0, 0.0, 0.0);
	mStartToTarget = mStartPos + FVector(10000, 0, 0);
	mTargetPos = mStartPos + mStartToTarget;

	// Physics
	mInitialSpeed = 1000.f;		// ??
	mGravityScale = 1.0f;
	mIsBounce = true;
	mBounciness = 0.6f;
	mFriction = 0.8f;
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

	float dis = GetDistanceToTarget(GetActorLocation());
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dis: %f"), dis));
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction<ABall>(TEXT("Swing"), EInputEvent::IE_Pressed, this, &ABall::SwingStraight);
	PlayerInputComponent->BindAction<ABall>(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ABall::Roll);
}

void ABall::SetCamera()
{
	FVector location = GetActorLocation() + mCameraOffset;

	mCamera->SetRelativeLocation(location);
	mCamera->SetRelativeRotation(mCameraRotation);
}

void ABall::SwingStraight()
{
	PrintViewport(1.f, FColor::Red, TEXT("Swing"));

	mProjectile->InitialSpeed = 5000.f;

	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.6f;
	mProjectile->Friction = 0.8f;
	mSwingArc = 0.3f;
	
	FVector StartLoc = GetActorLocation();
	// FVector TargetLoc = StartLoc + FVector(10000, 0, 0);
	FVector TargetLoc = FVector(5000, 0, 0);
	FVector outVelocity = FVector::ZeroVector;

	mStartPos = GetActorLocation();
	mTargetPos = mStartPos + mStartToTarget;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, mStartPos, TargetLoc, GetWorld()->GetGravityZ(), mSwingArc);


	//GetDistanceTo

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

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingLeft()
{

}

void ABall::SwingRight()
{
}

void ABall::Roll()
{
	PrintViewport(1.f, FColor::Red, TEXT("Roll"));

	mProjectile->bShouldBounce = false;
	mProjectile->Bounciness = 0.f;
	mProjectile->Friction = 1.f;
	mSwingArc = 0.8f;

	FVector StartLoc = GetActorLocation();
	FVector TargetLoc = StartLoc + FVector(300, 0, 0);
	FVector outVelocity = FVector::ZeroVector;

	mStartPos = GetActorLocation();
	mTargetPos = mStartPos + mStartToTarget;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, mStartPos, mTargetPos, GetWorld()->GetGravityZ(), mSwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

float ABall::GetDistanceToTarget(FVector pos)
{
	float dis = (pos - mTargetPos).Size();

	if (dis < 0.f)
		dis = 0.f;

	return dis;
}

