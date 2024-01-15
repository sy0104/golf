#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "ConcedeTrigger.generated.h"

UCLASS()
class GOLF_API AConcedeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	AConcedeTrigger();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void TriggerBegin(const FHitResult& SweepResult);
	virtual void TriggerEnd();

public:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool		mIsCollide;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USceneComponent*		mRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	USphereComponent*		mSphere;
};
