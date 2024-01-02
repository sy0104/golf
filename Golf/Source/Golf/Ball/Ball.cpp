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
	}

	SetRootComponent(mStaticMesh);
	mStaticMesh->SetSimulatePhysics(true);
	mStaticMesh->SetAngularDamping(50.f);
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
	mCameraLagSpeed = 1.f;
	mSpringArm->CameraLagSpeed = mCameraLagSpeed;

	// main camera
	mMainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mMainCamera->SetupAttachment(mSpringArm);
	mMainCamera->SetRelativeLocation(FVector(250.0, 0.0, 50.0));
	mMainCamera->SetRelativeRotation(FRotator(-5.0, 0.0, 0.0));
	mMainCamera->bConstrainAspectRatio = true;
	mMainCamera->SetAutoActivate(true);
	mMainCamera->SetActive(true);

	//// Ball Info
	mBallInfo.StartPos = FVector(0.0, 0.0, 0.0);
	mBallInfo.DestPos = FVector(37303.0, -998.0, 0.0);
	mBallInfo.BallDis = 0.f;
	mBallInfo.BallPower = 0.0;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinForce = 200.f;

	// spin
	mIsEnableSwing = true;

	// power
	mIsPowerUp = true;

	mResetTime = 0.f;
	mIsResetPos = false;
	mIsFindResetPos = false;

	mIsBallStopped = true;

	mIsConcede = false;
	mIsInHole = false;

	mIsMultiEnd = false;
	mTurn = 0;
	mIsStart = false;
	mIsEnd = false;

	mGolfClubType = EGolfClub::Driver;
	mHitMaterialType = EMaterialType::Tee;

	// wind
	mWindType = EWindType(FMath::RandRange(0, 3));
	mWindPowerMin = 100.f;
	mWindPowerMax = 300.f;
	mWindPower = FMath::RandRange(mWindPowerMin, mWindPowerMax);
	mIsWindBlow = false;

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
		}
	}
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

	//mBallInfo.BallDis = 300.f;
	//FVector TargetPos = GetActorLocation() + (mMainCamera->GetForwardVector() * mBallInfo.BallDis);
	//FVector force = mMainCamera->GetForwardVector()* FVector(1.0, 1.0, 1.0) * mBallInfo.BallDis;
	////mStaticMesh->AddForceAtLocation(force, TargetPos);
	//mStaticMesh->AddForce(force);
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �׼� ����
	PlayerInputComponent->BindAction<ABall>(TEXT("SwingStraight"), EInputEvent::IE_Pressed, this, &ABall::Swing);
	PlayerInputComponent->BindAction<ABall>(TEXT("Test"), EInputEvent::IE_Pressed, this, &ABall::TestKey);
	PlayerInputComponent->BindAction<ABall>(TEXT("CloseTotalScore"), EInputEvent::IE_Released, this, &ABall::CloseTotalScore);
	PlayerInputComponent->BindAction<ABall>(TEXT("Menu"), EInputEvent::IE_Pressed, this, &ABall::ShowMenu);

	// �� ����
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallDir"), this, &ABall::SetSwingDir);
	PlayerInputComponent->BindAxis<ABall>(TEXT("BallPower"), this, &ABall::AddBallPower);
	PlayerInputComponent->BindAxis<ABall>(TEXT("ShowTotalScore"), this, &ABall::ShowTotalScore);
}

