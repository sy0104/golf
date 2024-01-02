#include "MenuBase.h"
#include "../GFGameInstance.h"
#include "../Manager/GameManager.h"
#include "../Ball/Ball.h"

void UMenuBase::NativeConstruct()
{
	Super::NativeConstruct();

	mHomeButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("HomeButton"))));
	mExitButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("ExitButton"))));

	mHomeButton->OnClicked.AddDynamic(this, &UMenuBase::OnHomeButtonClicked);
	mExitButton->OnClicked.AddDynamic(this, &UMenuBase::OnExitButtonClicked);
}

void UMenuBase::OnHomeButtonClicked()
{
	// 플레이어 초기화
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
		GameManager->ClearPlayers();

	// 공 초기화
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->Init(true);

	// Lobby로 돌아가기
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}

void UMenuBase::OnExitButtonClicked()
{
	PrintViewport(1.f, FColor::Red, TEXT("Exit"));

}
