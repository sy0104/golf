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
	//const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_GolfTee.SM_GolfTee'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
		mStaticMesh->SetStaticMesh(mesh);

	// Camera & Spring Arm
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	
	mSpringArm->SetupAttachment(mStaticMesh);
	mCamera->SetupAttachment(mSpringArm);

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
	mProjectile->OnProjectileBounce.AddDynamic(this, &ABall::BallBounced);
	mProjectile->OnProjectileStop.AddDynamic(this, &ABall::BallStopped);

	// Rotating Movement
	//mRotating = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating"));
	//mRotating->SetUpdatedComponent(mRoot);
	//mRotating->RotationRate = FRotator(0.0, 0.0, 0.0);

	// Collision
	mRoot->SetCollisionProfileName(TEXT("Ball"));
	mRoot->SetSimulatePhysics(true);
	mRoot->SetLinearDamping(0.0f);

	// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.TargetPos = FVector(43000.0, 0.0, 0.0);

	mBallInfo.TargetDir = mBallInfo.TargetPos - mBallInfo.StartPos;
	mBallInfo.TargetDir.Normalize();
	mBallInfo.SpinForce = 1000.f;

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

	mIsGround = true;

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

	mRoot->OnComponentHit.AddDynamic(this, &ABall::OnHit);
	mProjectile->InitialSpeed = 100.f;
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mIsSwingLeft || mIsSwingRight)
		AddForceToSide();
	else if (mIsSwingStraight)
		AddForceToStraight();

	ShowDistance();

	CheckMaterialCollision();
	CheckLandscapeCollision();

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("dir: %f"), mBallInfo.BallDir));

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

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("SwingDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
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
	mBallInfo.StartPos = GetActorLocation();
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.3f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower + mAddPower, mBallInfo.BallDir, 0.0);
	//FVector TargetPos = GetActorLocation() + FVector(50.0, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	mBallInfo.TargetPos = TargetPos;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingLeft()
{
	// Projectile
	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.5f;
	mProjectile->Friction = 0.8f;

	// Spin
	mBallInfo.StartPos = GetActorLocation();
	mBallInfo.SpinForce = 1000.f;
	mIsSwingStraight = false;
	mIsSwingRight = false;
	mIsSwingLeft = true;

	// Ball Info
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower + mAddPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	mBallInfo.TargetPos = TargetPos;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingRight()
{
	// Projectile
	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.5f;
	mProjectile->Friction = 0.8f;

	// Spin
	mBallInfo.StartPos = GetActorLocation();
	mBallInfo.SpinForce = 1000.f;
	mIsSwingStraight = false;
	mIsSwingRight = true;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.SwingArc = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower + mAddPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	mBallInfo.TargetPos = TargetPos;

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
	FVector TargetPos = StartPos + FVector(50, 0, 0);	// test
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity / 4);
}

void ABall::SetSwingDir(float scale)
{
	if (scale == 0.f)
		return;

	if (scale == -1.f/* && mBallInfo.BallDir > -90.0*/)
	{
		mBallInfo.BallDir -= 1.0;
	}

	else if (scale == 1.f/* && mBallInfo.BallDir < 90.0*/)
	{
		mBallInfo.BallDir += 1.0;
	}

	FRotator springArmRot = FRotator(0.0, mBallInfo.BallDir, 0.0);
	mSpringArm->SetRelativeRotation(springArmRot);
}

void ABall::AddForceToStraight()
{
	//mProjectile->ConstrainDirectionToPlane(GetActorForwardVector());

}

void ABall::AddForceToSide()
{
	FVector forwardVec = mBallInfo.TargetPos;
	forwardVec.Normalize();

	FVector leftVec = FVector(GetActorLocation().X, GetActorLocation().Y - 90.0, GetActorLocation().Z);
	leftVec.Normalize();

	FVector CrossPrdt = FVector::CrossProduct(forwardVec, leftVec);

	if (mIsSwingRight)
		CrossPrdt.Y *= -1.0;

	mRoot->AddForce(CrossPrdt * 100.0);
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
	float leftDis = GetDistanceToTarget(GetActorLocation(), mBallInfo.TargetPos);
	// float dis = GetActorLocation().X;

	float dis = GetDistanceToTarget(mBallInfo.StartPos, GetActorLocation());

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetLeftDistanceText(leftDis / 100.f);
		mMainHUD->SetDistanceText(dis / 100.f);
	}
}

void ABall::PrintPower()
{
	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("power: %f"), mBallInfo.BallPower));
}

void ABall::CheckMaterialCollision()
{
	FVector startPos = GetActorLocation();
	FVector endPos = GetActorLocation() - FVector(0.0, 0.0, startPos.Z);

	FHitResult hitResult;
	FCollisionQueryParams collisionParams(NAME_None, false, this);
	collisionParams.bReturnPhysicalMaterial = true;

	bool hit = GetWorld()->LineTraceSingleByChannel(hitResult, startPos, endPos, ECC_GameTraceChannel12, collisionParams);
	//DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, false, 1.0f);

	if (hit)
	{
		UObject* material = Cast<UObject>(hitResult.PhysMaterial.Get());

		//UPhysicalMaterial* pm = Cast<UPhysicalMaterial>(material);
		//pm->Restitution = 0.3f;

		if (IsValid(material))
		{
			mHitMaterialName = material->GetName();
			SetBallDetail();
		}

		FVector playerPos = GetActorLocation();
		if (playerPos.Z < 3.0)
		{
			mIsGround = true;
			// PrintViewport(1.f, FColor::Red, TEXT("Ground"));
		}

		else
		{
			mIsGround = false;
		}
	}
}

void ABall::CheckLandscapeCollision()
{
	if (!mIsGround)
		return;

	//mIsSwingStraight = false;
	//mIsSwingLeft = false;
	//mIsSwingRight = false;

	//mProjectile->StopMovementImmediately();
}

void ABall::SetBallDetail()
{
	PrintViewport(1.f, FColor::Red, mHitMaterialName);

	if (mHitMaterialName == L"PM_LandscapeBase")
	{

	}

	else if (mHitMaterialName == L"PM_LandscapeFairway")
	{

	}

	else if (mHitMaterialName == L"PM_LandscapeGreen")
	{

	}

	else if (mHitMaterialName == L"PM_LandscapeWater")
	{

	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{

	}

	else if (mHitMaterialName == L"PM_LandscapeLine")
	{

	}
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, const FHitResult& Hit)
{
	PrintViewport(1.f, FColor::Red, TEXT("OnHit"));
}

void ABall::BallBounced(const FHitResult& Hit, const FVector& ImpactVelocity)
{
	PrintViewport(1.f, FColor::Blue, TEXT("Bounced"));
}

void ABall::BallStopped(const FHitResult& ImpactResult)
{
	PrintViewport(1.f, FColor::Blue, TEXT("Stopped"));
}

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

float ABall::GetDistanceToTarget(FVector src, FVector dst)
{
	float dis = (src - dst).Size();

	if (dis < 0.f)
		dis = 0.f;

	return dis;
}

