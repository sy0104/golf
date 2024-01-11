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
#include "../Manager/GameManager.h"
#include "../UMG/BallInfoBase.h"

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
		mStaticMesh->SetRelativeScale3D(FVector(1.1, 1.1, 1.1));
	}

	SetRootComponent(mStaticMesh);
	mStaticMesh->SetSimulatePhysics(true);
	mStaticMesh->SetAngularDamping(40.f);
	mStaticMesh->SetUseCCD(true);
	mStaticMesh->SetCollisionProfileName(TEXT("Ball"));
	
	//// Sphere Component (Collision)
	mSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mSphereComponent->SetSphereRadius(2.25f);
	mSphereComponent->SetupAttachment(mStaticMesh);
	mSphereComponent->SetCollisionProfileName(TEXT("Ball"));
	mSphereComponent->SetNotifyRigidBodyCollision(true);
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
	mCameraLagSpeed = 1.2f;
	mSpringArm->CameraLagSpeed = mCameraLagSpeed;

	// main camera
	mMainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mMainCamera->SetupAttachment(mSpringArm);
	mMainCamera->SetRelativeLocation(FVector(220.0, 0.0, 60.0));
	mMainCamera->SetRelativeRotation(FRotator(-5.0, 0.0, 0.0));
	mMainCamera->bConstrainAspectRatio = true;
	mMainCamera->SetAutoActivate(true);
	mMainCamera->SetActive(true);

	// side spring arm
	mSideSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SideSpringArm"));
	mSideSpringArm->SetupAttachment(mStaticMesh);
	mSideSpringArm->SetRelativeLocation(FVector(150.0, 30.0, 0.0));
	mSideSpringArm->SetRelativeRotation(FRotator(20.0, -170.0, 0.0));
	mSideSpringArm->TargetArmLength = 500.f;
	mSideSpringArm->bUsePawnControlRotation = false;
	mSideSpringArm->bEnableCameraLag = true;
	mSideSpringArm->CameraLagSpeed = 0.f;

	// side camera
	mSideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SideCamera"));
	mSideCamera->SetupAttachment(mSideSpringArm);
	mSideCamera->SetRelativeLocation(FVector(220.0, 0.0, 60.0));
	mSideCamera->SetRelativeRotation(FRotator(-5.0, 0.0, 0.0));
	mSideCamera->bConstrainAspectRatio = true;
	mSideCamera->SetAutoActivate(false);

	//// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(37303.0, -998.0, 0.0);
	mBallInfo.BallPower = 0.f;
	mBallInfo.BallRatio = 0.0;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinPower = 300.f;

	// spin
	mIsEnableSwing = true;

	// detail
	mIsAddPower = true;
	mIsAddSpin = false;
	mIsPowerUp = true;
	mIsSpinUp = true;

	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;

	mIsBallStopped = true;

	mIsConcede = false;
	mIsInHole = false;
	mIsSetScore = false;

	mIsMultiEnd = false;
	mTurn = 0;
	mIsStart = false;
	mIsEnd = false;
	
	mMovingDis = 0.f;
	mIsGoodShot = false;
	mIsOnGreen = false;

	mIsRetry = false;

	mGolfClubType = EGolfClub::Driver; 
	mHitMaterialType = EMaterialType::Tee;

	// wind
	mWindType = EWindType(FMath::RandRange(0, 3)); 
	mWindPowerMin = 10.f;
	mWindPowerMax = 60.f;
	mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);
	mIsWindBlow = true;

	// Trailer
	mTrailer = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trailer"));
	mTrailer->SetupAttachment(mStaticMesh);

	const FString& niagaraPath = TEXT("/Game/Materials/N_BallTrail.N_BallTrail");
	UNiagaraSystem* niagara = LoadObject<UNiagaraSystem>(nullptr, *niagaraPath);
	if (IsValid(niagara))
	{
		mTrailer->SetAsset(niagara);
	}
	mTrailer->bAutoActivate = false;
	mTrailer->Deactivate();

	SetActorLocation(mBallInfo.StartPos);
}

