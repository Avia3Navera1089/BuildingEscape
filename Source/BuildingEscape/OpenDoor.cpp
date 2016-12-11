// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
}




// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );


	//poll the trigger volume	
	//if ActorThatOpens is in the volume	
	if (GetTotalMassOfActorsOnPLate() > TriggerMass)
	{
		OnOpen.Broadcast();
	}
	//Check if its time to close the door
	else //(GetTotalMassOfActorsOnPLate() < TriggerMass)
	{
		OnClose.Broadcast();
	}
	
}

float UOpenDoor::GetTotalMassOfActorsOnPLate()
{
	float TotalMass = 0.f;

	//find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	//gets the mass of all the objects on the pressure plate
	for (const auto& Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}
	//iterate through them adding their masses

	return TotalMass;
}