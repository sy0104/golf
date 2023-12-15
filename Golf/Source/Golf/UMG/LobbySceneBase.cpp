#include "LobbySceneBase.h"

void ULobbySceneBase::NativeConstruct()
{
	Super::NativeConstruct();

	mStartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartButton"))));

	mStartButton->OnClicked.AddDynamic(this, &ULobbySceneBase::OnStartButtonClicked);
}

void ULobbySceneBase::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Main"));
}
