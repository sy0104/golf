#include "Ball.h"
#include "../GFGameModeBase.h"
#include "../UMG/DistanceBase.h"
#include "../UMG/MainHUDBase.h"
#include "BallController.h"
#include "../GFGameInstance.h"
#include "../Manager/ScoreSubsystem.h"

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

	mSpringArm->TargetArmLength = 400.f;
	mCamera->SetRelativeLocation(FVector(-150.0, 0.0, 70.0));
	//mCamera->bConstrainAspectRatio = true;
		
	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));

	mSpringArm->bUsePawnControlRotation = false;
	mSpringArm->bInheritPitch = false;
	mSpringArm->bInheritYaw = false;
	mSpringArm->bInheritRoll = false;
	mSpringArm->bDoCollisionTest = false;

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
	mRoot->SetSimulatePhysics(false);
	mRoot->SetNotifyRigidBodyCollision(true);
	mRoot->SetUseCCD(true);
	//mRoot->SetLinearDamping(0.05f);
	//mProjectile->ProjectileGravityScale = 20.f;

	// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(43000.0, 0.0, 0.0);

	mBallInfo.TargetDir = mBallInfo.TargetPos - mBallInfo.StartPos;
	mBallInfo.TargetDir.Normalize();
	mBallInfo.SpinForce = 1000.f;

	mBallInfo.BallPower = 0.0;
	mBallInfo.BallMinPower = 0.0;
	mBallInfo.BallMaxPower = 1500.0;

	mBallInfo.BallHeight = 0.0;
	mBallInfo.BallMinHeight = 0.0;
	mBallInfo.BallMaxHeight = 1500.0;

	mBallInfo.BallDir = 0.0;
	mBallInfo.BallSpinDir = 0.0;

	mBallInfo.Score = -4;

	// spin
	mIsEnableSwing = true;
	mIsSwingStraight = true;
	mIsSwingLeft = false;
	mIsSwingRight = false;

	// power
	mIsPowerUp = true;
	mIsHeightUp = true;
	mIsPowerTurn = true;
	mTempBallPower = mBallInfo.BallPower;

	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;

	// Init
	SetActorLocation(mBallInfo.StartPos);
	SetActorScale3D(FVector(12.0, 12.0, 12.0));
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		mMainHUD = GameMode->GetMainHUD();
		mMainHUD->SetDistanceText(0.f);
	}

	mRoot->OnComponentHit.AddDynamic(this, &ABall::OnHit);
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

	//CheckCameraChange(DeltaTime);

	ResetBallPos(DeltaTime);
	//FindResetPos();

	//FVector target = mSpringArm->GetForwardVector();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel X: %f"), target.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Y: %f"), target.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Z: %f"), target.Z));

	//FVector loc = GetActorLocation();
	//loc.Y++;
	//SetActorLocation(loc);

	//if (mIsSwingStraight)
	//{
	//	static float power = 100.f;
	//	FVector resistance = -mRoot->GetPhysicsLinearVelocity().GetSafeNormal();
	//	mRoot->AddForce(resistance * power);
	//	power += 0.1f;
	//}

	//FVector upVec = mRoot->GetUpVector();
	//FVector forwardVec = mRoot->GetForwardVector();

	//FVector vel = mProjectile->Velocity;
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel X: %f"), forwardVec.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Y: %f"), forwardVec.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), forwardVec.Z));

	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (IsValid(BallController))
	//{
	//	FVector vec = BallController->GetControlRotation().Vector();

	//	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Controller X: %f"), vec.X));
	//	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Controller Y: %f"), vec.Y));
	//	PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Controller Z: %f"), vec.Z));
	//}
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
	PlayerInputComponent->BindAction<ABall>(TEXT("ChangeCamera"), EInputEvent::IE_Pressed, this, &ABall::ChangeCamera);
	PlayerInputComponent->BindAction<ABall>(TEXT("StopRotate"), EInputEvent::IE_Released, this, &ABall::StopRotate);
	//PlayerInputComponent->BindAction<ABall>(TEXT("BallDetail"), EInputEvent::IE_Pressed, this, &ABall::);

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("SwingDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDetail"), this, &ABall::AddBallPower);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDetail"), this, &ABall::AddBallHeight);
}

