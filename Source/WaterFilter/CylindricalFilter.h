// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CylindricalFilter.generated.h"

UCLASS()
class WATERFILTER_API ACylindricalFilter : public AActor
{
	GENERATED_BODY()
	
public:
	ACylindricalFilter();

	UPROPERTY(EditAnyWhere)
		UStaticMeshComponent* InnerCylinderMesh;
	UPROPERTY(EditAnyWhere)
		UInstancedStaticMeshComponent* OuterCylinderMesh;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int OuterCylinderRadius = 300;
	UPROPERTY(EditAnywhere, Category = "Parameters")
		int InnerCylinderRadius = 150;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int CylindersDelta = 100;
	UPROPERTY(EditAnywhere, Category = "Parameters")
		int CylinderHeight = 600;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int SphereDiameter = 80;
	UPROPERTY(EditAnywhere, Category = "Parameters")
		int SphereDiameterTolerance = 5;

	UPROPERTY(EditAnywhere, Category = "Parameters")
		int ParticlesHeight = 600;

private:
	USceneComponent* Scene;

	const float ScaleFactor = 100.0f;

protected:
	virtual void BeginPlay() override;

private:
	void DrawOuterCylinder();
	void SettingParametersInnerCylinder();
	void CreateWaterMolecules();
	void SetupOuterCylinderMesh();
	void SetupInnerCylinderMesh();
};
