// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/InteractionTrace.h"
#include "Interface/Interactable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/EngineBaseTypes.h"


// Sets default values for this component's properties
UInteractionTrace::UInteractionTrace()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//PrimaryComponentTick.TickInterval = 1;
	

	// ...
}


// Called when the game starts
void UInteractionTrace::BeginPlay()
{
	Super::BeginPlay();

	// i want this line to be in the constructor but it dont work for some reason
	//PrimaryComponentTick.TickInterval = .3;
	// ...
	cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}


// Called every frame
void UInteractionTrace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Trace();
	// ...
}

void UInteractionTrace::PerformInteraction()
{
	//check if you are focusing
	if (!isFocusing) return;

	//if actor is null return
	if (!IsValid(focusedActor)) return;

	// dont know why i need to check if the actor implements the interface...
	// but the editor seems to crash when calling the interface in an actor that does not implement it
	if (focusedActor->Implements<UInteractable>()) {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("interface"));
		IInteractable::Execute_Interact(focusedActor);
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("pressed"));
	
}

void UInteractionTrace::Trace()
{
	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	if (GetWorld()->LineTraceSingleByChannel
	(
		hit,
		cameraManager->GetCameraLocation(), // start
		cameraManager->GetCameraLocation() + (cameraManager->GetActorForwardVector() * interactionLength), // end
		ECollisionChannel::ECC_Visibility,
		params,
		FCollisionResponseParams())
	)
	{
		// return if not looking at new actor
		if (focusedActor != hit.GetActor() || !isFocusing) {

			// a new actor has been looked at!
			
			// first unfocus the old acotr you used to look at
			if (IsValid(focusedActor)) {
				if (focusedActor->Implements<UInteractable>()) {
					IInteractable::Execute_EndHover(focusedActor);

				}
			}

			// set new focused actor
			focusedActor = hit.GetActor();

			// call the begin focus interface
			if (focusedActor->Implements<UInteractable>()) {
				IInteractable::Execute_BeginHover(focusedActor);
			}

			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, *focusedActor->GetName());


			isFocusing = true;
		}
	}
	else
	{
		if (isFocusing) {
			isFocusing = false;

			if (IsValid(focusedActor)) {
				if (focusedActor->Implements<UInteractable>()) {
					IInteractable::Execute_EndHover(focusedActor);

				}
			}
		}
	}

	
}
