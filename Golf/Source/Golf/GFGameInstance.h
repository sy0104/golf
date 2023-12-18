// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameInfo.h"
#include "Engine/GameInstance.h"
#include "GFGameInstance.generated.h"

UCLASS()
class GOLF_API UGFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGFGameInstance();
	~UGFGameInstance();

public:
	virtual void Init();
};
