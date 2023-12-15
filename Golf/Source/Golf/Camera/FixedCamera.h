
#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "FixedCamera.generated.h"

UCLASS()
class GOLF_API AFixedCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AFixedCamera();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent* mRoot;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	//ACameraActor*	mFixedCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component, meta = (AllowPrivateAccess = true))
	UCameraComponent* mFixedCamera;

public:
	UCameraComponent* GetFixedCamera()
	{
		return mFixedCamera;
	}
};
