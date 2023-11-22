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
	void SetCamera();
	void SwingStraight();
	void SwingLeft();
	void SwingRight();
	void Roll();

	// Dir
	void SetSwingDir(float scale);
	void SetBallDirLeft();
	void SetBallDirRight();

	// Spin
	void AddForceToStraight();
	void AddForceToSide();
	void LimitRotation(float DeltaTime);

	// �̵�
	void MoveFront(float scale);
	void MoveSide(float scale);

	void AddBallPower(float scale);

	void ShowDistance();
	void PrintPower();

	// �浹
	void CheckMaterialCollision();
	void CheckLandscapeCollision();
	void SetBallDetail();

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

	FBallInfo	mBallInfo;

private:
	FVector		mCameraOffset;
	FRotator	mCameraRotation;

	double		mMinRotation;
	double		mMaxRotation;

	bool		mIsSwingStraight;
	bool		mIsSwingLeft;
	bool		mIsSwingRight;

	bool		mIsPowerUp;
	bool		mIsGround;

	double		mTempBallPower;
	double		mAddPower;

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
		mBallInfo.SwingArc = arc;
	}

public:
	float GetDistanceToTarget(FVector pos);
};