void ABall::SwingStraight()
{
	//if (!mIsEnableSwing)
	//	return;

	//PrintViewport(1.f, FColor::Red, TEXT("SwingStraight"));

	// Projectile
	//mProjectile->bShouldBounce = false;
	//mProjectile->Bounciness = 0.0f;
	//mProjectile->Friction = 300.f;
	//mProjectile->bBounceAngleAffectsFriction = true;

	mMainHUD->SetBallStateVisible(false);
	mRoot->SetSimulatePhysics(true);
	mIsEnableSwing = false;

	// Spin
	mIsSwingStraight = true;
	mIsSwingRight = false;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();
	mBallInfo.BallPower = mTempBallPower;
	mBallInfo.BallHeight = 0.3f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	// landscape
	if (mHitMaterialName == L"PM_LandscapeBunker")
		mBallInfo.BallHeight *= 2.f;

	//FVector StartPos = GetActorLocation();
	FVector TargetPos;

	////FVector TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower, mBallInfo.BallDir, 0.0);
	//FVector TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower, 0.0, 0.0);

	// dir
	if (abs(mBallInfo.BallSpinDir) <= 90.f)
	{
		//PrintViewport(1.f, FColor::Red, TEXT("Forward"));
		TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower, 0.0, 0.0);
	}

	else if (abs(mBallInfo.BallSpinDir) > 90.f)
	{
		//PrintViewport(1.f, FColor::Red, TEXT("Back"));
		TargetPos = GetActorLocation() - FVector(mBallInfo.BallPower, 0.0, 0.0);
	}

	//if (mBallInfo.BallSpinDir < 0.0)
	//	RotVec.X *= -1.0;

	//mBallInfo.TargetPos = RotVec;

	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("SpinDir: %f"), mBallInfo.BallSpinDir));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("X1: %f"), TargetPos.X));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("X2: %f"), x2));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("Z1: %f"), TargetPos.Z));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("Z2: %f"), z2));

	mBallInfo.BallSpinDir = 0.0;

	// club
	switch (mGolfClubType)
	{
	case EGolfClub::Club1:

		break;
	case EGolfClub::Club2:

		break;
	case EGolfClub::Club3:
		mBallInfo.BallHeight /= 1.5f;
		break;
	}

	FVector outVelocity = FVector::ZeroVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), 0.4);

	if (mHitMaterialName == L"PM_LandscapeBunker")
		outVelocity /= 2.0;
	
	double x = 170000.0;
	double z = 250000.0;

	FVector powerVec = FVector(x, 0.0, z);
	//mRoot->AddImpulse(powerVec);

	//FVector angleVec = FVector(0.5, 0.0, angle);
	//FVector vel = powerVec * angleVec.Normalize();

	// BallMesh->AddImpulse(FVector(Vector.X, Vector.Y, (abs(Vector.X)+abs(Vector.Y))*BallAng).GetSafeNormal()*1000.0f*BallVel);


	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FVector controllerVec;
	if (IsValid(BallController))
	{
		controllerVec = BallController->GetControlRotation().Vector();
	}

	//FVector power = FVector(controllerVec.X, controllerVec.Y, (abs(controllerVec.X) + abs(controllerVec.Y)) * 0.3);
	FVector power = FVector(1.0, 0.0, 0.4);
	power.Normalize();

	mRoot->AddImpulse(power * 450000.0);
	//mRoot->AddImpulse(power * 200000.0);

	//mRoot->AddImpulse(outVelocity);
	mBallInfo.Score++;

	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("normal X: %f"), power.X));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("normal Y: %f"), power.Y));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("normal Z: %f"), power.Z));
}

void ABall::SwingLeft()
{
	mRoot->SetSimulatePhysics(true);

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
	mBallInfo.BallHeight = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallHeight);

	mRoot->AddImpulse(outVelocity);
}

void ABall::SwingRight()
{
	mRoot->SetSimulatePhysics(true);

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
	mBallInfo.BallHeight = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallHeight);

	mRoot->AddImpulse(outVelocity);
}