void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		mMainHUD = GameMode->GetMainHUD();

		if (IsValid(mMainHUD) && UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Main")
		{
			// Distance
			mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetDistanceVisible(false);

			// Minimap
			mMainHUD->SetMiniMapHoleImage(mBallInfo.DestPos);

			// Wind
			mMainHUD->SetWindTextVisible(mWindType, true);
			mMainHUD->SetWindVelText(mWindPower / 10.f);
		}
	}

	mStaticMesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	// Play Type �޾ƿ���
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		mPlayType = GameManager->GetPlayType();

		if (mPlayType == EPlayType::Multi)
		{
			FPlayerInfo PlayerInfo = GameManager->GetPlayerInfo(EPlayer::Player2);
			PlayerInfo.BallPos = GetActorLocation();
			GameManager->SetPlayerInfo(PlayerInfo, (int)(EPlayer::Player2));

			if (IsValid(mMainHUD))
				mMainHUD->SetPlayerTargetDistanceText(PlayerInfo.LeftDistance / 100.f, false);
		}
	}

	// Camera

}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == L"Start")
	{
		return;
	}

	if (mBallSwingType != EBallSwingType::Straight)
		AddForceToSide();

	ShowDistance();	

	ChangeCamera(DeltaTime);

	CheckMaterialCollision();
	
	CheckBallStopped();

	CheckChangeTurn(DeltaTime);

	SetBallDetailsByMaterial();

	ResetBallPos(DeltaTime);

	FindResetPos(DeltaTime);

	Wind();

	CheckPlayerGoal();

	//FVector loc = GetActorLocation();
	//loc.Z += 10.f;
	//SetActorLocation(loc);

	// test
	//mTrailer->Deactivate();

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), vel.Z));

}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �׼� ����
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingStraight"), EInputEvent::IE_Pressed, this, &ABall::Swing);
	PlayerInputComponent->BindAction<ABall>(TEXT("Cheat"), EInputEvent::IE_Pressed, this, &ABall::Cheat);
	PlayerInputComponent->BindAction<ABall>(TEXT("Test"), EInputEvent::IE_Pressed, this, &ABall::TestKey);
	PlayerInputComponent->BindAction<ABall>(TEXT("CloseTotalScore"), EInputEvent::IE_Released, this, &ABall::CloseTotalScore);
	PlayerInputComponent->BindAction<ABall>(TEXT("Menu"), EInputEvent::IE_Pressed, this, &ABall::ShowMenu);

	// �� ����
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDetail"), this, &ABall::SetBallDetail);
	PlayerInputComponent->BindAxis<ABall>(TEXT("ShowTotalScore"), this, &ABall::ShowTotalScore);
}

void ABall::Swing()
{
	if (!mIsEnableSwing)
		return;

	if (!mIsStart)
		mIsStart = true;

	mIsEnableSwing = false;
	mTrailer->Activate();
	mSpringArm->CameraLagSpeed = mCameraLagSpeed;

	// Club�� ���� �� ����(dis, arc) ����
	SetBallInfoByClub(mGolfClubType);

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();

	// Swing
	FVector TargetPos = mBallInfo.StartPos + (mMainCamera->GetForwardVector() * (mBallInfo.BallPower * mBallInfo.BallRatio));
	FVector OutVelocity = FVector::ZeroVector;

	if (mGolfClubType == EGolfClub::Putter)
	{
		OutVelocity = mMainCamera->GetForwardVector() * (mBallInfo.BallPower * mBallInfo.BallRatio);
		mStaticMesh->AddImpulse(OutVelocity, NAME_None, true);
	}

	else
	{
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this, OutVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallArc);

		// Bunker
		if (mHitMaterialType == EMaterialType::Bunker)
		{
			OutVelocity *= 0.6;
			mBallInfo.BallArc *= 0.5;
		}

		mStaticMesh->AddImpulse(OutVelocity);
	}

	// Ball Info �ʱ�ȭ
	mBallInfo.BallRatio = 0.f;
	mBallInfo.BallPower = 0.f;

	// Player UI ������Ʈ (Shot)
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		EPlayer CurPlayer = GameManager->GetCurPlayer();
		FPlayerInfo CurPlayerInfo = GameManager->GetCurPlayerInfo();
		CurPlayerInfo.Shot++;
		GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetPlayerShotNumText(CurPlayerInfo.Shot, true);
		}
	}

	// UI Update
	if (IsValid(mMainHUD))
	{
		mMainHUD->SetDistanceVisible(true);
		mMainHUD->SetBallInfoVisible(false);
		mMainHUD->SetBallPower(0.f);
		mMainHUD->SetBallSpin(0.f);
		mMainHUD->SetPowerCheckBoxChecked(true);
		mMainHUD->SetSpinCheckBoxChecked(false);
		mMainHUD->SetMiniMapVisible(false);
		mMainHUD->SetPuttingInfoVisible(false);
		mMainHUD->SetIronButtonVisible(false);
	}

	mTurn++;
}

void ABall::SetSwingDir(float scale)
{
	if (scale == 0.f || !mIsEnableSwing)
		return;

	AddControllerYawInput(scale * 0.3);
}

