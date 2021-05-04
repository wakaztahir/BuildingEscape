// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "EdGraphSchema_K2_Actions.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	MoveGrabbedComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s -> Grabber : PhysicsHandle Componenet not found"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s -> Grabber : InputComponent not found"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actor = hitResult.GetActor();
	if (PhysicsHandle && actor)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(componentToGrab, NAME_None, actor->GetActorLocation(),
		                                                   FRotator(0.f, 0.f, 0.f));
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FHitResult HitResult;

	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(
			ECollisionChannel::ECC_PhysicsBody
		),
		traceParams
	);

	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	return playerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation
	);

	auto lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * Reach;

	return lineTraceEnd;
}

void UGrabber::MoveGrabbedComponent()
{
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}
