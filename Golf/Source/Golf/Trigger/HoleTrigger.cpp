#include "HoleTrigger.h"
#include "../Ball/Ball.h"

AHoleTrigger::AHoleTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	// Box Component
	mBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	mBox->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);
	mBox->SetCollisionProfileName(TEXT("BallTrigger"));

	// Delegate
	mBox->OnComponentBeginOverlap.AddDynamic(this, &AHoleTrigger::BeginOverlap);
	mBox->OnComponentEndOverlap.AddDynamic(this, &AHoleTrigger::EndOverlap);
}

void AHoleTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHoleTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHoleTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	PrintViewport(5.f, FColor::Red, TEXT("Hole Trigger Begin"));

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetInHole(true);
}

void AHoleTrigger::TriggerEnd()
{
	PrintViewport(5.f, FColor::Red, TEXT("Hole Trigger Begin"));

	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (IsValid(Ball))
		Ball->SetInHole(false);
}

void AHoleTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerBegin(SweepResult);
}

void AHoleTrigger::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TriggerEnd();
}