void ABall::Roll()
{
	//PrintViewport(1.f, FColor::Red, TEXT("Roll"));
	mRoot->SetSimulatePhysics(true);

	// Projectile
	mProjectile->bShouldBounce = false;
	mProjectile->Bounciness = 0.f;
	mProjectile->Friction = 1.5f;

	// Spin
	mIsSwingRight = false;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();
	mBallInfo.BallHeight = 0.9f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(100000, 0, 0);	// test
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallHeight);

	mRoot->AddImpulse(FVector(1000000, 0, 0));
}

void ABall::SetSwingDir(float scale)
{
	if (scale == 0.f)
	{
		return;
	}

	FRotator rot = GetActorRotation();

	if (scale == -1.f/* && mBallInfo.BallDir > -90.0*/)
	{
		mBallInfo.BallSpinDir -= 1.0;
		rot.Pitch -= 1.0;

		//rot.Pitch = mBallInfo.BallSpinDir;
		//SetActorRelativeRotation(rot);
	}

	else if (scale == 1.f/* && mBallInfo.BallDir < 90.0*/)
	{
		mBallInfo.BallSpinDir += 1.0;
		rot.Pitch += 1.0;

		//rot.Pitch = mBallInfo.BallSpinDir;
	}

	SetActorRelativeRotation(rot);
	mSpringArm->SetRelativeRotation(FRotator(0.0, rot.Pitch, 0.0));
	
	FRotator Rot = GetActorRotation();

	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Roll: %f"), Rot.Roll));
	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Pitch: %f"), Rot.Pitch));
	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Yaw: %f"), Rot.Yaw));

	//FRotator springArmRot = FRotator(0.0, rot.Pitch, 0.0);
	//mSpringArm->SetRelativeRotation(springArmRot);
	//SetActorRelativeRotation(springArmRot);

	//FVector vec = FVector(0.0, 0.0, 0.0);
	//FVector StartPos = GetActorLocation();
	//FVector TargetPos = vec + FVector(mBallInfo.BallPower, 0.0, 0.0);

	//double radian = mBallInfo.BallSpinDir * (float)(3.141592 / 180);

	//double x2 = (cos(radian) * TargetPos.X);
	//double y2 = (sin(radian) * TargetPos.X) + (cos(radian) * TargetPos.Y);
	//double z2 = (sin(radian) * TargetPos.X);
	//FVector RotVec = FVector(x2, 0.0, z2);

	//if (mBallInfo.BallSpinDir < 0.0)
	//	RotVec.X *= -1.0;

	//mBallInfo.TargetPos = RotVec;
	//DrawDebugLine(GetWorld(), mBallInfo.StartPos, mBallInfo.TargetPos, FColor::Red, false, 1.0f);
}

