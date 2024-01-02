#pragma once

#include <Components\WidgetComponent.h>

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "HoleFlag.generated.h"

UCLASS()
class GOLF_API AHoleFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	AHoleFlag();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent*	mStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UWidgetComponent*		mWidgetComponent;
};
