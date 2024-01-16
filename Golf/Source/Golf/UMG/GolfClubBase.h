#pragma once

#include <Components\Button.h>
#include <Components\Image.h>

#include "../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "GolfClubBase.generated.h"


UCLASS()
class GOLF_API UGolfClubBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct();

private:
	UButton*		mDriverButton;
	UButton*		mWoodButton;
	UButton*		mIronButton;
	UButton*		mWedgeButton;
	UButton*		mPutterButton;

	UButton*		mIron5Button;
	UButton*		mIron6Button;
	UButton*		mIron7Button;
	UButton*		mIron8Button;
	UButton*		mIron9Button;

	UImage*			mDriverFrame;
	UImage*			mWoodFrame;
	UImage*			mIronFrame;
	UImage*			mWedgeFrame;
	UImage*			mPutterFrame;

	UImage*			mIron5Frame;
	UImage*			mIron6Frame;
	UImage*			mIron7Frame;
	UImage*			mIron8Frame;
	UImage*			mIron9Frame;

public:
	UFUNCTION()
	void OnDriverButtonClicked();

	UFUNCTION()
	void OnWoodButtonClicked();

	UFUNCTION()
	void OnIronButtonClicked();

	UFUNCTION()
	void OnWedgeButtonClicked();

	UFUNCTION()
	void OnPutterButtonClicked();

	UFUNCTION()
	void OnIron5ButtonClicked();

	UFUNCTION()
	void OnIron6ButtonClicked();

	UFUNCTION()
	void OnIron7ButtonClicked();

	UFUNCTION()
	void OnIron8ButtonClicked();

	UFUNCTION()
	void OnIron9ButtonClicked();

public:
	void SetPuttingClub(bool isPutting);
	void SetIronButtonVisible(bool visible);
	void ResetClubButtons();
};
