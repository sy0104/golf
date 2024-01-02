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
#include "../Camera/FixedCamera.h"
#include "../Manager/GameManager.h"

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
		//mStaticMesh->SetRelativeScale3D(FVector(12.0, 12.0, 12.0));
	}

	SetRootComponent(mStaticMesh);
	mStaticMesh->SetSimulatePhysics(true);
	mStaticMesh->SetAngularDamping(30.f);
	mStaticMesh->SetUseCCD(true);

	//// Sphere Component (Collision)
	mSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mSphereComponent->SetSphereRadius(2.3f);
	//mSphereComponent->SetSphereRadius(10.f);
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
	mSpringArm->CameraLagSpeed = 0.2;

	// main camera
	mMainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mMainCamera->SetupAttachment(mSpringArm);
	//mMainCamera->SetRelativeLocation(FVector(-230.0, 0.0, 120.0));
	mMainCamera->SetRelativeLocation(FVector(90.0, 0.0, 110.0));
	mMainCamera->SetRelativeRotation(FRotator(0.0, 0.0, 0.0));
	//mMainCamera->bConstrainAspectRatio = true;
	mMainCamera->SetAutoActivate(true);
	mMainCamera->SetActive(true);

	//// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	//mBallInfo.DestPos = FVector(4300000.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(37305.0, -1000.0, 0.0);
	mBallInfo.BallDis = 0.f;
	mBallInfo.BallPower = 0.0;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinForce = 10000.f;
	// mBallInfo.Score = -4;

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

	mIsConcede = false;
	mIsInHole = false;

	mGolfClubType = EGolfClub::Driver;
	mHitMaterialType = EMaterialType::Tee;

	// wind
	mWindType = EWindType(FMath::RandRange(0, 3));
	mWindPowerMin = 100.f;
	mWindPowerMax = 300.f;
	mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);
	mIsWindBlow = false;

	mCameraBlendTime = 10.f;

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
			//mMainHUD->SetShotNumText(mBallInfo.ShotNum);

			// Minimap
			mMainHUD->SetMiniMapHoleImage(mBallInfo.DestPos);

			// Wind
			mMainHUD->SetWindTextVisible(mWindType, true);
		}
	}

	mStaticMesh->OnComponentHit.AddDynamic(this, &ABall::OnHit);

	// Play Type 받아오기
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		mPlayType = GameManager->GetPlayType();

		if (mPlayType == EPlayType::Multi)
		{
			FPlayerInfo PlayerInfo = GameManager->GetPlayer(EPlayer::Player2);
			PlayerInfo.BallPos = GetActorLocation();
			GameManager->SetPlayer(PlayerInfo, (int)(EPlayer::Player2));
		}
	}

	//// 시작 카메라 설정 (Fixed Camera)
	//mFixedCamera = GameMode->GetFixedCamera();
	//
	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (IsValid(BallController))
	//	BallController->SetViewTarget(mMainCamera->GetOwner());
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

	CheckMaterialCollision();
	
	CheckBallStopped();

	CheckChangeTurn(DeltaTime);

	SetBallDetailsByMaterial();

	ResetBallPos(DeltaTime);

	FindResetPos(DeltaTime);

	// Wind();

	CheckPlayerGoal();

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
	PlayerInputComponent->BindAction<ABall>(TEXT("CloseTotalScore"), EInputEvent::IE_Released, this, &ABall::CloseTotalScore);

	// 축 매핑
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
	PlayerInputComponent->BindAxis<ABall>(TEXT("ShowTotalScore"), this, &ABall::ShowTotalScore);
}

