// Fill out your copyright notice in the Description page of Project Settings.

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
	void AddBallPower(float scale);
	void SetSwingDir(float scale);
	void Swing();

	// Spin
	void AddForceToSide();

	// Distance
	void ShowDistance();

	// Collision
	void CheckMaterialCollision();
	void SetBallHitMaterial(FString MaterialName);
	void CheckBallStopped();

	// 
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

	// Good Shot
	void CheckGoodShot();

	void TestKey();


public:
	void Init(bool isEnd);

public:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, const FHitResult& Hit);

public:
	UFUNCTION(BlueprintCallable)
	void BallBounced(const FHitResult& Hit, const FVector& ImpactVelocity);
	
	UFUNCTION(BlueprintCallable)
	void BallStopped(const FHitResult& ImpactResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent*			mSphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*		mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mMainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGolfClub					mGolfClubType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EBallSwingType				mBallSwingType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EMaterialType				mHitMaterialType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//UParticleSystemComponent*	mTrailer;
	UNiagaraComponent*			mTrailer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mMinimapSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USceneCaptureComponent2D*	mMinimapCapture;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//UPaperSpriteComponent*		mMinimapCurrentBall;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EWindType					mWindType;

	FBallInfo	mBallInfo;
	EPlayType	mPlayType;

private:
	bool		mIsEnableSwing;

	bool		mIsPowerUp;
	bool		mIsBallStopped;

	float		mResetTime;
	bool		mIsResetPos;
	bool		mIsFindResetPos;
	FVector		mResetPos;

	float		mWindPower;
	float		mWindPowerMin;
	float		mWindPowerMax;
	bool		mIsWindBlow;

	float		mCameraLagSpeed;

	float		mChangeTurnTime;
	bool		mIsChangeTurn;

	bool		mIsConcede;
	bool		mIsInHole;
	
	int			mTurn;
	bool		mIsStart;
	bool		mIsEnd;
	bool		mIsMultiEnd;

	float		mMovingDis;
	bool		mIsGoodShot;
	bool		mIsOnGreen;

	class UMainHUDBase*		mMainHUD;
	class AFixedCamera*		mFixedCamera;

public:
	void SetGolfClubType(EGolfClub GolfClub);

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
};