void ABall::AddForceToSide()
{
	double vel = mStaticMesh->GetComponentVelocity().Size();

	if (!mIsEnableSwing && vel > 10.f)
	{
		FVector forwardVec = mBallInfo.DestPos;
		forwardVec.Normalize();

		FVector leftVec = FVector(GetActorLocation().X, GetActorLocation().Y - 90.0, GetActorLocation().Z);
		leftVec.Normalize();

		FVector CrossVec = FVector::CrossProduct(forwardVec, leftVec);

		if (mBallSwingType == EBallSwingType::Right)
			CrossVec.Y *= -1.0;

		mStaticMesh->AddForce(CrossVec * mBallInfo.SpinPower * mBallInfo.SpinRatio);
	}
}

void ABall::SetBallDetail(float scale)
{
	if (scale == 0.f || !mIsEnableSwing)
		return;

	if (mIsAddPower)
		AddBallPower(scale);

	else if (mIsAddSpin)
		AddBallSpin(scale);
}

void ABall::AddBallPower(float scale)
{
	if (mIsPowerUp)
	{
		mBallInfo.BallRatio += 0.01f;

		if (mBallInfo.BallRatio >= 1.f)
			mIsPowerUp = false;
	}

	else
	{
		mBallInfo.BallRatio -= 0.01f;

		if (mBallInfo.BallRatio <= 0.f)
			mIsPowerUp = true;
	}

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetBallPower(mBallInfo.BallRatio);
	}
}

void ABall::AddBallSpin(float scale)
{
	if (mIsSpinUp)
	{
		mBallInfo.SpinRatio += 0.01f;

		if (mBallInfo.SpinRatio >= 1.f)
			mIsSpinUp = false;
	}

	else
	{
		mBallInfo.SpinRatio -= 0.01f;

		if (mBallInfo.SpinRatio <= 0.f)
			mIsSpinUp = true;
	}

	if (IsValid(mMainHUD))
		mMainHUD->SetBallSpin(mBallInfo.SpinRatio);
}

void ABall::ShowDistance()
{
	mMovingDis = FVector::Dist(mBallInfo.StartPos, GetActorLocation());
	float leftDis = 0.f;

	if (mIsInHole)
		leftDis = 0.f;

	else
		leftDis = FVector::Dist(GetActorLocation(), mBallInfo.DestPos);

	double targetDis = 0.0;
	switch (mGolfClubType)
	{
	case EGolfClub::Driver:
		targetDis = 25000;
		break;
	case EGolfClub::Wood:
		targetDis = 22000;
		break;
	case EGolfClub::Iron:
		switch (mIronType)
		{
		case EIronType::Iron5:
			targetDis = 16000;
			break;
		case EIronType::Iron6:
			targetDis = 15000;
			break;
		case EIronType::Iron7:
			targetDis = 14000;
			break;
		case EIronType::Iron8:
			targetDis = 13000;
			break;
		case EIronType::Iron9:
			targetDis = 12000;
			break;
		default:
			targetDis = 16000;
			break;
		}
		break;
	case EGolfClub::Wedge:
		targetDis = 10000;
		break;
	case EGolfClub::Putter:
		targetDis = 600;
		break;
	}

	FVector targetPos = GetActorLocation() + (targetDis * mMainCamera->GetForwardVector());
	targetPos.Z = 0.0;
	float holeDis = FVector::Dist(targetPos, mBallInfo.DestPos);

	if (IsValid(mMainHUD))
	{
		// Distance UI
		mMainHUD->SetDistanceText(mMovingDis / 100.f);
		mMainHUD->SetLeftDistanceText(leftDis / 100.f);

		// Player Info UI
		mMainHUD->SetPlayerTargetDistanceText(leftDis / 100.f, true);

		// MiniMap UI
		mMainHUD->SetMiniMapBallCurrent(GetActorLocation());
		mMainHUD->SetMiniMapBallTarget(GetActorLocation(), mMainCamera->GetForwardVector(), targetDis);
		mMainHUD->SetMiniMapTargetDistanceText(targetDis / 100.f);
		mMainHUD->SetMiniMapHoleDistanceText(holeDis / 100.f);
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

	else /*if (MaterialName == L"PM_LandscapeTee")*/
	{
		mHitMaterialType = EMaterialType::Tee;
		mMainHUD->SetCourseText(L"Tee");
	}
}

void ABall::SetBallDetailsByMaterial()
{
	if (!mIsBallStopped/* || mIsEnableSwing*/)
		return;

	switch (mHitMaterialType)
	{
	case EMaterialType::Tee:

		break;
	case EMaterialType::Fairway:
		
		break;
	case EMaterialType::Green:
		break;
	case EMaterialType::Water:
	{
		mIsResetPos = true;
		FVector curPos = GetActorLocation();
		mResetPos = FVector(curPos.X, 0.0, curPos.Z + 200);
		mIsRetry = true;
	}
		break;
	case EMaterialType::Bunker:

		break;
	case EMaterialType::Road:
		//mIsResetPos = true;
		break;
	case EMaterialType::OB:
	{
		mIsResetPos = true;
		mResetPos = mBallInfo.StartPos;
		mIsRetry = true;
	}
		break;
	}
}

void ABall::ResetBallPos(float DeltaTime)
{
	if (!mIsResetPos)
		return;

	mResetTime += DeltaTime;

	if (mResetTime > 3.f)
	{
		if (mHitMaterialType == EMaterialType::Road)
		{
			mIsFindResetPos = true;
		}

		else
		{
			mSpringArm->CameraLagSpeed = 0.f;
			SetActorLocation(mResetPos);

			ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mBallInfo.DestPos);
			Rotator.Pitch = 0.f;
			BallController->SetControlRotation(Rotator);

			// �� �߰�
			UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
			UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

			if (IsValid(GameManager))
			{
				EPlayer CurPlayer = GameManager->GetCurPlayer();
				FPlayerInfo CurPlayerInfo = GameManager->GetCurPlayerInfo();
				CurPlayerInfo.Shot++;
				GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);

				if (IsValid(mMainHUD))
					mMainHUD->SetPlayerShotNumText(CurPlayerInfo.Shot);
			}
		}

		mResetTime = 0.f;
		mIsResetPos = false;
		mIsRetry = false;
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
		mIsBallStopped = true;
		mStaticMesh->ComponentVelocity = FVector(0.0, 0.0, 0.0);

		mTrailer->Deactivate();

		if (IsValid(mMainHUD))
		{
			if (mIsEnableSwing && !mIsEnd)
			{
				mMainHUD->SetDistanceText(0.f);
				mMainHUD->SetDistanceVisible(false);
				mMainHUD->SetBallStateVisible(true);
				mMainHUD->SetMiniMapVisible(true);
				mMainHUD->SetBallDistance(mGolfClubType, mIronType);
				mMainHUD->SetHoleMark(GetActorLocation(), mBallInfo.DestPos);

				if (mHitMaterialType == EMaterialType::Green)
					SetPuttingMode(true);
				else
					SetPuttingMode(false);	
			}

			else
			{
				// Good Shot
				CheckGoodShot();
				if (mIsGoodShot && !mIsConcede)
					mMainHUD->SetGoodShotVisible(true);

				if (mHitMaterialType == EMaterialType::Green)
					mIsWindBlow = false;
			}
		}
	}

	else
	{
		mIsBallStopped = false;

		if (mIsStart)
			mTrailer->Activate();

		if (mIsEnableSwing)
			mTrailer->Deactivate();

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetBallStateVisible(false);
			mMainHUD->SetMiniMapVisible(false);
		}
	}
}

