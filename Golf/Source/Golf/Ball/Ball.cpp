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
	mRoot->SetSphereRadius(2.15f);

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

	//mSpringArm->TargetArmLength = 2100.0;
	//mCamera->SetRelativeLocation(FVector(0.0, 0.0, 40.0));
	////mCamera->bConstrainAspectRatio = true;
	//	
	//mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 25.0));
	//mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));

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
	mProjectile->bShouldBounce = true;
	mProjectile->Bounciness = 0.1f;
	mProjectile->Friction = 1000.f;
	
	mProjectile->OnProjectileBounce.AddDynamic(this, &ABall::BallBounced);
	mProjectile->OnProjectileStop.AddDynamic(this, &ABall::BallStopped);

	// Collision
	mRoot->SetCollisionProfileName(TEXT("Ball"));
	mRoot->SetSimulatePhysics(false);
	mRoot->SetNotifyRigidBodyCollision(true);
	mRoot->SetUseCCD(true);
	//mRoot->SetLinearDamping(0.1f);
	mRoot->SetAngularDamping(0.5f);
	//mProjectile->ProjectileGravityScale = 20.f;

	// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(4300000.0, 0.0, 0.0);
	mBallInfo.SpinForce = 1000.f;

	mBallInfo.BallPower = 0.0;
	mBallInfo.BallMinPower = 0.0;
	mBallInfo.BallMaxPower = 1500.0;

	//mBallInfo.BallHeight = 0.0;
	//mBallInfo.BallMinHeight = 0.0;
	//mBallInfo.BallMaxHeight = 1500.0;

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

	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;

	mIsBallStopped = true;

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
	
	CheckBallStopped();

	CheckMaterialCollision();

	ResetBallPos(DeltaTime);

	FindResetPos(DeltaTime);

	//if (mIsBallStopped)
	//	PrintViewport(1.f, FColor::Red, TEXT("Stop"));

	//FVector loc = GetActorLocation();
	//loc.Y += 10.0;
	//SetActorLocation(loc);

	//FVector target = mSpringArm->GetForwardVector();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel X: %f"), target.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Y: %f"), target.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Vel Z: %f"), target.Z));

	//if (mIsSwingStraight)
	//{
	//	static float power = 100.0;
	//	FVector resistance = -mRoot->GetPhysicsLinearVelocity().GetSafeNormal();
	//	mRoot->AddForce(resistance * power);
	//	power += 0.1f;
	//}

	//FVector upVec = mRoot->GetUpVector();
	//FVector forwardVec = mRoot->GetForwardVector();

	//FVector vel = mRoot->GetComponentVelocity();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel X: %f"), vel.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Y: %f"), vel.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), vel.Z));

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
	//PlayerInputComponent->BindAction<ABall>(TEXT("ChangeCamera"), EInputEvent::IE_Pressed, this, &ABall::ChangeCamera);
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

	mMainHUD->SetBallStateVisible(false);
	mRoot->SetSimulatePhysics(true);
	mIsEnableSwing = false;

	// Spin
	mIsSwingStraight = true;
	mIsSwingRight = false;
	mIsSwingLeft = false;

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();

	//// landscape
	//if (mHitMaterialName == L"PM_LandscapeBunker")
	//	mBallInfo.BallHeight *= 1.0;

	//FVector StartPos = GetActorLocation();
	FVector TargetPos;

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

	//mBallInfo.BallSpinDir = 0.0;

	// club
	switch (mGolfClubType)
	{
	case EGolfClub::Club1:

		break;
	case EGolfClub::Club2:

		break;
	case EGolfClub::Club3:
		//mBallInfo.BallHeight /= 1.5f;
		break;
	}

	FVector outVelocity = FVector::ZeroVector;
	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), 0.4);

	if (mHitMaterialName == L"PM_LandscapeBunker")
		outVelocity /= 2.0;

	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FVector conVec;
	if (IsValid(BallController))
	{
		conVec = BallController->GetControlRotation().Vector();
	}

	//mBallInfo.BallPower = 250000.0;
	mBallInfo.BallAngle = 1.0;

	FVector forwardVec = mRoot->GetForwardVector();
	FVector power = FVector(1.0, 0.0, mBallInfo.BallAngle);
	power.Normalize();

	mRoot->AddImpulse(power * mBallInfo.BallPower);
	//mProjectile->Velocity = FVector(330000.0, 0.0, 330000.0);

	//mRoot->AddImpulse(outVelocity);
	mBallInfo.Score++;
	mBallInfo.BallPower = 0.0;
	mMainHUD->SetBallPower(0.f);

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
	//mBallInfo.BallHeight = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), 0.3);

	mRoot->AddImpulse(outVelocity);
	mBallInfo.Score++;
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
	//mBallInfo.BallHeight = 0.3f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(mBallInfo.BallPower, 0.0, 0.0);
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallSpinDir);

	mRoot->AddImpulse(outVelocity);
	mBallInfo.Score++;

	//mBallInfo.BallPower = 300000.0;
	//mBallInfo.BallAngle = 1.3;

	//FVector power = FVector(1.0, 0.0, mBallInfo.BallAngle);
	//power.Normalize();

	//mRoot->AddImpulse(power * mBallInfo.BallPower);
	//mBallInfo.Score++;
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
	//mBallInfo.BallHeight = 0.9f;

	FVector StartPos = GetActorLocation();
	FVector TargetPos = StartPos + FVector(100000, 0, 0);	// test
	FVector outVelocity = FVector::ZeroVector;

	UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this, outVelocity, StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallSpinDir);

	mRoot->AddImpulse(FVector(10000, 0, 0));
	mBallInfo.Score++;
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

	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(BallController))
	{
		FVector conVec = BallController->GetControlRotation().Vector();
		FRotator conRot = BallController->GetControlRotation();

		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Roll: %f"), conRot.Roll));
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Pitch: %f"), conRot.Pitch));
		PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Yaw: %f"), conRot.Yaw));
	}
	
	//FRotator Rot = GetActorRotation();

	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Roll: %f"), Rot.Roll));
	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Pitch: %f"), Rot.Pitch));
	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Yaw: %f"), Rot.Yaw));
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
	FVector forwardVec = mBallInfo.DestPos;
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
	if (scale == 0.f)
		return;

	// 5000 ~ 30000

	if (mIsPowerUp)
	{
		mBallInfo.BallPower += 1000.0;

		if (mBallInfo.BallPower >= mBallInfo.BallMaxPower)
			mIsPowerUp = false;
	}

	else
	{
		mBallInfo.BallPower -= 1000.0;

		if (mBallInfo.BallPower <= mBallInfo.BallMinPower)
			mIsPowerUp = true;
	}

	if (IsValid(mMainHUD))
	{
		float ratio = mBallInfo.BallPower / mBallInfo.BallMaxPower;
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
		mMainHUD->SetCourseText(L"Base");
	}

	else if (mHitMaterialName == L"PM_LandscapeFairway")
	{
		mMainHUD->SetCourseText(L"Fairway");

	}

	else if (mHitMaterialName == L"PM_LandscapeGreen")
	{
		mMainHUD->SetCourseText(L"Green");

		if (mIsBallStopped)
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

	else if (mHitMaterialName == L"PM_LandscapeWater")
	{
		mMainHUD->SetCourseText(L"Water");

		mRoot->SetSimulatePhysics(false);

		if (mIsBallStopped)
		{
			mIsResetPos = true;
			mResetPos = FVector(28280.0, 820.0, -230.0);
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{
		mMainHUD->SetCourseText(L"Bunker");

		mRoot->SetSimulatePhysics(false);
	}

	else if (mHitMaterialName == L"PM_LandscapeLine")
	{
		// 위치 조정
		if (mIsBallStopped)
		{
			mRoot->SetSimulatePhysics(false);
			mIsResetPos = true;
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeRough")
	{
		mMainHUD->SetCourseText(L"Rough");

		mRoot->SetSimulatePhysics(false);

		if (mIsBallStopped)
		{
			mIsResetPos = true;
			FVector curPos = GetActorLocation();
			curPos.Y = 0.0;
			mResetPos = FVector(curPos.X, curPos.Y, curPos.Z);
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeTee")
	{
		mMainHUD->SetCourseText(L"Tee");
	}
}

void ABall::ResetBallPos(float DeltaTime)
{
	if (!mIsResetPos)
		return;

	mResetTime += DeltaTime;
	if (mResetTime > 2.f)
	{
		if (mHitMaterialName == L"PM_LandscapeLine")
		{
			mIsFindResetPos = true;
		}

		else
		{
			SetActorLocation(mResetPos);
		}

		mResetTime = 0.f;
		mIsResetPos = false;
	}
}

void ABall::FindResetPos(float DeltaTime)
{
	if (!mIsFindResetPos)
		return;

	mStaticMesh->SetVisibility(false);

	FVector loc = GetActorLocation();

	if (loc.Y < 0)
		loc.Y += 100.0;

	else
		loc.Y -= 100.0;

	SetActorLocation(loc);

	if (mHitMaterialName == L"PM_LandscapeBase")
	{
		if (loc.Y < 0)
			loc.Y += 1000.0;
		else
			loc.Y -= 1000.0;

		SetActorLocation(loc);
		mStaticMesh->SetVisibility(true);

		mIsResetPos = false;
		mIsFindResetPos = false;
	}
}

void ABall::CheckBallStopped()
{
	FVector vel = mRoot->GetComponentVelocity();

	if (vel.X == 0.0 && vel.Y == 0.0 && vel.Z == 0.0)
	{
		mIsBallStopped = true;
		//mRoot->SetSimulatePhysics(false);

		mIsEnableSwing = true;
		mIsSwingStraight = false;
		mIsSwingLeft = false;
		mIsSwingRight = false;

		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetBallStateVisible(true);
	}

	else
	{
		mIsBallStopped = false;
		mMainHUD->SetBallStateVisible(false);
	}
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//PrintViewport(5.f, FColor::Blue, TEXT("OnHit"));
	//PrintViewport(5.f, FColor::Red, mHitMaterialName);

	if (mHitMaterialName == L"PM_LandscapeWater" || mHitMaterialName == L"PM_LandscapeRough")
	{
		mRoot->SetSimulatePhysics(false);

		if (mIsBallStopped)
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
		if (mIsBallStopped)
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
		PrintViewport(1.f, FColor::Red, TEXT("Line"));

		// 위치 조정
		if (mIsBallStopped)
		{
			mRoot->SetSimulatePhysics(false);
			mIsResetPos = true;
		}

	}

	if (mIsBallStopped)
	{
		mRoot->SetSimulatePhysics(false);
		PrintViewport(1.f, FColor::Red, TEXT("Stop"));

		if (mIsSwingStraight)	// 추가
		{
			//SetActorRelativeRotation(FRotator(0.0, 0.0, 0.0));
			//mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));

		}

		mIsEnableSwing = true;
		mIsSwingStraight = false;
		mIsSwingLeft = false;
		mIsSwingRight = false;

		//mRoot->SetLinearDamping(0.5f);

		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetBallStateVisible(true);
	}

	else
	{
		//mRoot->SetLinearDamping(0.0f);
	}
}

void ABall::BallBounced(const FHitResult& Hit, const FVector& ImpactVelocity)
{
	//PrintViewport(1.f, FColor::Blue, TEXT("Bounced"));
}

void ABall::BallStopped(const FHitResult& ImpactResult)
{
	//PrintViewport(1.f, FColor::Blue, TEXT("Stopped"));

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
