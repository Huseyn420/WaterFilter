// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "WaterMolecules.generated.h"

UCLASS()
class WATERFILTER_API AWaterMolecules : public AActor
{
	GENERATED_BODY()

public:
	AWaterMolecules();

	UPROPERTY(EditAnyWhere)
		UInstancedStaticMeshComponent* WaterMoleculesMesh;

	int SphereDiameter = 80;
	int SphereDiameterTolerance = 5;

	int ParticlesHeight = 600;
	int OuterCylinderDiameter = 300;

	bool FreezeStates = true;

private:
	USceneComponent* Scene;
	TArray< USphereComponent*> CollisionComponents;

	float ScaleFactor = 100.0f;

public:
	virtual void Tick(float DeltaTime) override;

	void CreateCloudMolecules();
	void FreezeButtonPressed();

private:
	void SetupWaterMoleculesMesh();
	void CreateCollisionComponent(FTransform Transform);
};