void ABall::Swing()
{
	if (!mIsEnableSwing)
		return;

	if (!mIsStart)
		mIsStart = true;

	mIsEnableSwing = false;
	mIsWindBlow = true;
	mTrailer->Activate();
	mSpringArm->CameraLagSpeed = mCameraLagSpeed;

	//mMainHUD->SetBallStateVisible(false);

	// Club�� ���� �� ����(dis, arc) ����
	SetBallInfoByClub(mGolfClubType);

	// Ball Info
	mBallInfo.StartPos = GetActorLocation();

	// Swing
	FVector TargetPos = mBallInfo.StartPos + (mMainCamera->GetForwardVector() * (mBallInfo.BallDis * mBallInfo.BallPower));
	FVector OutVelocity = FVector::ZeroVector;

	if (mGolfClubType == EGolfClub::Putter)
	{
		OutVelocity = mMainCamera->GetForwardVector() * FVector(1.0, 1.0, 1.0) * (mBallInfo.BallDis * mBallInfo.BallPower);
		// OutVelocity = mMainCamera->GetForwardVector() * FVector(50, 0, 0);
	}

	else
		UGameplayStatics::SuggestProjectileVelocity_CustomArc(
			this, OutVelocity, mBallInfo.StartPos, TargetPos, GetWorld()->GetGravityZ(), mBallInfo.BallArc);

	mStaticMesh->AddImpulse(OutVelocity);

	// Ball Info �ʱ�ȭ
	mBallInfo.BallPower = 0.f;
	mBallInfo.BallDis = 0.0;
	mMainHUD->SetBallPower(0.f);
	mMainHUD->SetMiniMapVisible(false);

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
		mMainHUD->SetBallInfoVisible(false);

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
	if (mIsEnableSwing)
		return;

	FVector forwardVec = mBallInfo.DestPos;
	forwardVec.Normalize();

	FVector leftVec = FVector(GetActorLocation().X, GetActorLocation().Y - 90.0, GetActorLocation().Z);
	leftVec.Normalize();

	FVector CrossVec = FVector::CrossProduct(forwardVec, leftVec);

	if (mBallSwingType == EBallSwingType::Right)
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
		mMainHUD->SetPlayerTargetDistanceText(leftDis / 100.f);
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
	{
		mIsWindBlow = false;
		// mTrailer->Deactivate();
	}

	if (vel < 1.0)
	{
		mStaticMesh->ComponentVelocity = FVector(0.0, 0.0, 0.0);
		mIsBallStopped = true;
		mTrailer->Deactivate();

		if (IsValid(mMainHUD) && mIsEnableSwing)
		{
			mMainHUD->SetDistanceText(0.f);
			mMainHUD->SetBallStateVisible(true);
			//mMainHUD->SetShotNumText(mBallInfo.ShotNum);
			
			UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
			UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
			EPlayer CurPlayer = GameManager->GetCurPlayer();
			FPlayerInfo CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);
			mMainHUD->SetMiniMapBallCurrent(CurPlayerInfo.BallPos);
			mMainHUD->SetMiniMapBallTarget(CurPlayerInfo.BallPos, mMainCamera->GetForwardVector(), mGolfClubType);
			mMainHUD->SetMiniMapVisible(true);
		}

		//// Score UI
		//if (mIsConcede)
		//	ShowScoreUI();
	}

	else
	{
		mIsBallStopped = false;
		
		if (mIsStart)
			mTrailer->Activate();

		// mIsWindBlow = false;

		if (IsValid(mMainHUD))
		{
			mMainHUD->SetBallStateVisible(false);
			mMainHUD->SetMiniMapVisible(false);
			//mTrailer->Activate();
		}
	}
}