void ABall::SetBallInfoByClub(EGolfClub club)
{
	switch (club)
	{
	case EGolfClub::Driver:	// �ִ� 300m ����
		mBallInfo.BallPower = 2300.f;
		mBallInfo.BallArc = 0.6f;
		break;
	case EGolfClub::Wood:	// �ִ� 215m ����
		mBallInfo.BallPower = 1600.f;
		mBallInfo.BallArc = 0.7f;
		break;
	case EGolfClub::Iron:	// �ִ� 170m ����
		mBallInfo.BallArc = 0.7f;

		switch (mIronType)
		{
		case EIronType::Iron5:	// 160m
			mBallInfo.BallPower = 1400.f;
			break;
		case EIronType::Iron6:	// 150m
			mBallInfo.BallPower = 1000.f;
			break;
		case EIronType::Iron7:	// 140m
			mBallInfo.BallPower = 825.f;
			break;
		case EIronType::Iron8:	// 130m
			mBallInfo.BallPower = 760.f;
			break;
		case EIronType::Iron9:	// 120m
			mBallInfo.BallPower = 700.f;
			break;
		}
		break;
	case EGolfClub::Wedge:	// �ִ� 70m ����
		mBallInfo.BallPower = 550.f;
		mBallInfo.BallArc = 0.5f;
		break;
	case EGolfClub::Putter:	// �ִ� 20m ����
		mBallInfo.BallPower = 1000.f;
		mBallInfo.BallArc = 1.f;
		break;
	}
}

void ABall::SetPlayerInfoUI(EPlayer player, bool isDetail)
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	FPlayerInfo PlayerInfo;

	if (IsValid(GameManager))
		PlayerInfo = GameManager->GetPlayerInfo(player);

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetPlayerImage(PlayerInfo.ImagePath, isDetail);
		mMainHUD->SetPlayerNameText(PlayerInfo.Name, isDetail);
		mMainHUD->SetPlayerShotNumText(PlayerInfo.Shot, isDetail);
		//mMainHUD->SetPlayerScoreText(PlayerInfo.Score, isDetail);
	}
}

