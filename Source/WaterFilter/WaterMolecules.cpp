// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterMolecules.h"

AWaterMolecules::AWaterMolecules()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SetupWaterMoleculesMesh();
}

void AWaterMolecules::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	int CollisionComponentsCount = CollisionComponents.Num();
	for (int i = 0; i < CollisionComponentsCount; i++)
	{
		FTransform transform = CollisionComponents[i]->GetRelativeTransform();
		WaterMoleculesMesh->UpdateInstanceTransform(
			i, transform, true, i == CollisionComponentsCount - 1, true
		);
	}
}

void AWaterMolecules::CreateCloudMolecules()
{
	int CountX = OuterCylinderDiameter / (SphereDiameter + SphereDiameterTolerance);
	int CountY = CountX;
	int CountZ = ParticlesHeight / (SphereDiameter + SphereDiameterTolerance);

	FVector StartLocation = FVector(
		(SphereDiameterTolerance - CountX * SphereDiameter) / 2.0f,
		(SphereDiameterTolerance - CountY * SphereDiameter) / 2.0f,
		0.0f
	);

	for (int i = 0; i < CountX; i++)
	{
		for (int j = 0; j < CountY; j++)
		{
			for (int k = 0; k < CountZ; k++)
			{
				FRotator Rotation = FRotator::ZeroRotator;
				FVector Location = StartLocation + FVector(
					i * (SphereDiameter + FMath::RandRange(0, SphereDiameterTolerance / 2)),
					j * (SphereDiameter + FMath::RandRange(0, SphereDiameterTolerance / 2)),
					k * (SphereDiameter + FMath::RandRange(0, SphereDiameterTolerance / 2))
				);
				FVector Scale = FVector(SphereDiameter / ScaleFactor);
				FTransform Transform = FTransform(Rotation, Location, Scale);
				WaterMoleculesMesh->AddInstance(Transform);
				CreateCollisionComponent(Transform);
			}
		}
	}
}

void AWaterMolecules::CreateCollisionComponent(FTransform Transform)
{
	USphereComponent* SphereComponent = NewObject<USphereComponent>(this);

	SphereComponent->InitSphereRadius(SphereDiameter / 2.0f);
	SphereComponent->SetCollisionProfileName("BlockAll");
	SphereComponent->SetRelativeTransform(Transform);
	SphereComponent->SetEnableGravity(1);
	SphereComponent->SetSimulatePhysics(1);

	SphereComponent->SetupAttachment(RootComponent);
	CollisionComponents.Add(SphereComponent);
	SphereComponent->RegisterComponent();
}

void AWaterMolecules::SetupWaterMoleculesMesh()
{
	WaterMoleculesMesh =
		CreateDefaultSubobject<UInstancedStaticMeshComponent>("WaterMoleculesMesh");
	WaterMoleculesMesh->SetupAttachment(RootComponent);

	WaterMoleculesMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")
	);
	if (MeshAsset.Succeeded()) 
	{
		WaterMoleculesMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AssetMaterial(
		TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'")
	);
	if (AssetMaterial.Succeeded())
	{
		WaterMoleculesMesh->SetMaterial(0, AssetMaterial.Object);
	}
}