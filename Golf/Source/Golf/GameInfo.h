// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"	// 디버깅용 도형 출력기능.
#include "EngineGlobals.h"
#include "Engine.h"
#include "LandscapeInfo.h"
#include "Kismet/KismetMathLibrary.h"

#include "UObject/NoExportTypes.h"
#include "GameInfo.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PF, Log, All);

#define	LOG_CALLINFO		(FString(__FUNCTION__) + TEXT("[") + FString::FromInt(__LINE__) + TEXT("]"))

#define LOG(Format, ...)	UE_LOG(PF, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define LOGSTRING(Str)		UE_LOG(PF, Warning, TEXT("%s : %s"), *LOG_CALLINFO, *Str)

void PrintViewport(float Time, const FColor& Color, const FString& Text);



USTRUCT(BlueprintType)
struct FBallInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector		StartPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector		DestPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		BallPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		BallRatio;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		BallArc;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		SpinPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		SpinRatio;
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		Name;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString		ImagePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		Shot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	int32		TotalShot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	float		LeftDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector		BallPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool		TurnEnd;
};

UENUM(BlueprintType)
enum class EGolfClub : uint8
{
	Driver,
	Wood,
	Iron,
	Wedge,
	Putter,
};

UENUM(BlueprintType)
enum class EIronType : uint8
{
	Iron5,
	Iron6,
	Iron7,
	Iron8,
	Iron9,
};

UENUM(BlueprintType)
enum class ECameraType : uint8
{
	Main,
	Side,
};

UENUM(BlueprintType)
enum class EBallSwingType : uint8
{
	Straight,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EMaterialType : uint8
{
	Tee,
	Fairway,
	Green,
	Rough,
	Water,
	Bunker,
	Road,
	OB
};

UENUM(BlueprintType)
enum class EWindType : uint8
{
	Left,
	Right,
	Forward,
	Back
};

UENUM(BlueprintType)
enum class EPlayType : uint8
{
	Single,
	Multi
};

UENUM(BlueprintType)
enum class EPlayer : uint8
{
	Player1,
	Player2
};

UCLASS()
class GOLF_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};