void ABall::CheckChangeTurn(float DeltaTime)
{
	//if (mPlayType == EPlayType::Single)
	//	return;

	if (mIsBallStopped && !mIsEnableSwing)
		mChangeTurnTime += DeltaTime;

	else if (!mIsBallStopped)
		mChangeTurnTime = 0.f;

	if (!mIsChangeTurn && mChangeTurnTime >= 3.f)	// 5
	{
		mIsChangeTurn = true;
		ChangeTurn();
	}
}

void ABall::ChangeTurn()
{
	if (!mIsChangeTurn)
		return;

	mTrailer->Deactivate();

	// Players ���� ������Ʈ
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (!IsValid(GameManager))
		return;

	// ���� ������(��� ������) �÷��̾� ������
	EPlayer CurPlayer, NextPlayer;
	FPlayerInfo CurPlayerInfo, NextPlayerInfo;

	CurPlayer = GameManager->GetCurPlayer();
	CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);
	
	float leftDis = FVector::Dist(GetActorLocation(), mBallInfo.DestPos);
	CurPlayerInfo.LeftDistance = leftDis;

	if (!mIsRetry)
	{

		// [ Multi ] ���� ������ �÷��̾� ������(��Ƽ �÷��� && Ȧ�� ���� �ƹ��� ���� ������ ���)
		if (mPlayType == EPlayType::Multi)
		{
			NextPlayer = GameManager->GetNextPlayer();
			NextPlayerInfo = GameManager->GetPlayerInfo(NextPlayer);

			// ���� �÷��̾��� ���� ����
			CurPlayerInfo.BallPos = GetActorLocation();
			GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);

			// 2�� �� ���� Ȧ�� ���� ������ ���: �÷��̾� ������ �ٲ۴�
			if (!CurPlayerInfo.TurnEnd && !NextPlayerInfo.TurnEnd)
			{
				// ���� �Ÿ��� �� �� ������� ģ�� (�� �÷��̾� ��� �ѹ��� ģ ���ĺ���)
				float dis1 = GameManager->GetPlayerLeftDis(EPlayer::Player1);
				float dis2 = GameManager->GetPlayerLeftDis(EPlayer::Player2);

				if (dis1 >= dis2)	// player1 ����
				{
					GameManager->SetCurPlayer((int)EPlayer::Player1);
					mMainHUD->SetPlayerTargetDistanceText(dis2 / 100.f, false);
				}

				else	// player2 ����
				{
					GameManager->SetCurPlayer((int)EPlayer::Player2);
					mMainHUD->SetPlayerTargetDistanceText(dis1 / 100.f, false);
				}

				mSpringArm->CameraLagSpeed = 0.f;
				SetActorLocation(GameManager->GetCurPlayerInfo().BallPos);
			}

			// 2�� �� 1���� ���� Ȧ�� ���� ���� ���: �÷��̾� ���� �ٲ��� ����
			else if (CurPlayerInfo.TurnEnd)
			{
				CurPlayerInfo.LeftDistance = 0.f;
				GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);

				mSpringArm->CameraLagSpeed = 0.f;
				SetActorLocation(NextPlayerInfo.BallPos);

				GameManager->SetCurPlayer((int)NextPlayer);
				mPlayType = EPlayType::Single;
			}

			// Multi�� ��쿡�� ���� �÷��̾��� UI ����(���� �÷��̾�: Simple UI, ���� �÷��̾�: Detail UI)
			SetPlayerInfoUI(GameManager->GetNextPlayer(), false);
		}

		// [ Single ] ���� �÷��̾� ���� ����
		else
		{
			// ��Ƽ���� 2�� �� Ȧ�� �־��� ���: ���� ����
			if (GameManager->GetPlayType() == EPlayType::Multi)
			{
				NextPlayer = GameManager->GetNextPlayer();
				NextPlayerInfo = GameManager->GetPlayerInfo(NextPlayer);

				if (CurPlayerInfo.TurnEnd && NextPlayerInfo.TurnEnd)
				{
					mIsEnd = true;
					NextGame();
				}
			}

			// �̱�
			else
			{
				CurPlayerInfo.BallPos = GetActorLocation();
				GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);

				// ���� ���� Ȯ��
				if (CurPlayerInfo.TurnEnd)
				{
					mIsEnd = true;
					NextGame();
				}
			}
		}
	}

	// Player Info UI ����
	SetPlayerInfoUI(GameManager->GetCurPlayer(), true);

	// �� �Ѿ �� ������ �ʱ�ȭ
	mIsEnableSwing = true;
	mIsChangeTurn = false;
	mChangeTurnTime = 0.f;
	mMovingDis = 0.f;
	mIsGoodShot = false;
	mIsAddPower = true;
	mIsAddSpin = false;
	mBallInfo.SpinRatio = 0.f;
	mIsChangeCamera = false;
	mMovingTime = 0.f;

	// Hole ���� �ٶ󺸵���
	if (!mIsResetPos)
	{
		ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mBallInfo.DestPos);
		Rotator.Pitch = 0.f;
		BallController->SetControlRotation(Rotator);
	}

	// �ٶ�
	UpdateWind();

	// UI ������Ʈ
	if (IsValid(mMainHUD) && !mIsEnd)
	{
		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetDistanceVisible(false);
		mMainHUD->SetBallInfoVisible(true);
		mMainHUD->SetScoreTextVisible(false);
		mMainHUD->SetConcedeTextVisible(false);
		mMainHUD->SetGoodShotVisible(false);
		mMainHUD->SetSpinButtonsEnable();
	}

	SetActorRotation(FRotator(0.0, 0.0, 0.0));
}

