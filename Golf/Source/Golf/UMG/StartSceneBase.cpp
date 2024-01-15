#include "StartSceneBase.h"
#include "../GFGameModeBase.h"

void UStartSceneBase::NativeConstruct()
{
	Super::NativeConstruct();

	mStartButton = Cast<UButton>(GetWidgetFromName(FName(TEXT("StartButton"))));

	mStartButton->OnClicked.AddDynamic(this, &UStartSceneBase::OnStartButtonClicked);
}

void UStartSceneBase::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lobby"));
}
