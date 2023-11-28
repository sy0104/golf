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

	mSpringArm->TargetArmLength = 200.f;
	mCamera->SetRelativeLocation(FVector(-60.0, 0.0, 100.0));
	//mCamera->bConstrainAspectRatio = true;

	mSubCamera = CreateDefaultSubobject<ACameraActor>(TEXT("SubCamera"));
	//mSubCamera->GetCameraComponent()->SetupAttachment(mSpringArm);
	
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
	mRoot->SetLinearDamping(0.0f);
	mRoot->SetUseCCD(true);

	// Ball Info
	mBallInfo.StartPos = FVector(500.0, 0.0, 0.0);
	mBallInfo.TargetPos = FVector(43000.0, 0.0, 0.0);

	mBallInfo.TargetDir = mBallInfo.TargetPos - mBallInfo.StartPos;
	mBallInfo.TargetDir.Normalize();
	mBallInfo.SpinForce = 1000.f;

	mBallInfo.BallPower = 0.0;
	//mBallInfo.BallMinPower = 0.0;
	mBallInfo.BallMaxPower = 190.0;
	mBallInfo.BallDir = 0.0;
	mBallInfo.BallSpinDir = 0.0;

	mBallInfo.SwingArc = 0.3f;
	mBallInfo.Score = -4;

	// spin
	mIsSwingStraight = true;
	mIsSwingLeft = false;
	mIsSwingRight = false;

	// power
	mIsPowerUp = true;
	mTempBallPower = mBallInfo.BallPower;

	mFlyingTime = 0.f;
	mIsSubCamera = false;

	mResetTime = 0.f;
	mIsResetPos = false;

	// Init
	SetActorLocation(mBallInfo.StartPos);
	SetActorScale3D(FVector(1.0, 1.0, 1.0));

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

	//CheckCameraChange(DeltaTime);

	ResetBallPos(DeltaTime);

	//FVector vel = mRoot->GetComponentVelocity();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel X: %f"), vel.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Y: %f"), vel.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Z: %f"), vel.Z));


	//FRotator rot = GetActorRotation();
	//rot.Pitch -= 1.0;
	//SetActorRelativeRotation(rot);
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("pitch: %f"), rot.Pitch));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("dir: %f"), mBallInfo.BallDir));

	//FVector CameraOffset = FVector(-150.0, 0.0, 30.0);
	//FVector loc = GetActorLocation() + CameraOffset;
	//mCamera->SetRelativeLocation(loc);
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

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("SwingDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
}

void ABall::SwingStraight()
{
	//PrintViewport(1.f, FColor::Red, TEXT("SwingStraight"));

	// Projectile
	//mProjectile->bShouldBounce = false;
	//mProjectile->Bounciness = 0.0f;
	//mProjectile->Friction = 300.f;
	//mProjectile->bBounceAngleAffectsFriction = true;

	mRoot->SetSimulatePhysics(true);

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

	// landscape
	if (mHitMaterialName == L"PM_LandscapeBunker")
		mBallInfo.SwingArc *= 2.f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos;



	//FVector TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower, mBallInfo.BallDir, 0.0);
	//FVector TargetPos = GetActorLocation() + FVector(mBallInfo.BallPower, 0.0, 0.0);

	double radian = mBallInfo.BallSpinDir * (float)(3.141592 / 180);
	////double x2 = cos(radian) * TargetPos.X - sin(radian) * TargetPos.Y;
	////double y2 = sin(radian) * TargetPos.X + cos(radian) * TargetPos.Y;

	double x2 = (cos(radian) * TargetPos.X);
	double y2 = (sin(radian) * TargetPos.X) + (cos(radian) * TargetPos.Y);
	double z2 = (sin(radian) * TargetPos.X);
	FVector RotVec = FVector(x2, 0, z2);

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

	if (mBallInfo.BallSpinDir < 0.0)
		RotVec.X *= -1.0;

	mBallInfo.TargetPos = RotVec;

	//PrintViewport(10.f, FColor::Blue, FString::Printf(TEXT("SpinDir: %f"), mBallInfo.BallSpinDir));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("X1: %f"), TargetPos.X));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("X2: %f"), x2));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("Z1: %f"), TargetPos.Z));
	//PrintViewport(10.f, FColor::Red, FString::Printf(TEXT("Z2: %f"), z2));

	mBallInfo.BallSpinDir = 0.0;

	//TargetPos = GetActorLocation() + FVector(600.0, 0.0, 0.0);

	// club
	switch (mGolfClubType)
	{
	case EGolfClub::Club1:

		break;
	case EGolfClub::Club2:

		break;
	case EGolfClub::Club3:
		mBallInfo.SwingArc /= 1.5f;
		break;
	}

	FVector outVelocity = FVector::ZeroVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	if (mHitMaterialName == L"PM_LandscapeBunker")
		outVelocity /= 2.0;

	
	mRoot->AddImpulse(outVelocity);
	mBallInfo.Score++;
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
	mBallInfo.SwingArc = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

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
	mBallInfo.SwingArc = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

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
	mBallInfo.SwingArc = 0.9f;
	mBallInfo.TargetDir = mBallInfo.TargetPos - GetActorLocation();
	mBallInfo.TargetDir.Normalize();

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(500, 0, 0);	// test
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.SwingArc);

	mRoot->AddImpulse(outVelocity / 4);
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

		//rot.Pitch -= 1.0;
		rot.Pitch = mBallInfo.BallSpinDir;
		SetActorRelativeRotation(rot);
	}

	else if (scale == 1.f/* && mBallInfo.BallDir < 90.0*/)
	{
		mBallInfo.BallSpinDir += 1.0;

		//rot.Pitch += 1.0;
		rot.Pitch = mBallInfo.BallSpinDir;
		SetActorRelativeRotation(rot);
	}

	//FRotator springArmRot = FRotator(0.0, rot.Pitch, 0.0);
	//mSpringArm->SetRelativeRotation(springArmRot);
	//SetActorRelativeRotation(springArmRot);
}

void ABall::StopRotate()
{
	mSpringArm->bInheritYaw = false;
	PrintViewport(1.f, FColor::Red, TEXT("Stop Rotate"));
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
		SetActorLocation(mBallInfo.StartPos);
		mResetTime = 0.f;
		mIsResetPos = false;
	}
}

void ABall::ChangeCamera()
{
	// APlayerController::SetViewTargetWithBlend(mSubCamera, 2.f);

	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(BallController))
	{
		PrintViewport(1.f, FColor::Red, TEXT("ChangeCamera"));
		BallController->SetViewTargetWithBlend(mSubCamera, 2.f);
	}
}

void ABall::CheckCameraChange(float DeltaTime)
{
	mFlyingTime += DeltaTime;

	if (mFlyingTime >= 4.f)
	{

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
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{
		mRoot->SetSimulatePhysics(false);
	}

	else if (mHitMaterialName == L"PM_LandscapeLine")
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

	if (IsBallStopped())
	{
		if (mIsSwingStraight)
		{
			SetActorRelativeRotation(FRotator(0.0, 0.0, 0.0));
			mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));

		}

		mIsSwingStraight = false;
		mIsSwingLeft = false;
		mIsSwingRight = false;
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

