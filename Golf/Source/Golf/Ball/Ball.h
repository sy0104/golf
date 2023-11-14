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

	void AddForceToStraight();
	void AddForceToLeft();
	void AddForceToRight();

	// 이동
	void MoveFront(float scale);
	void MoveSide(float scale);

	void ShowDistance();

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
	UProjectileMovementComponent* mProjectile;

	//// Physics
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	//float		mInitialSpeed;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	//float		mGravityScale;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	//bool		mIsBounce;

	//// 각도 저항 영향받기

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	//float		mBounciness;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	//float		mFriction;

private:
	FVector		mCameraOffset;
	FRotator	mCameraRotation;

	float		mSwingArc;

	FVector		mStartPos;
	FVector		mTargetPos;
	FVector		mStartToTarget;
	FVector		mTargetDir;

	bool		mIsSwingLeft;
	bool		mIsSwingRight;
	float		mSpinForce;

	class UMainHUDBase*		mMainHUD;

public:
	void SetStaticMesh(const FString& path);

public:
	float GetDistanceToTarget(FVector pos);
};
