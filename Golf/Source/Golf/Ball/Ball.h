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
	void StopRotate();

	// Spin
	void AddForceToStraight();
	void AddForceToSide();

	void SetBallDetailTurn();
	void AddBallPower(float scale);
	void AddBallHeight(float scale);

	void ShowDistance();
	void PrintPower();

	// �浹
	void CheckMaterialCollision();
	void SetBallDetail();

	void ResetBallPos(float DeltaTime);
	void FindResetPos();

	// camera
	void ChangeCamera();

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
	USphereComponent*			mRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*		mStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	USpringArmComponent*		mSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent*			mCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent*	mProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	URotatingMovementComponent*		mRotating;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	EGolfClub	mGolfClubType;

	FBallInfo	mBallInfo;

private:
	bool		mIsEnableSwing;
	bool		mIsSwingStraight;
	bool		mIsSwingLeft;
	bool		mIsSwingRight;

	bool		mIsPowerUp;
	bool		mIsPowerTurn;
	bool		mIsHeightUp;

	double		mTempBallPower;

	float		mResetTime;
	bool		mIsResetPos;
	bool		mIsFindResetPos;
	FVector		mResetPos;

	FString		mHitMaterialName;

	class UMainHUDBase*		mMainHUD;

public:
	void SetStaticMesh(const FString& path);
	void SetBallMinPower(double power)
	{
		mBallInfo.BallMinPower = power;
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Min: %f"), mBallInfo.BallMinPower));
	}

	void SetBallMaxPower(double power)
	{
		mBallInfo.BallMaxPower = power;
		//PrintViewport(1.f, FColor::Red, FString::Printf(TEXT("Max: %f"), mBallInfo.BallMaxPower));
	}

	void SetSwingArc(float arc)
	{
		mBallInfo.BallHeight = arc;
	}

	void SetGolfClubType(EGolfClub GolfClub)
	{
		mGolfClubType = GolfClub;
	}

public:
	float GetDistanceToTarget(FVector src, FVector dst);
	bool IsBallStopped();
};
