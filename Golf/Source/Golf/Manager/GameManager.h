// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

UCLASS()
class GOLF_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UGameManager();
	~UGameManager();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

};