void ABall::Swing()
{
	if (!mIsEnableSwing)
		return;

	mIsEnableSwing = false;
	mIsWindBlow = true;
	mTrailer->Activate();
	mSpringArm->CameraLagSpeed = 0.7;

	//mMainHUD->SetBallStateVisible(false);

	// Spin
	if (mBallSwingType == EBallSwingType::Left)
		mIsSwingLeft = true;

	else if (mBallSwingType == EBallSwingType::Right)
		mIsSwingRight = true;

	// Club에 따라 볼 정보(dis, arc) 설정
	SetBallInfoByClub(mGolfClubType);

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();

	// Swing
	FVector TargetPos = mBallInfo.StartPos + (mMainCamera->GetForwardVector() * (mBallInfo.BallDis * mBallInfo.BallPower));
	FVector OutVelocity = FVector::ZeroVector;

	if (mGolfClubType == EGolfClub::Putter)
	{
		OutVelocity = mMainCamera->GetForwardVector() * FVector(1.0, 1.0, 1.0) * (mBallInfo.BallDis * mBallInfo.BallPower);
		OutVelocity = mMainCamera->GetForwardVector() * FVector(50, 0, 0);
	}

	else
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this, OutVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallArc);

	mStaticMesh->AddImpulse(OutVelocity);

	// Ball Info 초기화
	mBallInfo.BallPower = 0.f;
	mBallInfo.BallDis = 0.0;
	mMainHUD->SetBallPower(0.f);
	mMainHUD->SetMiniMapVisible(false);

	// Player UI 업데이트 (Shot)
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		EPlayer CurPlayer = GameManager->GetCurPlayer();
		FPlayerInfo CurPlayerInfo = GameManager->GetCurPlayerInfo();
		CurPlayerInfo.Shot++;
		GameManager->SetPlayer(CurPlayerInfo, (int)CurPlayer);

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetShotNumText(CurPlayerInfo.Shot, true);
		}
	}

	// UI Update
	if (IsValid(mMainHUD))
		mMainHUD->SetBallInfoVisible(false);
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
	if (scale == 0.f || !mIsEnableSwing)
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
		// Distance UI
		mMainHUD->SetDistanceText(dis / 100.f);
		mMainHUD->SetLeftDistanceText(leftDis / 100.f);

		// Player Info UI
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
		// CalculateScore();
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

	if (vel < 5.0)
		mIsWindBlow = false;

	if (vel < 1.0)
	{
		mStaticMesh->ComponentVelocity = FVector(0.0, 0.0, 0.0);
		mIsBallStopped = true;
		//mTrailer->Deactivate();

		// mIsEnableSwing = true;

		//UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
		//UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

		//if (IsValid(GameManager))
		//{

		//}

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

		if (IsValid(mMainHUD) && mIsEnableSwing)
		{
			mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetBallStateVisible(true);
			//mMainHUD->SetShotNumText(mBallInfo.ShotNum);
			
			UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
			UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
			EPlayer CurPlayer = GameManager->GetCurPlayer();
			FPlayerInfo CurPlayerInfo = GameManager->GetPlayer(CurPlayer);
			mMainHUD->SetMiniMapBallCurrent(CurPlayerInfo.BallPos);
			mMainHUD->SetMiniMapBallTarget(CurPlayerInfo.BallPos, mMainCamera->GetForwardVector(), mGolfClubType);
			if(FVector::Dist(CurPlayerInfo.BallPos, mBallInfo.DestPos) > 3000)
				mMainHUD->SetMiniMapVisible(true);
			mMainHUD->SetBallDistance(mGolfClubType);
			mMainHUD->SetHoleMark(CurPlayerInfo.BallPos, mBallInfo.DestPos);
		}
	}

	else
	{
		mIsBallStopped = false;
		// mIsWindBlow = false;

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetBallStateVisible(false);
			mMainHUD->SetMiniMapVisible(false);
			//mTrailer->Activate();
		}
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

	if (!mIsChangeTurn && mChangeTurnTime >= 2.f)
	{
		//PrintViewport(1.f, FColor::Red, TEXT("Change Turn"));
		mIsChangeTurn = true;

		ChangeTurn();
	}
}

