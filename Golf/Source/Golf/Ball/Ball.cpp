#include "Ball.h"
#include "../GFGameModeBase.h"
#include "../UMG/DistanceBase.h"
#include "../UMG/MainHUDBase.h"
#include "BallController.h"
#include "../GFGameInstance.h"
#include "../Manager/ScoreSubsystem.h"
#include "../Camera/FixedCamera.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	//// Static Mesh (Root)
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
	{
		mStaticMesh->SetStaticMesh(mesh);
		mStaticMesh->SetRelativeScale3D(FVector(12.0, 12.0, 12.0));
	}

	SetRootComponent(mStaticMesh);
	mStaticMesh->SetSimulatePhysics(true);
	mStaticMesh->SetAngularDamping(30.f);

	//// Sphere Component (Collision)
	mSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mSphereComponent->SetSphereRadius(2.15f);
	mSphereComponent->SetupAttachment(mStaticMesh);
	mSphereComponent->SetCollisionProfileName(TEXT("Ball"));
	mSphereComponent->SetNotifyRigidBodyCollision(true);	// ?
	mSphereComponent->SetUseCCD(true);

	//// Camera & Spring Arm
	// spring arm
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mSpringArm->SetupAttachment(mStaticMesh);

	mSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	mSpringArm->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	mSpringArm->TargetArmLength = 400.f;
	mSpringArm->bUsePawnControlRotation = true;
	mSpringArm->bEnableCameraLag = true;
	mSpringArm->CameraLagSpeed = 0.f;
	mSpringArm->CameraLagMaxDistance = 10000.f;

	// main camera (moving)
	mMainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mMainCamera->SetupAttachment(mSpringArm);
	mMainCamera->SetRelativeLocation(FVector(-150.0, 0.0, 70.0));
	mMainCamera->SetRelativeRotation(FRotator(-5.0, 0.0, 0.0));
	//mMainCamera->bConstrainAspectRatio = true;
	mMainCamera->SetAutoActivate(false);
	mMainCamera->SetActive(true);

	//// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	//mBallInfo.DestPos = FVector(4300000.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(43000.0, 0.0, 0.0);
	mBallInfo.BallDis = 0.f;
	mBallInfo.BallPower = 0.0;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinForce = 10000.f;
	mBallInfo.Score = -4;

	// spin
	mIsEnableSwing = true;
	mIsSwingLeft = false;
	mIsSwingRight = false;

	// power
	mIsPowerUp = true;

	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;

	mIsBallStopped = true;

	mGolfClubType = EGolfClub::Driver;
	mHitMaterialType = EMaterialType::Tee;

	mWindType = EWindType(FMath::RandRange(0, 3));
	mWindPowerMin = 100.f;
	mWindPowerMax = 300.f;
	mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);
	mIsWindBlow = false;

	mCameraBlendTime = 10.f;

	SetActorLocation(mBallInfo.StartPos);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		mMainHUD = GameMode->GetMainHUD();

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetWindTextVisible(mWindType, true);
		}
	}

	mStaticMesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	// 시작 카메라 설정 (Fixed Camera)
	mFixedCamera = GameMode->GetFixedCamera();
	
	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (IsValid(BallController))
		BallController->SetViewTarget(mFixedCamera);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (mBallSwingType != EBallSwingType::Straight)
		AddForceToSide();

	ShowDistance();	
	
	CheckBallStopped();

	CheckMaterialCollision();

	SetBallDetailsByMaterial();

	ResetBallPos(DeltaTime);

	FindResetPos(DeltaTime);

	CameraMove();


	// Wind();

	//FVector vel = mStaticMesh->GetComponentVelocity();
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel X: %f"), vel.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Y: %f"), vel.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), vel.Z));
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 액션 매핑
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingStraight"), EInputEvent::IE_Pressed, this, &ABall::Swing);
	PlayerInputComponent->BindAction<ABall>(TEXT("ChangeCamera"), EInputEvent::IE_Pressed, this, &ABall::ChangeCamera);
	PlayerInputComponent->BindAction<ABall>(TEXT("Test"), EInputEvent::IE_Pressed, this, &ABall::TestKey);

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
}

