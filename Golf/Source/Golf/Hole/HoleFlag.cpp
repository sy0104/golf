#include "HoleFlag.h"
#include "../GFGameModeBase.h"
#include "../Ball/Ball.h"

AHoleFlag::AHoleFlag()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	const FString& path = TEXT("/Script/Engine.StaticMesh'/Game/Assets/World_Flags/Meshes/SM_Flag_Holder_13.SM_Flag_Holder_13'");
	UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path);

	if (IsValid(mesh))
	{
		mStaticMesh->SetStaticMesh(mesh);
		mStaticMesh->SetRelativeScale3D(FVector(0.5, 0.5, 1.5));
	}

	mStaticMesh->SetCollisionProfileName(TEXT("IgnoreBall"));
	mStaticMesh->SetCollisionObjectType(ECC_GameTraceChannel4);

	// Widget Component
	mWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetCom"));
	mWidgetComponent->SetupAttachment(mStaticMesh);
}

void AHoleFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHoleFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGFGameModeBase* GameMode = Cast<AGFGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (IsValid(GameMode))
	{
		ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (IsValid(Ball))
		{
			FVector dir = Ball->GetActorLocation() - GetActorLocation();
			dir.Z = 0.f;
			dir.Normalize();

			SetActorRotation(FRotator(0.f, dir.Rotation().Yaw, 0.f));
		}
	}	
}

