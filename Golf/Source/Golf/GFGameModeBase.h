#pragma once

#include <Blueprint\UserWidget.h>

#include "GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "GFGameModeBase.generated.h"


UCLASS()
class GOLF_API AGFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGFGameModeBase();

public:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget>	mStartSceneUIClass;
	class UStartSceneBase*		mStartSceneUI;

	TSubclassOf<UUserWidget>	mMainHUDClass;
	class UMainHUDBase*			mMainHUD;

	TSubclassOf<UUserWidget>	mLobbySceneUIClass;
	class ULobbySceneBase*		mLobbySceneUI;

public:
	UMainHUDBase* GetMainHUD()
	{
		return mMainHUD;
	}
};
