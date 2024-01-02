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
	// �÷��̾� �ʱ�ȭ
	UGFGameInstance* GameInst = GetWorld()->GetGameInstance<UGFGameInstance>();
	UGameManager* GameManager = GameInst->GetSubsystem<UGameManager>();

	if (IsValid(GameManager))
		GameManager->ClearPlayers();

	// �� �ʱ�ȭ
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->Init(true);

	// Lobby�� ���ư���
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}

void UMenuBase::OnExitButtonClicked()
{
	PrintViewport(1.f, FColor::Red, TEXT("Exit"));

}
