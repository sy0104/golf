#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "BallController.generated.h"

UCLASS()
class GOLF_API ABallController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABallController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent();
};
