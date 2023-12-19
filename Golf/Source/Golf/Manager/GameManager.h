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

public:
	void CreatePlayers(EPlayType PlayType);

public:
	void SetPlayType(EPlayType PlayType)
	{
		mPlayType = PlayType;
	}

	void AddTurn(int turn)
	{
		mTurn += turn;
	}

	void SetPlayer(FPlayerInfo player, int idx)
	{
		mPlayers[idx] = player;
	}

	void SetCurPlayer(int CurPlayer)
	{
		mCurPlayer = (EPlayer)CurPlayer;
	}

public:
	TArray<FPlayerInfo> GetPlayers() const
	{
		return mPlayers;
	}

	FPlayerInfo GetPlayer(EPlayer player) const
	{
		return mPlayers[(int)player];
	}

	EPlayType GetPlayType() const
	{
		return mPlayType;
	}

	EPlayer GetCurPlayer() const
	{
		return mCurPlayer;
	}

	EPlayer GetNextPlayer() const
	{
		if (mCurPlayer == EPlayer::Player1)
			return EPlayer::Player2;
		else
			return EPlayer::Player1;
	}

private:
	EPlayType				mPlayType;
	TArray<FPlayerInfo>		mPlayers;
	EPlayer					mCurPlayer;

	int		mTurn;
};
