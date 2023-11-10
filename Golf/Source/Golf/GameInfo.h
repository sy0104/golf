// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "DrawDebugHelpers.h"	// ������ ���� ��±��.
#include "EngineGlobals.h"
#include "Engine.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName	name;
};

UCLASS()
class GOLF_API UGameInfo : public UObject
{
	GENERATED_BODY()
	
};