void ABall::Swing()
{
	if (!mIsEnableSwing)
		return;

	mIsEnableSwing = false;
	mMainHUD->SetBallStateVisible(false);
	mIsWindBlow = true;

	// Spin
	if (mBallSwingType == EBallSwingType::Left)
		mIsSwingLeft = true;

	else if (mBallSwingType == EBallSwingType::Right)
		mIsSwingRight = true;

	// Club에 따라 볼 정보(dis, arc) 설정
	SetBallInfoByClub(mGolfClubType);

	// Ball Info
	//mBallInfo.BallDis = 20000000.0;		// test, 클럽별로 다르게 설정
	//mBallInfo.BallArc = 0.2f;

	mBallInfo.StartPos = GetActorLocation();

	FVector TargetPos = mBallInfo.StartPos + (mMainCamera->GetForwardVector() * (mBallInfo.BallDis * mBallInfo.BallPower));
	FVector OutVelocity = FVector::ZeroVector;

	if (mGolfClubType == EGolfClub::Putter)
	{
		OutVelocity = mMainCamera->GetForwardVector() * FVector(1.0, 1.0, 1.0) * mBallInfo.BallDis;
	}

	else
	{
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this, OutVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallArc);
	}

	mStaticMesh->AddImpulse(OutVelocity);
	mBallInfo.Score++;

	mBallInfo.BallPower = 0.f;
	mBallInfo.BallDis = 0.0;
	mMainHUD->SetBallPower(0.f);
}

void ABall::SetSwingDir(float scale)
{
	if (scale == 0.f || !mIsEnableSwing)
		return;

	AddControllerYawInput(scale * 0.5);
}

void ABall::AddForceToSide()
{
	FVector forwardVec = mBallInfo.DestPos;
	forwardVec.Normalize();

	FVector leftVec = FVector(GetActorLocation().X, GetActorLocation().Y - 90.0, GetActorLocation().Z);
	leftVec.Normalize();

	FVector CrossVec = FVector::CrossProduct(forwardVec, leftVec);

	if (mIsSwingRight)
		CrossVec.Y *= -1.0;

	mStaticMesh->AddForce(CrossVec * mBallInfo.SpinForce);
}

void ABall::AddBallPower(float scale)
{
	if (scale == 0.f)
		return;

	if (mIsPowerUp)
	{
		mBallInfo.BallPower += 0.01f;

		if (mBallInfo.BallPower >= 1.f)
			mIsPowerUp = false;
	}

	else
	{
		mBallInfo.BallPower -= 0.01f;

		if (mBallInfo.BallPower <= 0.f)
			mIsPowerUp = true;
	}

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetBallPower(mBallInfo.BallPower);
	}
}

void ABall::ShowDistance()
{
	float dis = FVector::Dist(mBallInfo.StartPos, GetActorLocation());
	float leftDis = FVector::Dist(GetActorLocation(), mBallInfo.DestPos);

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetDistanceText(dis / 100.f);
		mMainHUD->SetLeftDistanceText(leftDis / 100.f);
	}
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

		if (IsValid(material))
		{
			FString name = material->GetName();
			SetBallHitMaterial(name);
		}
	}
}

void ABall::SetBallHitMaterial(FString MaterialName)
{
	//PrintViewport(1.f, FColor::Red, mHitMaterialName);

	if (MaterialName == L"PM_LandscapeBase")
	{
		mHitMaterialType = EMaterialType::Rough;
		mMainHUD->SetCourseText(L"Rough");
	}

	else if (MaterialName == L"PM_LandscapeFairway")
	{
		mHitMaterialType = EMaterialType::Fairway;
		mMainHUD->SetCourseText(L"Fairway");
	}

	else if (MaterialName == L"PM_LandscapeGreen")
	{
		mHitMaterialType = EMaterialType::Green;
		mMainHUD->SetCourseText(L"Green");
	}

	else if (MaterialName == L"PM_LandscapeWater")
	{
		mHitMaterialType = EMaterialType::Water;
		mMainHUD->SetCourseText(L"Water");
	}

	else if (MaterialName == L"PM_LandscapeBunker")
	{
		mHitMaterialType = EMaterialType::Bunker;
		mMainHUD->SetCourseText(L"Bunker");
	}

	else if (MaterialName == L"PM_LandscapeLine")
	{
		mHitMaterialType = EMaterialType::Road;
		mMainHUD->SetCourseText(L"Road");
	}

	else if (MaterialName == L"PM_LandscapeRough")
	{
		mHitMaterialType = EMaterialType::OB;
		mMainHUD->SetCourseText(L"OB");
	}

	else if (MaterialName == L"PM_LandscapeTee")
	{
		mHitMaterialType = EMaterialType::Tee;
		mMainHUD->SetCourseText(L"Tee");
	}
}