void ABall::SetBallInfoByClub(EGolfClub club)
{
	switch (club)
	{
	case EGolfClub::Driver:	// 최대 300m 정도
		mBallInfo.BallDis = 2300.f;
		mBallInfo.BallArc = 0.6f;
		//mBallInfo.BallArc = 1.0f;
		break;
	case EGolfClub::Wood:	// 최대 215m 정도
		mBallInfo.BallDis = 1600.f;
		mBallInfo.BallArc = 0.7f;
		break;
	case EGolfClub::Iron:	// 최대 180m 정도
		mBallInfo.BallDis = 1400.f;
		mBallInfo.BallArc = 0.7f;
		break;
	case EGolfClub::Wedge:	// 최대 70m 정도
		mBallInfo.BallDis = 550.f;
		mBallInfo.BallArc = 0.5f;
		break;
	case EGolfClub::Putter:	// 최대 20m 정도
		mBallInfo.BallDis = 500.f;
		mBallInfo.BallArc = 0.99f;
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

void ABall::SetPlayerInfoUI(EPlayType PlayType, bool isNextPlayerEnd)
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	EPlayer CurPlayer, NextPlayer;
	FPlayerInfo CurPlayerInfo, NextPlayerInfo;

	if (IsValid(GameManager))
	{
		CurPlayer = GameManager->GetCurPlayer();
		CurPlayerInfo = GameManager->GetPlayer(CurPlayer);
	}

	if (IsValid(mMainHUD))
	{
		if (mPlayType == EPlayType::Single)
		{
			mMainHUD->SetShotNumText(CurPlayerInfo.Shot, true);
			//mMainHUD->SetScoreText(CurPlayerInfo.Score, true);
			mMainHUD->SetTargetDistanceText(CurPlayerInfo.LeftDistance / 100.f);
		}

		else if (mPlayType == EPlayType::Multi)
		{
			if (!isNextPlayerEnd)
			{
				NextPlayer = GameManager->GetNextPlayer();
			}

			else
			{
				NextPlayer = GameManager->GetCurPlayer();
			}

			NextPlayerInfo = GameManager->GetPlayer(NextPlayer);

			// Player Info - 다음 플레이어(턴이 돌아오는 플레이어)
			mMainHUD->SetPlayerImage(NextPlayerInfo.ImagePath, true);
			mMainHUD->SetPlayerNameText(NextPlayerInfo.Name, true);
			mMainHUD->SetShotNumText(NextPlayerInfo.Shot, true);
			//mMainHUD->SetScoreText(NextPlayerInfo.Score, true);
			mMainHUD->SetTargetDistanceText(NextPlayerInfo.LeftDistance / 100.f);

			// Player Simple Info - 현재 플레이어(방금 스윙한 플레이어)
			mMainHUD->SetPlayerImage(CurPlayerInfo.ImagePath, false);
			mMainHUD->SetPlayerNameText(CurPlayerInfo.Name, false);
			mMainHUD->SetShotNumText(CurPlayerInfo.Shot, false);
			//mMainHUD->SetScoreText(CurPlayerInfo.Score, false);

		}
	}
}

void ABall::ChangeTurn()
{
	if (!mIsChangeTurn)
		return;

	mTrailer->Deactivate();

	// Players 정보 업데이트
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		// Player 정보 갱신
		EPlayer CurPlayer = EPlayer::Player1;
		
		if (mPlayType == EPlayType::Single)
			CurPlayer = EPlayer::Player1;
		else
			CurPlayer = GameManager->GetCurPlayer();

		FPlayerInfo CurPlayerInfo = GameManager->GetPlayer(CurPlayer);	// 방금 스윙한 플레이어
		EPlayer NextPlayer = GameManager->GetNextPlayer();

		//CurPlayerInfo.Score++;
		CurPlayerInfo.BallPos = GetActorLocation();

		bool isNextEnd = false;

		if (mPlayType == EPlayType::Single)
		{

		}

		else if (mPlayType == EPlayType::Multi)
		{
			FPlayerInfo NextPlayerInfo = GameManager->GetPlayer(NextPlayer);	// 이제 스윙할 플레이어

			// 이제 스윙할 차례인 플레이어가 이미 홀에 공을 넣었을 경우
			if (NextPlayerInfo.TurnEnd)
			{
				isNextEnd = true;
			}

			mSpringArm->CameraLagSpeed = 0.f;
			SetActorLocation(NextPlayerInfo.BallPos);
		}

		// 현재 턴 플레이어로 UI 갱신
		SetPlayerInfoUI(mPlayType, isNextEnd);

		// CurPlayer와 NextPlayer 변경
		GameManager->SetPlayer(CurPlayerInfo, (int)CurPlayer);

		if (mPlayType == EPlayType::Multi)
		{
			if (!isNextEnd)
				GameManager->SetCurPlayer((int)NextPlayer);
			else
				GameManager->SetCurPlayer((int)CurPlayer);
		}

		// Turn 증가
		GameManager->AddTurn(1);
	}

	// 턴 넘어갈 때 변수들 초기화
	mIsWindBlow = false;
	mIsEnableSwing = true;
	mIsChangeTurn = false;
	mChangeTurnTime = 0.f;

	// 바람
	UpdateWind();

	// UI 업데이트
	if (IsValid(mMainHUD))
	{
		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetBallInfoVisible(true);
	}
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

void ABall::TestKey()
{
	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//BallController->SetViewTargetWithBlend(mFixedCamera, mCameraBlendTime);

	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	//UScoreSubsystem* SubSystem = GameInst->GetSubsystem<UScoreSubsystem>();
	//
	//if (IsValid(SubSystem))
	//{
	//	PrintViewport(1.f, FColor::Red, TEXT("Sub System Valid"));
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
	{
		EPlayer CurPlayer = GameManager->GetCurPlayer();
		EPlayer NextPlayer = GameManager->GetNextPlayer();

		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("vel Z: %f"), vel.Z));

		PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Next Player: %d"), (int)NextPlayer));
		PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Cur Player: %d"), (int)CurPlayer));
	}
}

