#pragma once

#include "../GameInfo.h"
#include "GameFramework/Pawn.h"
#include <NiagaraComponent.h>
#include <PaperSpriteComponent.h>
#include "Ball.generated.h"

UCLASS()
class GOLF_API ABall : public APawn
{
	GENERATED_BODY()

public:
	ABall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// Swing
	void SetBallDetail(float scale);
	void AddBallPower(float scale);
	void SetSwingDir(float scale);
	void Swing();

	// Spin
	void AddBallSpin(float scale);
	void AddForceToSide();

	// Distance
	void ShowDistance();

	// Collision
	void CheckMaterialCollision();
	void CheckBallStopped();

	// Material
	void SetBallHitMaterial(FString MaterialName);
	void SetBallDetailsByMaterial();
	void ResetBallPos(float DeltaTime);
	void FindResetPos(float DeltaTime);

	// Club
	void SetBallInfoByClub(EGolfClub club);

	// Score
	void CheckPlayerGoal();
	void ShowScoreUI();

	// Wind
	void Wind();
	void UpdateWind();

	// Turn (Multi)
	void SetPlayerInfoUI(EPlayer player, bool isDetail);
	void CheckChangeTurn(float DeltaTime);
	void ChangeTurn();
	void NextGame();

	// UI
	void ShowTotalScore(float scale);
	void CloseTotalScore();
	void ShowMenu();
	void CheckGoodShot();

	// Putting
	void SetPuttingMode(bool isPutting);

	// Camera
	void ChangeCamera(float DeltaTime);

	// Test
	void TestKey();
	void Cheat();

public:
	void Init(bool isEnd);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USphereComponent*			mSphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*		mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mMainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSideSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mSideCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGolfClub					mGolfClubType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EIronType					mIronType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EBallSwingType				mBallSwingType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMaterialType				mHitMaterialType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UNiagaraComponent*			mTrailer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mMinimapSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USceneCaptureComponent2D*	mMinimapCapture;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EWindType					mWindType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase*					mSwingSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USoundBase*					mGoodShotSound;

	FBallInfo	mBallInfo;
	EPlayType	mPlayType;

private:
	bool		mIsEnableSwing;
	bool		mIsBallStopped;

	bool		mIsAddPower;
	bool		mIsAddSpin;
	bool		mIsPowerUp;
	bool		mIsSpinUp;

	float		mResetTime;
	bool		mIsResetPos;
	bool		mIsFindResetPos;
	FVector		mResetPos;

	float		mWindPower;
	float		mWindPowerMin;
	float		mWindPowerMax;

	float		mCameraLagSpeed;

	float		mChangeTurnTime;
	bool		mIsChangeTurn;

	bool		mIsSetScore;
	bool		mIsConcede;
	bool		mIsInHole;
	
	bool		mIsStart;
	bool		mIsEnd;

	float		mMovingDis;
	bool		mIsGoodShot;
	bool		mIsPlayGoodShotSound;
	bool		mIsOnGreen;

	bool		mIsChangeCamera;
	float		mMovingTime;

	bool		mIsRetry;

	class UMainHUDBase*		mMainHUD;

public:
	void SetGolfClubType(EGolfClub GolfClub)
	{
		mGolfClubType = GolfClub;
	}

	void SetBallSwingType(EBallSwingType SwingType)
	{
		mBallSwingType = SwingType;
	}

	void SetPlayType(EPlayType PlayType)
	{
		mPlayType = PlayType;
	}

	void SetConcede(bool concede)
	{
		mIsConcede = concede;
	}

	void SetInHole(bool InHole)
	{
		mIsInHole = InHole;
	}

	void SetStart(bool start)
	{
		mIsStart = start;
	}

	void SetIsAddPower(bool power)
	{
		mIsAddPower = power;
	}

	void SetIsAddSpin(bool spin)
	{
		mIsAddSpin = spin;
	}

	void SetIronType(EIronType IronType)
	{
		mIronType = IronType;
	}
};
