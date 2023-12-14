#include "Ball.h"
#include "../GFGameModeBase.h"
#include "../UMG/DistanceBase.h"
#include "../UMG/MainHUDBase.h"
#include "../UMG/PlayInfoBase.h"
#include "../UMG/MiniMap.h"
#include "BallController.h"
#include "../GFGameInstance.h"
#include "../Manager/ScoreSubsystem.h"
#include "NiagaraSystem.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	//// Static Mesh (Root)
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/GolfPack/Meshes/SM_Golfball.SM_Golfball'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
	{
		//PrintViewport(1.f, FColor::Red, TEXT("Mesh Load"));
		mStaticMesh->SetStaticMesh(mesh);
		mStaticMesh->SetRelativeScale3D(FVector(12.0, 12.0, 12.0));
	}

	SetRootComponent(mStaticMesh);
	mStaticMesh->SetSimulatePhysics(true);
	//mStaticMesh->SetAngularDamping(30.f);

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

	// main camera
	mMainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mMainCamera->SetupAttachment(mSpringArm);
	mMainCamera->SetRelativeLocation(FVector(-150.0, 0.0, 70.0));
	mMainCamera->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	mMainCamera->bConstrainAspectRatio = true;
	mMainCamera->SetActive(true);

	// side camera
	mSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	mSideCamera->SetupAttachment(mSpringArm);
	mSideCamera->SetRelativeLocation(FVector(130.0, 0.0, 0.0));
	mSideCamera->SetRelativeRotation(FRotator(0.0, 180.0, 0.0));
	mSideCamera->bConstrainAspectRatio = true;
	mSideCamera->SetActive(false);

	//// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	//mBallInfo.DestPos = FVector(4300000.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(43000.0, 0.0, 0.0);
	mBallInfo.BallDis = 0.f;
	mBallInfo.BallPower = 0.0;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinForce = 10000.f;
	mBallInfo.Score = -4;
	mBallInfo.ShotNum = 1;

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

	// Trailer
	mTrailer = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trailer"));
	mTrailer->SetupAttachment(mStaticMesh);

	const FString& niagaraPath = TEXT("/Game/Materials/N_BallTrail.N_BallTrail");
	UNiagaraSystem* niagara = LoadObject<UNiagaraSystem>(nullptr, *niagaraPath);
	if (IsValid(niagara))
	{
		mTrailer->SetAsset(niagara);
	}

	// Minimap
	mMinimapSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapSpringArm"));
	mMinimapSpringArm->SetupAttachment(mStaticMesh);
	mMinimapSpringArm->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	mMinimapSpringArm->SetRelativeRotation(FRotator(-90.0, 0.0, 0.0));
	mMinimapSpringArm->TargetArmLength = 35000.f;
	mMinimapSpringArm->bUsePawnControlRotation = true;
	mMinimapSpringArm->bInheritPitch = false;
	//mMinimapSpringArm->bInheritYaw = false;
	//mMinimapSpringArm->bInheritRoll = false;

	mMinimapCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCapture"));
	mMinimapCapture->SetupAttachment(mMinimapSpringArm);
	mMinimapCapture->SetRelativeLocation(FVector(0.0, 150.0, 2000.0));
	mMinimapCapture->bCaptureEveryFrame = false;
	mMinimapCapture->bCaptureOnMovement = false;

	const FString& mapRenderPath = TEXT("/Game/UMG/UI_IMAGE/R_MiniMapCapture.R_MiniMapCapture");
	UTextureRenderTarget2D* mapRender = LoadObject<UTextureRenderTarget2D>(nullptr, *mapRenderPath);
	if (IsValid(mapRender))
	{
		mMinimapCapture->TextureTarget = mapRender;
	}

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
			mMainHUD->SetCourseDistanceText(mBallInfo.DestPos.X / 10000.f);
			mMainHUD->SetShotNumText(mBallInfo.ShotNum);
			//mMainHUD->SetMapImage();
			mMinimapCapture->bCaptureEveryFrame = true;
		}
	}

	mStaticMesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Start")
	{
		return;
	}

	if (mIsSwingLeft || mIsSwingRight)
		AddForceToSide();

	ShowDistance();	
	
	CheckBallStopped();

	CheckMaterialCollision();

	ResetBallPos(DeltaTime);

	FindResetPos(DeltaTime);

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

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
}

void ABall::Swing()
{
	if (!mIsEnableSwing)
		return;

	mIsEnableSwing = false;
	mStaticMesh->SetSimulatePhysics(true);
	mMainHUD->SetBallStateVisible(false);

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
	mBallInfo.ShotNum++;

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

	mMainHUD->SetPlayInfoVisible(false);
	mTrailer->Activate();
	mMinimapCapture->bCaptureEveryFrame = false;
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
		mMainHUD->SetTargetDistanceText(leftDis / 100.f);
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
		mMainHUD->SetCourseText(L"Rough");
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

		if (mIsBallStopped)
		{
			mIsResetPos = true;
			mResetPos = FVector(28280.0, 820.0, -230.0);
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{
		mMainHUD->SetCourseText(L"Bunker");
	}

	else if (mHitMaterialName == L"PM_LandscapeLine")
	{
		mMainHUD->SetCourseText(L"Road");

		// 위치 조정
		if (mIsBallStopped)
		{
			mIsResetPos = true;
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeRough")
	{
		mMainHUD->SetCourseText(L"Rough");

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

	//mStaticMesh->SetVisibility(false);

	FVector loc = GetActorLocation();

	if (loc.Y < 0)
		loc.Y += 10.0;

	else
		loc.Y -= 10.0;

	SetActorLocation(loc);

	if (mHitMaterialName == L"PM_LandscapeBase")
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
		//mStaticMesh->SetSimulatePhysics(false);

		mStaticMesh->ComponentVelocity = FVector(0.0, 0.0, 0.0);
		mIsBallStopped = true;
		mIsEnableSwing = true;
		
		if (IsValid(mMainHUD))
		{
			//mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetBallStateVisible(true);
			mMainHUD->SetShotNumText(mBallInfo.ShotNum);
			mMainHUD->SetPlayInfoVisible(true);
			mTrailer->Deactivate();
		}
	}

	else
	{
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel X: %f"), vel.X));
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Y: %f"), vel.Y));
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), vel.Z));

		mIsBallStopped = false;

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetBallStateVisible(false);
			//mMainHUD->SetBallStateVisible(false);
			mMainHUD->SetPlayInfoVisible(false);
			mTrailer->Activate();
		}

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
	//mMainCamera->SetActive(false);
	//mSideCamera->SetActive(true);

	//mSpringArm->CameraLagSpeed = 10.f;

	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//BallController->SetViewTarget(mSubCamera);
	BallController->SetViewTargetWithBlend(mSubCamera, 2.f);

}

void ABall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//PrintViewport(5.f, FColor::Blue, TEXT("OnHit"));
	//PrintViewport(5.f, FColor::Red, mHitMaterialName);

	if (mHitMaterialName == L"PM_LandscapeWater" || mHitMaterialName == L"PM_LandscapeRough")
	{
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
				mResetPos = FVector(curPos.X, 0.0, curPos.Z);
			}
		}
	}

	else if (mHitMaterialName == L"PM_LandscapeBunker")
	{
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
			mIsResetPos = true;
		}

	}
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