void ABall::SetBallDetailsByMaterial()
{
	if (!mIsBallStopped)
		return;

	switch (mHitMaterialType)
	{
	case EMaterialType::Tee:

		break;
	case EMaterialType::Fairway:
		
		break;
	case EMaterialType::Green:
		CalculateScore();
		break;
	case EMaterialType::Rough:
	{
		FVector curPos = GetActorLocation();
		mResetPos = FVector(curPos.X, 0.0, curPos.Z);
	}
		break;
	case EMaterialType::Water:
		mIsResetPos = true;
		mResetPos = FVector(28280.0, 820.0, -230.0);
		break;
	case EMaterialType::Bunker:

		break;
	case EMaterialType::Road:
		mIsResetPos = true;
		break;
	case EMaterialType::OB:

		break;
	}
}

void ABall::ResetBallPos(float DeltaTime)
{
	if (!mIsResetPos)
		return;

	mResetTime += DeltaTime;

	if (mResetTime > 2.f)
	{
		if (mHitMaterialType == EMaterialType::Road)
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

	//mStaticMesh->SetVisibility(false);

	FVector loc = GetActorLocation();

	if (loc.Y < 0)
		loc.Y += 10.0;

	else
		loc.Y -= 10.0;

	SetActorLocation(loc);

	if (mHitMaterialType == EMaterialType::Rough)	// Base
	{
		if (loc.Y < 0)
			loc.Y += 1000.0;
		else
			loc.Y -= 1000.0;

		SetActorLocation(loc);
		//mStaticMesh->SetVisibility(true);

		mIsResetPos = false;
		mIsFindResetPos = false;
	}
}

void ABall::CheckBallStopped()
{
	double vel = mStaticMesh->GetComponentVelocity().Size();

	if (vel < 1.0)
	{
		mStaticMesh->ComponentVelocity = FVector(0.0, 0.0, 0.0);
		mIsBallStopped = true;
		mIsEnableSwing = true;

		//if (mIsWindBlow)
		//{
		//	mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);
		//	if (IsValid(mMainHUD))
		//	{
		//		mMainHUD->SetWindTextVisible(mWindType, false);
		//		mWindType = (EWindType)FMath::RandRange(0, 3);
		//		mMainHUD->SetWindTextVisible(mWindType, true);
		//	}

		//	mIsWindBlow = false;
		//}

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetBallStateVisible(true);
		}
	}

	else
	{
		mIsBallStopped = false;
		mIsEnableSwing = false;

		if (IsValid(mMainHUD))
			mMainHUD->SetBallStateVisible(false);
	}
}

void ABall::SetBallInfoByClub(EGolfClub club)
{
	switch (club)
	{
	case EGolfClub::Driver:
		mBallInfo.BallDis = 80000000.f;
		mBallInfo.BallArc = 0.4f;
		break;
	case EGolfClub::Wood:
		mBallInfo.BallDis = 60000000.f;
		mBallInfo.BallArc = 0.5f;
		break;
	case EGolfClub::Iron:
		mBallInfo.BallDis = 55000000.f;
		mBallInfo.BallArc = 0.5f;
		break;
	case EGolfClub::Wedge:
		mBallInfo.BallDis = 20000000.f;
		mBallInfo.BallArc = 0.2f;
		break;
	case EGolfClub::Putter:
		mBallInfo.BallDis = 60000.f;
		mBallInfo.BallArc = 1.f;
		break;
	}
}

void ABall::ChangeCamera()
{
	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	BallController->SetViewTargetWithBlend(mMainCamera->GetOwner(), mCameraBlendTime);
}

void ABall::CameraMove()
{
	mCameraBlendTime -= 0.01f;
}

void ABall::TestKey()
{
	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	BallController->SetViewTargetWithBlend(mFixedCamera, mCameraBlendTime);
}

void ABall::CalculateScore()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UScoreSubsystem* ScoreSub = GameInst->GetScoreSubsystem();
	FString ScoreText = "";

	if (IsValid(ScoreSub))
		ScoreText = ScoreSub->GetScoreText(mBallInfo.Score);

	if (IsValid(mMainHUD))
		mMainHUD->SetScoreText(ScoreText);
}

void ABall::Wind()
{
	if (!mIsWindBlow)
		return;

	PrintViewport(1.f, FColor::Red, TEXT("Wind"));

	FVector dirVec;

	switch (mWindType)
	{
	case EWindType::Left:
		dirVec = -mMainCamera->GetRightVector();
		break;
	case EWindType::Right:
		dirVec = mMainCamera->GetRightVector();
		break;
	case EWindType::Forward:
		dirVec = mMainCamera->GetForwardVector();
		break;
	case EWindType::Back:
		dirVec = -mMainCamera->GetForwardVector();
		break;
	}

	mStaticMesh->AddForce(dirVec * mWindPower);
}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	PrintViewport(5.f, FColor::Blue, TEXT("OnHit"));
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
