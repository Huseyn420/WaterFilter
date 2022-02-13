// Fill out your copyright notice in the Description page of Project Settings.


#include "CylindricalFilter.h"
#include "WaterMolecules.h"

ACylindricalFilter::ACylindricalFilter()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SetupOuterCylinderMesh();
	SetupInnerCylinderMesh();
}

void ACylindricalFilter::BeginPlay()
{
	Super::BeginPlay();
	
	DrawOuterCylinder();
	SettingParametersInnerCylinder();
	CreateWaterMolecules();
}

void ACylindricalFilter::DrawOuterCylinder() 
{
	int NumberOfSides = OuterCylinderRadius * 5;
	float CylinderWidth = 2.0f * OuterCylinderRadius * FMath::Tan(PI / NumberOfSides);
	FVector Scale = FVector(CylinderHeight / ScaleFactor, CylinderWidth / ScaleFactor, 1.0f);

	for (int i = 0; i < NumberOfSides; i++)
	{
		FVector Direction = FVector(
			OuterCylinderRadius * FMath::Sin(i * PI * 2.0f / NumberOfSides),
			OuterCylinderRadius * FMath::Cos(i * PI * 2.0f / NumberOfSides),
			0
		);
		FVector Location = Direction + FVector(0, 0, CylinderHeight / 2.0f);
		FRotator Rotation = Direction.Rotation() + FRotator(90, 0, 0);
		OuterCylinderMesh->AddInstance(FTransform(Rotation, Location, Scale));
	}
}

void ACylindricalFilter::SettingParametersInnerCylinder()
{
	float CylinderDiameter = 2.0f * InnerCylinderRadius / ScaleFactor;
	FVector Location = FVector(0, 0, CylinderHeight / 2.0f + CylindersDelta);
	FVector Scale = FVector(CylinderDiameter, CylinderDiameter, CylinderHeight / ScaleFactor);
	InnerCylinderMesh->SetRelativeTransform(
		FTransform(FRotator::ZeroRotator, Location, Scale)
	);
}

void ACylindricalFilter::CreateWaterMolecules()
{
	FVector Location = 
		GetActorLocation() + FVector(0, 0, (CylinderHeight + CylindersDelta) * 2.0f);
	AWaterMolecules* WaterMolecules = 
		GetWorld()->SpawnActor<AWaterMolecules>(Location, GetActorRotation());

	WaterMolecules->SphereDiameter = SphereDiameter;
	WaterMolecules->SphereDiameterTolerance = SphereDiameterTolerance;
	WaterMolecules->ParticlesHeight = ParticlesHeight;
	WaterMolecules->OuterCylinderDiameter = 2.0f * OuterCylinderRadius;

	WaterMolecules->CreateCloudMolecules();
}

void ACylindricalFilter::SetupOuterCylinderMesh()
{
	OuterCylinderMesh =
		CreateDefaultSubobject<UInstancedStaticMeshComponent>("OuterCylinderMesh");
	OuterCylinderMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")
	);
	if (MeshAsset.Succeeded()) {
		OuterCylinderMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AssetMaterial(
		TEXT("Material'/Engine/MapTemplates/Materials/BasicAsset01.BasicAsset01'")
	);
	if (AssetMaterial.Succeeded())
	{
		OuterCylinderMesh->SetMaterial(0, AssetMaterial.Object);
	}
}

void ACylindricalFilter::SetupInnerCylinderMesh()
{
	InnerCylinderMesh =
		CreateDefaultSubobject<UStaticMeshComponent>("InnerCylinderMesh");
	InnerCylinderMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'")
	);
	if (MeshAsset.Succeeded())
	{
		InnerCylinderMesh->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> AssetMaterial(
		TEXT("Material'/Engine/MapTemplates/Sky/M_Procedural_Sky_MASTER022.M_Procedural_Sky_MASTER022'")
	);
	if (AssetMaterial.Succeeded())
	{
		InnerCylinderMesh->SetMaterial(0, AssetMaterial.Object);
	}
}