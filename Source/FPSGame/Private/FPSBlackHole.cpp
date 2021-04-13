// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Classes\Components\SphereComponent.h"
#include "Classes\Components\StaticMeshComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	InnerSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComp->SetSphereRadius(100);
	InnerSphereComp->SetupAttachment(MeshComp);

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverLapInnerSphere);

	OuterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComp->SetSphereRadius(3000);
	OuterSphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned


void AFPSBlackHole::OverLapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	InnerSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OverLapInnerSphere);
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComs;
	OuterSphereComp->GetOverlappingComponents(OverlappingComs);

	for (int32 i = 0; i < OverlappingComs.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComs[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float Radius = OuterSphereComp->GetScaledSphereRadius();
			const float ForceStrength = -2000;
			PrimComp->AddRadialForce(GetActorLocation(), Radius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
	
}

