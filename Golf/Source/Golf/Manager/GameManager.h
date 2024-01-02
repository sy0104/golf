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
	void Init();
	void CreatePlayers(EPlayType PlayType);
	void ClearPlayers();

public:
	void SetPlayType(EPlayType PlayType)
	{
		mPlayType = PlayType;
	}

	void AddTurn(int turn)
	{
		mTurn += turn;
	}

	void SetPlayerInfo(FPlayerInfo player, int idx)
	{
		mPlayers[idx] = player;
	}

	void SetCurPlayer(int CurPlayer)
	{
		mCurPlayer = (EPlayer)CurPlayer;
	}

	void SetCurPlayerEnd()
	{
		mPlayers[(int)mCurPlayer].TurnEnd = true;
	}

	void SetCurPlayerLeftDis(float dis)
	{
		mPlayers[(int)mCurPlayer].LeftDistance = dis;
	}

public:
	TArray<FPlayerInfo> GetPlayers() const
	{
		return mPlayers;
	}

	FPlayerInfo GetPlayerInfo(EPlayer player) const
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

	FPlayerInfo GetCurPlayerInfo() const
	{
		return mPlayers[(int)mCurPlayer];
	}

	FPlayerInfo GetNextPlayerInfo() const
	{
		if (mCurPlayer == EPlayer::Player1)
			return mPlayers[(int)EPlayer::Player2];
		else
			return mPlayers[(int)EPlayer::Player1];
	}

	EPlayer GetNextPlayer() const
	{
		if (mCurPlayer == EPlayer::Player1)
			return EPlayer::Player2;
		else
			return EPlayer::Player1;
	}

	bool GetIsCreatePlayer() const
	{
		return mIsCreatePlayer;
	}

	bool GetIsCurPlayerEnd() const
	{
		return mPlayers[(int)mCurPlayer].TurnEnd;
	}

	float GetPlayerLeftDis(EPlayer player) const
	{
		return mPlayers[(int)player].LeftDistance;
	}

	int GetTurn() const
	{
		return mTurn;
	}

private:
	EPlayType				mPlayType;
	TArray<FPlayerInfo>		mPlayers;
	EPlayer					mCurPlayer;

	int		mTurn;
	bool	mIsCreatePlayer;
};
