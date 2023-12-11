// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Pawn.h"
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
	void SwingStraight();
	void SwingLeft();
	void SwingRight();
	void Roll();

	// Dir
	void SetSwingDir(float scale);

	// Spin
	void AddForceToStraight();
	void AddForceToSide();

	void AddBallPower(float scale);

	void ShowDistance();

	// Ãæµ¹
	void CheckMaterialCollision();
	void SetBallDetail();

	void ResetBallPos(float DeltaTime);
	void FindResetPos(float DeltaTime);

	void CheckBallStopped();

	// Club
	void SetBallInfoByClub(EGolfClub club);

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
	UCameraComponent*			mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGolfClub	mGolfClubType;

	FBallInfo	mBallInfo;

private:
	bool		mIsEnableSwing;
	bool		mIsSwingStraight;
	bool		mIsSwingLeft;
	bool		mIsSwingRight;

	bool		mIsPowerUp;
	bool		mIsBallStopped;

	float		mResetTime;
	bool		mIsResetPos;
	bool		mIsFindResetPos;
	FVector		mResetPos;

	FString		mHitMaterialName;

	class UMainHUDBase*		mMainHUD;

public:
	void SetStaticMesh(const FString& path);
	void SetSwingArc(float arc)
	{
		//mBallInfo.BallHeight = arc;
	}

	void SetGolfClubType(EGolfClub GolfClub)
	{
		mGolfClubType = GolfClub;
	}

	void SetBallPower(float power)
	{
		mBallInfo.BallPower = power;
	}

public:
	float GetDistanceToTarget(FVector src, FVector dst);
};
