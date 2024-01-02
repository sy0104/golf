#include "GamePlayBase.h"
#include "MainHUDBase.h"
#include "../GFGameModeBase.h"
#include "../Ball/Ball.h"
#include "../Manager/GameManager.h"
#include "../GFGameInstance.h"

void UGamePlayBase::NativeConstruct()
{
	Super::NativeConstruct();

	mNextButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("NextButton"))));
	mHomeButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("HomeButton"))));
	mExitButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ExitButton"))));

	mNextButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnNextButtonClicked);
	mHomeButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnHomeButtonClicked);
	mExitButton->OnClicked.AddDynamic(this, &UGamePlayBase::OnExitButtonClicked);
}

void UGamePlayBase::OnNextButtonClicked()
{
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
		GameManager->AddTurn(1);

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		UMainHUDBase* MainHUD = GameMode->GetMainHUD();
		if (IsValid(MainHUD))
		{
			MainHUD->SetTotalScoreVisible(false);
			MainHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->Init();

	SetVisibility(ESlateVisibility::Hidden);
}

void UGamePlayBase::OnHomeButtonClicked()
{
	SetVisibility(ESlateVisibility::Hidden);


}

void UGamePlayBase::OnExitButtonClicked()
{

}