void ABall::ShowTotalScore(float scale)
{
	if (scale == 0.f)
		return;

	if (IsValid(mMainHUD))
	{
		mMainHUD->SetTotalScoreVisible(true);
	}
}

void ABall::CloseTotalScore()
{
	if (IsValid(mMainHUD))
	{
		mMainHUD->SetTotalScoreVisible(false);
	}
}

void ABall::ShowMenu()
{
	if (IsValid(mMainHUD))
	{
		bool isVisible = mMainHUD->GetIsShowMenu();
		mMainHUD->ShowMenu(!isVisible);
	}
}

void ABall::Init(bool isEnd)
{
	// �� ��ġ �ʱ�ȭ
	mSpringArm->CameraLagSpeed = 0.f;
	SetActorLocation(FVector(0.0, 0.0, 13.5));

	// �÷��̾� ���� �ʱ�ȭ
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	
	if (!IsValid(GameManager))
	{
		PrintViewport(5.f, FColor::Red, TEXT("Init() GameManager Error"));
		return;
	}

	mPlayType = GameManager->GetPlayType();
	GameManager->SetCurPlayer((int)EPlayer::Player1);
	GameManager->Init();

	// Ball Info �ʱ�ȭ
	mBallInfo.StartPos = FVector(0.0, 0.0, 13.5);
	mBallInfo.BallPower = 0.f;
	mBallInfo.BallRatio = 0.f;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinPower = 200.f;

	// ������� �ʱ�ȭ
	mGolfClubType = EGolfClub::Driver;
	mHitMaterialType = EMaterialType::Tee;
	mIsEnableSwing = true;
	mIsPowerUp = true;
	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;
	mIsBallStopped = true;
	mIsConcede = false;
	mIsInHole = false;
	mTurn = 0;
	mIsStart = false;
	mIsEnd = false;
	mIsSetScore = false;

	// Ȧ ���� �ٶ󺸵��� ����
	ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mBallInfo.DestPos);
	Rotator.Pitch = 0.f;
	BallController->SetControlRotation(Rotator);

	if (!isEnd)
	{
		// Player Info UI �ʱ�ȭ
		SetPlayerInfoUI(EPlayer::Player1, true);
		if (GameManager->GetPlayType() == EPlayType::Multi)
			SetPlayerInfoUI(EPlayer::Player2, false);

		// Course Text UI �ʱ�ȭ
		if (IsValid(mMainHUD))
		{
			mMainHUD->SetCourseText(TEXT("Tee"));

		}
	}
}

void ABall::CheckGoodShot()
{
	switch (mGolfClubType)
	{
	case EGolfClub::Driver:
		if (mMovingDis / 100.f >= 250.f && mHitMaterialType == EMaterialType::Fairway)
			mIsGoodShot = true;
		break;
	case EGolfClub::Wood:
		if (mMovingDis / 100.f >= 150.f && mHitMaterialType == EMaterialType::Fairway)
			mIsGoodShot = true;
		break;
	case EGolfClub::Iron:
	case EGolfClub::Wedge:
		//if (mHitMaterialType == EMaterialType::Green && !mIsOnGreen)
		//{
		//	mIsGoodShot = true;
		//	mIsOnGreen = true;
		//}
		break;
	case EGolfClub::Putter:

		break;
	}
}

