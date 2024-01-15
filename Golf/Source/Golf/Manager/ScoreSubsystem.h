#pragma once

#include "../GameInfo.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ScoreSubsystem.generated.h"

UCLASS()
class GOLF_API UScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UScoreSubsystem();
	~UScoreSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	const FString GetScoreText(int score);

};