void ABall::StopRotate()
{
	mSpringArm->bInheritYaw = false;
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

void ABall::SetBallDetailTurn()
{
	mIsPowerTurn = !mIsPowerTurn;
}

void ABall::AddBallPower(float scale)
{
	if (scale == 0.f || !mIsPowerTurn)
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

void ABall::AddBallHeight(float scale)
{
	if (scale == 0.f || mIsPowerTurn)
		return;

	if (mIsHeightUp)
	{

	}

	else
	{

	}
}

void ABall::ShowDistance()
{
	float leftDis = GetDistanceToTarget(GetActorLocation(), mBallInfo.DestPos);
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
	//FVector endPos = GetActorLocation() - FVector(0.0, 0.0, startPos.Z);
	//FVector endPos = GetActorLocation() - FVector(0.0, 0.0, startPos.Z);
	FVector endPos = startPos + FVector::DownVector * 10000.f;

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
	}
}



void ABall::SetBallDetail()
{
	//PrintViewport(1.f, FColor::Red, mHitMaterialName);
	
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

	else if (mHitMaterialName == L"PM_LandscapeRough")
	{

	}
}

void ABall::ResetBallPos(float DeltaTime)
{
	if (!mIsResetPos)
		return;

	mResetTime += DeltaTime;
	if (mResetTime > 2.f)
	{
		SetActorLocation(mResetPos);
		mResetTime = 0.f;
		mIsResetPos = false;
	}
}

void ABall::FindResetPos()
{
	if (!mIsFindResetPos)
		return;

	if (mResetPos.Y < 0)
		mResetPos.Y += 10.0;
	else
		mResetPos.Y -= 10.0;

	SetActorLocation(mResetPos);

	if (mHitMaterialName == L"PM_LandscapeBase")
	{
		mIsFindResetPos = false;
		mIsResetPos = false;
	}
}

void ABall::ChangeCamera()
{
	// APlayerController::SetViewTargetWithBlend(mSubCamera, 2.f);

	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (IsValid(BallController))
	//{
	//	PrintViewport(1.f, FColor::Red, TEXT("ChangeCamera"));
	//	BallController->SetViewTargetWithBlend(mSubCamera, 2.f);
	//}

	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(BallController))
	{
		FRotator Rot = BallController->GetControlRotation();

		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Roll: %f"), Rot.Roll));
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Pitch: %f"), Rot.Pitch));
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Yaw: %f"), Rot.Yaw));
	}
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//PrintViewport(1.f, FColor::Blue, TEXT("OnHit"));
	//PrintViewport(1.f, FColor::Blue, mHitMaterialName);

	if (mHitMaterialName == L"PM_LandscapeWater" || mHitMaterialName == L"PM_LandscapeRough")
	{
		mRoot->SetSimulatePhysics(false);

		if (IsBallStopped())
		{
			mIsResetPos = true;

			if (mHitMaterialName == L"PM_LandscapeWater")
			{
				mResetPos = FVector(28280.0, 820.0, -230.0);
			}

			else if (mHitMaterialName == L"PM_LandscapeRough")
			{
				FVector curPos = GetActorLocation();
				curPos.Y = 0.0;
				mResetPos = FVector(curPos.X, curPos.Y, curPos.Z);
			}
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{
		mRoot->SetSimulatePhysics(false);
	}

	else if (mHitMaterialName == L"PM_LandscapeGreen")
	{
		if (IsBallStopped())
		{
			// 점수 계산
			UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
			UScoreSubsystem* ScoreSub = GameInst->GetScoreSubsystem();
			FString ScoreText = "";

			if (IsValid(ScoreSub))
				ScoreText = ScoreSub->GetScoreText(mBallInfo.Score);

			mMainHUD->SetScoreText(ScoreText);

			//if (IsValid(ScoreSub))
			//{
			//}
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeLine")
	{
		//// 위치 조정
		//mRoot->SetSimulatePhysics(false);

		//if (IsBallStopped())
		//{
		//	mIsResetPos = true;

		//	if (GetActorLocation().Y < 0)
		//	{

		//	}

		//	else
		//	{

		//	}
		//}

	}

	if (IsBallStopped())
	{
		if (mIsSwingStraight)	// 추가
		{
			//SetActorRelativeRotation(FRotator(0.0, 0.0, 0.0));
			//mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));

		}

		mIsEnableSwing = true;
		mIsSwingStraight = false;
		mIsSwingLeft = false;
		mIsSwingRight = false;

		PrintViewport(1.f, FColor::Red, TEXT("Stop"));

		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetBallStateVisible(true);
	}
}

void ABall::BallBounced(const FHitResult& Hit, const FVector& ImpactVelocity)
{
	//PrintViewport(1.f, FColor::Blue, TEXT("Bounced"));
}

void ABall::BallStopped(const FHitResult& ImpactResult)
{
	//PrintViewport(1.f, FColor::Blue, TEXT("Stopped"));

	if (mHitMaterialName == L"PM_LandscapeWater")
	{
		PrintViewport(1.f, FColor::Blue, TEXT("Water"));
		// 위치 옮김 추가
	}
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

bool ABall::IsBallStopped()
{
	FVector vel = mRoot->GetComponentVelocity();

	if (vel.X == 0.0 && vel.Y == 0.0 && vel.Z == 0.0)
		return true;

	return false;
}

