#include "ConcedeTrigger.h"
#include "../Manager/GameManager.h"
#include "../Ball/Ball.h"

AConcedeTrigger::AConcedeTrigger()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(mRoot);
	mRoot->bVisualizeComponent = true;

	// Sphere Component
	mSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	mSphere->AttachToComponent(mRoot, FAttachmentTransformRules::KeepRelativeTransform);
	mSphere->SetCollisionProfileName(TEXT("BallTrigger"));

	// Delegate
	mSphere->OnComponentBeginOverlap.AddDynamic(this, &AConcedeTrigger::BeginOverlap);
	mSphere->OnComponentEndOverlap.AddDynamic(this, &AConcedeTrigger::EndOverlap);

	mIsCollide = false;
}

void AConcedeTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConcedeTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConcedeTrigger::TriggerBegin(const FHitResult& SweepResult)
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->SetConcede(true);
}

void AConcedeTrigger::TriggerEnd()
{
	ABall* Ball = Cast<ABall>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (IsValid(Ball))
		Ball->SetConcede(false);
}

void AConcedeTrigger::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TriggerBegin(SweepResult);
}

void AConcedeTrigger::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	TriggerEnd();
}