void ABall::SetBallInfoByClub(EGolfClub club)
{
	switch (club)
	{
	case EGolfClub::Driver:	// �ִ� 300m ����
		mBallInfo.BallDis = 2300.f;
		mBallInfo.BallArc = 0.6f;
		//mBallInfo.BallArc = 1.0f;
		break;
	case EGolfClub::Wood:	// �ִ� 215m ����
		mBallInfo.BallDis = 1600.f;
		mBallInfo.BallArc = 0.7f;
		break;
	case EGolfClub::Iron:	// �ִ� 180m ����
		mBallInfo.BallDis = 1400.f;
		mBallInfo.BallArc = 0.7f;
		break;
	case EGolfClub::Wedge:	// �ִ� 70m ����
		mBallInfo.BallDis = 550.f;
		mBallInfo.BallArc = 0.5f;
		break;
	case EGolfClub::Putter:	// �ִ� 20m ����
		mBallInfo.BallDis = 300.f;
		mBallInfo.BallArc = 0.99f;
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
			if (mTurn >= 2 && mTurn % 2 == 0)
			{
				float dis1 = GameManager->GetPlayerLeftDis(EPlayer::Player1);
				float dis2 = GameManager->GetPlayerLeftDis(EPlayer::Player2);

				if (dis1 >= dis2)	// player1 ����
					GameManager->SetCurPlayer((int)EPlayer::Player1);

				else	// player2 ����
					GameManager->SetCurPlayer((int)EPlayer::Player2);
			}

			else
				GameManager->SetCurPlayer((int)NextPlayer);

			mSpringArm->CameraLagSpeed = 0.f;
			SetActorLocation(GameManager->GetCurPlayerInfo().BallPos);
		}
		
		// 2�� �� 1���� ���� Ȧ�� ���� ���� ���: �÷��̾� ���� �ٲ��� ����
		else if (CurPlayerInfo.TurnEnd)
		{
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
		// ��Ƽ���� 2�� �� Ȧ�� �־��� ���: ���� ����6
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

	// Player Info UI ����
	SetPlayerInfoUI(GameManager->GetCurPlayer(), true);

	// �� �Ѿ �� ������ �ʱ�ȭ
	mIsWindBlow = false;
	mIsEnableSwing = true;
	mIsChangeTurn = false;
	mChangeTurnTime = 0.f;

	//// Hole ���� �ٶ󺸵���
	//FVector dir = mBallInfo.DestPos - GetActorLocation();
	//dir.Z = 0.f;
	//dir.Normalize();
	//FRotator rot = GetActorRotation();
	//SetActorRotation(FRotator(0.f, dir.Rotation().Yaw, 0.f));
	//FRotator crot = mMainCamera->GetRelativeRotation();
	//mMainCamera->SetRelativeRotation(FRotator(0.f, dir.Rotation().Yaw, 0.f));
	//crot = mMainCamera->GetRelativeRotation();
	//rot = GetActorRotation();

	//FRotator Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mBallInfo.DestPos);
	//mMainCamera->SetRelativeRotation(FRotator(0.f, 0.f, Rotator.Pitch));
	//mMainCamera->SetRelativeRotation(Rotator);

	// �ٶ�
	UpdateWind();

	// UI ������Ʈ
	if (IsValid(mMainHUD))
	{
		mMainHUD->SetDistanceText(0.f);
		mMainHUD->SetBallInfoVisible(true);
		mMainHUD->SetScoreTextVisible(false);
		mMainHUD->SetConcedeTextVisible(false);
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
	mBallInfo.BallDis = 0.f;
	mBallInfo.BallPower = 0.f;
	mBallInfo.BallArc = 0.4f;
	mBallInfo.SpinForce = 200.f;

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

	if (!isEnd)
	{
		// Player Info UI �ʱ�ȭ
		SetPlayerInfoUI(EPlayer::Player1, true);
		SetPlayerInfoUI(EPlayer::Player2, false);

		// Total Score UI ���� & GamePlay UI Ȱ��ȭ
		if (IsValid(mMainHUD))
		{
			mMainHUD->SetCourseText(TEXT("Tee"));

			//// Total Score UI
			//EPlayType PlayType = GameManager->GetPlayType();
			//if (PlayType == EPlayType::Multi)
			//{
			//	// ��ŷ ���

			//}

			// Game Play UI
			mMainHUD->SetGamePlayVisible(true);
		}
	}
}

void ABall::TestKey()
{
	//ABallController* BallController = Cast<ABallController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//BallController->SetViewTargetWithBlend(mFixedCamera, mCameraBlendTime);

	//UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	//UScoreSubsystem* SubSystem = GameInst->GetSubsystem<UScoreSubsystem>();
	//
	//if (IsValid(SubSystem))
	//{
	//	EPlayer CurPlayer = GameManager->GetCurPlayer();
	//	EPlayer NextPlayer = GameManager->GetNextPlayer();

	//	FPlayerInfo CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);

	//	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Next Player: %d"), (int)NextPlayer));
	//	//PrintViewport(3.f, FColor::Red, FString::Printf(TEXT("Cur Player: %d"), (int)CurPlayer));

	//	if (IsValid(SubSystem))
	//	{
	//		FString ScoreText = SubSystem->GetScoreText(CurPlayerInfo.Shot);
	//		PrintViewport(3.f, FColor::Red, ScoreText);
	//	}
	//}

	SetActorLocation(FVector(0.0, 0.0, 13.5));
}

void ABall::NextGame()
{
	PrintViewport(2.f, FColor::Red, TEXT("Next Game"));

	//mMainHUD->SetVisibility(ESlateVisibility::Hidden);

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

		else if (Player1Info.Shot > Player2Info.Shot)
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
	if (!mIsBallStopped || !mIsConcede)
		return;

	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();
	
	EPlayer CurPlayer;
	FPlayerInfo CurPlayerInfo;

	if (IsValid(GameManager))
	{
		CurPlayer = GameManager->GetCurPlayer();
		CurPlayerInfo = GameManager->GetPlayerInfo(CurPlayer);
		CurPlayerInfo.TurnEnd = true;

		if (!mIsInHole)	// concede
			CurPlayerInfo.Shot++;

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
			FString ScoreText = ScoreSub->GetScoreText(CurPlayerInfo.Shot);
			mMainHUD->SetScoreText(ScoreText);
		}
	}

	mIsConcede = false;
}

void ABall::Wind()
{
	if (!mIsWindBlow)
		return;

	FVector dirVec;
	PrintViewport(1.f, FColor::Red, TEXT("Wind"));

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
