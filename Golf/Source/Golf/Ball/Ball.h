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

	// Spin
	void AddForceToStraight();
	void AddForceToSide();

	// �̵�
	void MoveFront(float scale);
	void MoveSide(float scale);

	void AddBallPower(float scale);

	void ShowDistance();

	UFUNCTION()
	void BallBounced();
	
	UFUNCTION()
	void BallStopped();

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	URotatingMovementComponent*		mRotating;

	FBallInfo	mBallInfo;

private:
	FVector		mCameraOffset;
	FRotator	mCameraRotation;

	bool		mIsSwingStraight;
	bool		mIsSwingLeft;
	bool		mIsSwingRight;

	bool		mIsPowerUp;

	double		mTempBallPower;
	double		mAddPower;

	class UMainHUDBase*		mMainHUD;

public:
	void SetStaticMesh(const FString& path);

public:
	float GetDistanceToTarget(FVector pos);
};
