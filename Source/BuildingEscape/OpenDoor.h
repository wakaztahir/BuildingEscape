// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <Actor.h>

#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor();
	void CloseDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	float GetTotalMassOfActorsOnPlate();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	//UProperties
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	
	UPROPERTY(EditAnywhere)
	float CloseAngle = 0.f;
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float CloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.f;

	//Helper Properties
	float LastDoorOpenTime = 0.f;
};