void ABall::SetPuttingMode(bool isPutting)
{
	if (isPutting)
	{
		// MiniMap
		mMainHUD->SetMiniMapVisible(false);

		// Club
		mMainHUD->SetPuttingClub(true);

		// Trailer
		if (mTrailer->GetAsset()->GetEmitterHandle(1).IsValid() && mTrailer->GetAsset()->GetEmitterHandle(1).GetName() == "Ribbon" &&
			mTrailer->GetAsset()->GetEmitterHandle(2).IsValid() && mTrailer->GetAsset()->GetEmitterHandle(2).GetName() == "Fire")
		{
			mTrailer->GetAsset()->GetEmitterHandle(2).SetIsEnabled(false, *mTrailer->GetAsset(), true);
			mTrailer->GetAsset()->GetEmitterHandle(1).SetIsEnabled(true, *mTrailer->GetAsset(), true);
		}

		// Ball Info (Distance)
		mMainHUD->SetPuttingInfo(GetActorLocation(), mBallInfo.DestPos);
		mMainHUD->SetPuttingInfoVisible(true);
	}
	else
	{
		// MiniMap
		mMainHUD->SetMiniMapVisible(true);

		// Club
		mMainHUD->SetPuttingClub(false);

		// Trailer
		if (mTrailer->GetAsset()->GetEmitterHandle(1).IsValid() && mTrailer->GetAsset()->GetEmitterHandle(1).GetName() == "Ribbon" &&
			mTrailer->GetAsset()->GetEmitterHandle(2).IsValid() && mTrailer->GetAsset()->GetEmitterHandle(2).GetName() == "Fire")
		{
			mTrailer->GetAsset()->GetEmitterHandle(2).SetIsEnabled(true, *mTrailer->GetAsset(), true);
			//mTrailer->GetAsset()->GetEmitterHandle(1).SetIsEnabled(true, *mTrailer->GetAsset(), true);
		}

		// Ball Info (Distance)
		mMainHUD->SetPuttingInfoVisible(false);

	}

}

void ABall::ChangeCamera(float DeltaTime)
{
	//if (!mIsEnableSwing && mGolfClubType == EGolfClub::Driver && !mIsChangeCamera)
	//{
	//	mMovingTime += DeltaTime;

	//	// main -> side
	//	if (mMainCamera->IsActive() && mMovingTime > 1.5f)
	//	{
	//		mMainCamera->SetActive(false);
	//		mSideCamera->SetActive(true);
	//	}

	//	else if (mSideCamera->IsActive() && mMovingTime > 4.f)
	//	{
	//		mMainCamera->SetActive(true);
	//		mSideCamera->SetActive(false);

	//		mIsChangeCamera = true;
	//		mMovingTime = 0.f;
	//	}
	//}
}

void ABall::TestKey()
{
	//UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	//UScoreSubsystem* SubSystem = GameInst->GetSubsystem<UScoreSubsystem>();
	//UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	//if (IsValid(SubSystem))
	//{
	//	EPlayer CurPlayer = GameManager->GetCurPlayer();
	//	//EPlayer NextPlayer = GameManager->GetNextPlayer();

	//	FPlayerInfo CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);

	//	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Next Player: %d"), (int)NextPlayer));
	//	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Cur Player: %d"), (int)CurPlayer));

	//	if (IsValid(SubSystem))
	//	{
	//		FString ScoreText = SubSystem->GetScoreText(CurPlayerInfo.Shot);
	//		PrintViewport(3.f, FColor::Red, ScoreText);
	//	}
	//}

	//SetActorLocation(FVector(0.0, 0.0, 13.5));

	//mMainHUD->SetGoodShotVisible(true);

	//AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//ACameraActor* SideCamera = GameMode->GetSideCamera();
	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//BallController->SetViewTarget(SideCamera);

	//mMainCamera->SetActive(false);
	//mSideCamera->SetActive(true);
}

void ABall::Cheat()
{
	//FVector loc = GetActorLocation();

	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("loc X: %f"), loc.X));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("loc Y: %f"), loc.Y));
	//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("loc Z: %f"), loc.Z));

	mSpringArm->CameraLagSpeed = 0.f;

	FVector loc = FVector(37248.8, -1010.9, 2.475);
	SetActorLocation(loc);

	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mBallInfo.DestPos);
	//Rotator.Pitch = 0.f;
	//BallController->SetControlRotation(Rotator);
}