void ABall::CheckPlayerGoal()
{
	if (!mIsBallStopped || !mIsConcede)
		return;

	if (!mIsInHole)
	{
		UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
		UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

		EPlayer CurPlayer = GameManager->GetCurPlayer();
		FPlayerInfo CurPlayerInfo = GameManager->GetPlayer(CurPlayer);

		CurPlayerInfo.Shot++;
		GameManager->SetPlayer(CurPlayerInfo, (int)CurPlayer);
	}

	CalculateScore();
}

void ABall::CalculateScore()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	UScoreSubsystem* ScoreSub = GameInst->GetSubsystem<UScoreSubsystem>();
	FString ScoreText = "";

	// 현재 플레이어 가져오기
	EPlayer CurPlayer = GameManager->GetCurPlayer();
	FPlayerInfo CurPlayerInfo = GameManager->GetPlayer(CurPlayer);

	// 점수 계산
	if (IsValid(ScoreSub))
		ScoreText = ScoreSub->GetScoreText(CurPlayerInfo.Shot);

	// UI 적용
	if (IsValid(mMainHUD))
		mMainHUD->SetScoreText(ScoreText);

	// 해당 플레이어 종료 설정
	CurPlayerInfo.TurnEnd = true;
	GameManager->SetPlayer(CurPlayerInfo, (int)CurPlayer);
}

void ABall::Wind()
{
	if (!mIsWindBlow)
		return;

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

void ABall::UpdateWind()
{
	if (IsValid(mMainHUD))
	{
		mMainHUD->SetWindTextVisible(mWindType, false);

		mWindType = EWindType(FMath::RandRange(0, 3));
		mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);

		mMainHUD->SetWindTextVisible(mWindType, true);
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

void ABall::SetStaticMesh(const FString& path)
{
	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(StaticMesh))
		mStaticMesh->SetStaticMesh(StaticMesh);
}

void ABall::SetGolfClubType(EGolfClub GolfClub)
{
	mGolfClubType = GolfClub;
}
