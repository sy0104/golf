#include "Ball.h"
#include "../GFGameModeBase.h"
#include "../UMG/DistanceBase.h"
#include "../UMG/MainHUDBase.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	mRoot = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
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
	mCamera->SetupAttachment(mSpringArm);
	//mCamera->SetupAttachment(mRoot);

	mSpringArm->TargetArmLength = 50.f;
	mCamera->SetRelativeLocation(FVector(-230.0, 0.0, 85.0));
	//mSpringArm->SetRelativeLocation(FVector(0.0, 80.0, 0.0));
	//mSpringArm->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));


	mSpringArm->bUsePawnControlRotation = false;
	mSpringArm->bInheritPitch = false;
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;

	// Projectile
	mProjectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	mProjectile->SetUpdatedComponent(mRoot);
	mProjectile->Friction = 30.f;

	//// Rotating Movement
	//mRotating = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
	//mRotating->SetUpdatedComponent(mRoot);

	// Collision
	mRoot->SetCollisionProfileName(TEXT("Ball"));
	mRoot->SetSimulatePhysics(true);
	mRoot->SetLinearDamping(0.0f);
	
	// Camera Offset
	mCameraOffset = FVector(-150.0, 0.0, 30.0);
	mCameraRotation = FRotator(0.0, 0.0, 0.0);

	// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.TargetPos = FVector(43000.0, 0.0, 0.0);

	mBallInfo.TargetDir = mBallInfo.TargetPos - mBallInfo.StartPos;
	mBallInfo.TargetDir.Normalize();
	mBallInfo.SpinForce = 2000.f;

	mBallInfo.BallPower = 0.0;
	mBallInfo.BallMinPower = 0.0;
	mBallInfo.BallMaxPower = 190.0;
	mBallInfo.BallDir = 0.0;

	mBallInfo.SwingArc = 0.3f;

	// spin
	mIsSwingStraight = true;
	mIsSwingLeft = false;
	mIsSwingRight = false;

	// power
	mIsPowerUp = true;
	mAddPower = 10.0;
	mTempBallPower = mBallInfo.BallPower;

	// Init
	SetActorLocation(mBallInfo.StartPos);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));

	mRoot->SetLinearDamping(0.f);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		mMainHUD = GameMode->GetMainHUD();
	}

	//mProjectile->OnProjectileStop.AddDynamic(this, &ABall::BallBounced);
	mProjectile->OnProjectileBounce.AddDynamic(this, &ABall::BallBounced);
	mProjectile->InitialSpeed = 100.f;
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetCamera();

	//float dis = GetDistanceToTarget(GetActorLocation());
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dis: %f"), dis));

	if (mIsSwingLeft || mIsSwingRight)
		AddForceToSide();
	else if (mIsSwingStraight)
		AddForceToStraight();

	ShowDistance();
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 액션 매핑
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingStraight"), EInputEvent::IE_Pressed, this, &ABall::SwingStraight);
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingLeft"), EInputEvent::IE_Pressed, this, &ABall::SwingLeft);
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingRight"), EInputEvent::IE_Pressed, this, &ABall::SwingRight);
	PlayerInputComponent->BindAction<ABall>(TEXT("Roll"), EInputEvent::IE_Pressed, this, &ABall::Roll);
	PlayerInputComponent->BindAction<ABall>(TEXT("PrintPower"), EInputEvent::IE_Pressed, this, &ABall::PrintPower);

	//// 축 매핑
	//PlayerInputComponent->BindAxis<ABall>(TEXT("MoveFront"), this, &ABall::MoveFront);
	PlayerInputComponent->BindAxis<ABall>(TEXT("SwingDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
}

void ABall::SetCamera()
{
	FVector location = GetActorLocation() + mCameraOffset;

	mCamera->SetRelativeLocation(location);
	mCamera->SetRelativeRotation(mCameraRotation);
}

void ABall::SwingStraight()
{
	//PrintViewport(1.f, FColor::Red, TEXT("SwingStraight"));

	// Projectile
	mProjectile->bShouldBounce = false;
	mProjectile->Bounciness = 0.0f;
	mProjectile->Friction = 300.f;

	mProjectile->bBounceAngleAffectsFriction = true;

	// Spin
	mIsSwingStraight = true;
	mIsSwingRight = false;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.3f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	//FVector TargetPos = StartPos + FVector(mBallInfo.BallPower + mAddPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	// 
	//mBallInfo.BallPower = 890.0;
	// FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector TargetPos =  GetActorLocation() + FVector(100.0, mBallInfo.BallDir - 180.0, 0.0);
	//FVector TargetPos = GetActorLocation() + FVector(100.0, -180.0, 0.0);
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dir: %f"), mBallInfo.BallDir));

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingLeft()
{
	//PrintViewport(1.f, FColor::Red, TEXT("SwingLeft"));

	// Projectile
	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.5f;
	mProjectile->Friction = 0.8f;

	// Spin
	mBallInfo.SpinForce = 200.f;
	mIsSwingStraight = false;
	mIsSwingRight = false;
	mIsSwingLeft = true;

	// Ball Info
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.4f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower + mAddPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingRight()
{
	//PrintViewport(1.f, FColor::Red, TEXT("SwingRight"));

	// Projectile
	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.5f;
	mProjectile->Friction = 0.8f;

	// Spin
	mBallInfo.SpinForce = 200.f;
	mIsSwingStraight = false;
	mIsSwingRight = true;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.4f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower + mAddPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::Roll()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Roll"));

	// Projectile
	mProjectile->bShouldBounce = false;
	mProjectile->Bounciness = 0.f;
	mProjectile->Friction = 1.5f;

	// Spin
	mIsSwingRight = false;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.SwingArc = 0.9f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(100, 0, 0);	// test
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity / 4);
}

void ABall::SetSwingDir(float scale)
{
	if (scale == 0.f)
		return;

	if (scale == -1.f)
	{
		if (mBallInfo.BallDir > 0)
			mBallInfo.BallDir -= 10.0;

		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dir: %f"), mBallInfo.BallDir));
	}

	else
	{
		if (mBallInfo.BallDir < 360.0)
			mBallInfo.BallDir += 10.0;
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Dir: %f"), mBallInfo.BallDir));
	}

	if (IsValid(mMainHUD))
	{
		float ratio = mBallInfo.BallDir / 360.f;
		mMainHUD->SetBallDir(ratio);
	}
}

void ABall::AddForceToStraight()
{
	//mProjectile->ConstrainDirectionToPlane(GetActorForwardVector());
}

void ABall::AddForceToSide()
{
	FVector AngularVelocityDelta = mRoot->GetPhysicsAngularVelocityInDegrees();
	AngularVelocityDelta.Normalize();

	FVector CompVelocityDelta = mRoot->GetComponentVelocity();
	CompVelocityDelta.Normalize();

	FVector TargetDir = mBallInfo.TargetPos - GetActorLocation();
	TargetDir.Normalize();

	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();
	FVector CrossPrdt;

	if (mIsSwingLeft)
	{
		FVector ForwardVector = TargetDir.ForwardVector;
		FVector LeftVec = TargetDir.LeftVector;
		//CrossPrdt = FVector::CrossProduct(mBallInfo.TargetDir, LeftVec);
		CrossPrdt = FVector::CrossProduct(AngularVelocityDelta, CompVelocityDelta); 
	}

	else if (mIsSwingRight)
	{
		FVector RightVec = FVector(1.0, 0.0, 0.0);
		//CrossPrdt = FVector::CrossProduct(mBallInfo.TargetDir, RightVec);
		CrossPrdt = FVector::CrossProduct(TargetDir.ForwardVector, TargetDir.RightVector);
	}

	mRoot->AddForce(CrossPrdt * mBallInfo.SpinForce);
}

void ABall::MoveFront(float scale)
{
	if (scale == 0.f)
		return;

	PrintViewport(1.f, FColor::Red, TEXT("MoveFront"));

	AddMovementInput(GetActorForwardVector(), scale);
}

void ABall::MoveSide(float scale)
{
	if (scale == 0.f)
		return;

	PrintViewport(1.f, FColor::Red, TEXT("MoveSide"));

	AddMovementInput(GetActorRightVector(), scale);
}

void ABall::AddBallPower(float scale)
{
	if (scale == 0.f)
		return;

	// 5000 ~ 30000

	if (mIsPowerUp)
	{
		mTempBallPower += 10.0;

		if (mTempBallPower >= mBallInfo.BallMaxPower)
			mIsPowerUp = false;
	}

	else
	{
		mTempBallPower -= 10.0;

		if (mTempBallPower <= mBallInfo.BallMinPower)
			mIsPowerUp = true;
	}

	if (IsValid(mMainHUD))
	{
		float ratio = mTempBallPower / mBallInfo.BallMaxPower;
		mMainHUD->SetBallPower(ratio);
	}

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("power: %f"), mBallInfo.BallPower));
}

void ABall::ShowDistance()
{
	float leftDis = GetDistanceToTarget(GetActorLocation());
	float dis = GetActorLocation().X;

	if (IsValid(mMainHUD))
	{
		//leftDis = floor(leftDis * 1000);
		//dis = floor(dis * 1000);

		mMainHUD->SetLeftDistanceText(leftDis / 100.f);
		mMainHUD->SetDistanceText(dis / 100.f);
	}
}

void ABall::PrintPower()
{
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("power: %f"), mBallInfo.BallPower));
}

void ABall::BallBounced(const FHitResult& Hit, const FVector& ImpactVelocity)
{
	PrintViewport(1.f, FColor::Red, TEXT("Bounced"));
}

void ABall::BallStopped()
{
	PrintViewport(1.f, FColor::Red, TEXT("Stopped"));
}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

float ABall::GetDistanceToTarget(FVector pos)
{
	float dis = (pos - mBallInfo.TargetPos).Size();

	if (dis < 0.f)
		dis = 0.f;

	return dis;
}