void ABall::NextGame()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (!IsValid(GameManager))
	{
		PrintViewport(5.f, FColor::Red, TEXT("NextGame() GameManager X"));
		return;
	}

	// Total Score ����
	mPlayType = GameManager->GetPlayType();
	FPlayerInfo Player1Info = GameManager->GetPlayerInfo(EPlayer::Player1);

	if (mPlayType == EPlayType::Multi)
	{
		FPlayerInfo Player2Info = GameManager->GetPlayerInfo(EPlayer::Player2);
		
		// Ranking
		if (Player1Info.Shot < Player2Info.Shot)	// p1
		{
			mMainHUD->SetPlayerRankingText(EPlayer::Player1, 1);
			mMainHUD->SetPlayerRankingText(EPlayer::Player2, 2);
		}

		else if (Player1Info.Shot > Player2Info.Shot)	// p2
		{
			mMainHUD->SetPlayerRankingText(EPlayer::Player1, 2);
			mMainHUD->SetPlayerRankingText(EPlayer::Player2, 1);
		}

		else
		{
			mMainHUD->SetPlayerRankingText(EPlayer::Player1, 1);
			mMainHUD->SetPlayerRankingText(EPlayer::Player2, 1);
		}

		// Player2 Score & Shot
		// ���±��� ģ �� shot ����
		Player2Info.TotalShot += Player2Info.Shot;
		int score = Player2Info.Shot - 4;
		Player2Info.Score += score;

		GameManager->SetPlayerInfo(Player2Info, (int)EPlayer::Player2);

		// ���� Ȧ���� ģ shot UI ����
		int turn = GameManager->GetTurn();
		mMainHUD->SetPlayerScoreText(EPlayer::Player2, turn - 1, Player2Info.Shot);

		// ���±��� ģ shot UI ����
		FString scoreText = "";
		mMainHUD->SetPlayerTotalScoreText(EPlayer::Player2, Player2Info.TotalShot, Player2Info.Score);

		// ���� �Ÿ� UI ����
		mMainHUD->SetPlayerTargetDistanceText(373.16, false);
	}

	// Player1 Score & Shot
	Player1Info.TotalShot += Player1Info.Shot;
	int score = Player1Info.Shot - 4;
	Player1Info.Score += score;

	GameManager->SetPlayerInfo(Player1Info, (int)EPlayer::Player1);

	int turn = GameManager->GetTurn();
	mMainHUD->SetPlayerScoreText(EPlayer::Player1, turn - 1, Player1Info.Shot);

	FString scoreText = "";
	mMainHUD->SetPlayerTotalScoreText(EPlayer::Player1, Player1Info.TotalShot, Player1Info.Score);

	// Total Score UI
	mMainHUD->SetTotalScoreVisible(true);
	mMainHUD->SetGamePlayVisible(true);
}

void ABall::CheckPlayerGoal()
{
	if (!mIsBallStopped)
		return;

	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	
	EPlayer CurPlayer;
	FPlayerInfo CurPlayerInfo;

	if (IsValid(GameManager) && mIsStart)
	{
		CurPlayer = GameManager->GetCurPlayer();
		CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);

		if (mIsConcede)
		{
			if (!mIsInHole)
				CurPlayerInfo.Shot++;

			CurPlayerInfo.TurnEnd = true;
			mMainHUD->SetPlayerShotNumText(CurPlayerInfo.Shot, true);
		}

		else if (CurPlayerInfo.Shot >= 8 && !mIsSetScore)
		{
			CurPlayerInfo.TurnEnd = true;
			mIsSetScore = true;

			UScoreSubsystem* ScoreSub = GameInst->GetSubsystem<UScoreSubsystem>();
			if (IsValid(ScoreSub))
			{
				FString ScoreText = ScoreSub->GetScoreText(CurPlayerInfo.Shot - 4);
				mMainHUD->SetScoreText(ScoreText);
			}
		}

		GameManager->SetPlayerInfo(CurPlayerInfo, (int)CurPlayer);
	}

	ShowScoreUI();
}

void ABall::ShowScoreUI()
{
	if (!mIsConcede)
		return;

	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	UScoreSubsystem* ScoreSub = GameInst->GetSubsystem<UScoreSubsystem>();

	EPlayer CurPlayer;
	FPlayerInfo CurPlayerInfo;

	if (IsValid(GameManager) && IsValid(mMainHUD))
	{
		CurPlayer = GameManager->GetCurPlayer();
		CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);

		if (!mIsInHole)
			mMainHUD->SetConcedeTextVisible(true);

		// ���� ���
		if (IsValid(ScoreSub))
		{
			FString ScoreText = ScoreSub->GetScoreText(CurPlayerInfo.Shot - 4);
			mMainHUD->SetScoreText(ScoreText);
		}
	}

	mIsConcede = false;
}

void ABall::Wind()
{
	if (mHitMaterialType == EMaterialType::Green)
		return;

	float vel = mStaticMesh->GetComponentVelocity().Size();

	if (!mIsEnableSwing && vel > 10.f)
	{
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
}

void ABall::UpdateWind()
{
	if (IsValid(mMainHUD))
	{
		if (mHitMaterialType == EMaterialType::Green)
		{
			mMainHUD->SetWindVisible(false);
			return;
		}

		mMainHUD->SetWindVisible(true);
		mMainHUD->SetWindTextVisible(mWindType, false);

		mWindType = EWindType(FMath::RandRange(0, 1));
		mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);

		mMainHUD->SetWindTextVisible(mWindType, true);
		mMainHUD->SetWindVelText(mWindPower / 10.f);
	}
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
