#include "TP_WeaponComponent.h"
#include "TP_FirstPersonCharacter.h"
#include "TP_FirstPersonProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "NiagaraSystem.h"
#include "Engine/LocalPlayer.h"
#include "Actors/Eggsplosive.h"
#include "Camera/CameraComponent.h"
#include "Interface/Hittable.h"
#include "Interface/Stabbable.h"
#include "Kismet/KismetMathLibrary.h"

void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsStabbableWeapon && StartedPerformingStab)
	{
		PerformStab();
	}
}

UTP_WeaponComponent::UTP_WeaponComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::AttachWeapon(ATP_FirstPersonCharacter* TargetCharacter)
{
	TimeBetweenShots = RateOfFire == 0 ? 0 : 1 / RateOfFire;

	Character = TargetCharacter;
	if (Character == nullptr)
		return;

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);
	Character->WeaponSwitching->AddWeapon(this);

	SetActionBindings();
}

void UTP_WeaponComponent::SetActionBindings()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			if (FireMode == EFireMode::Single)
			{
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::Fire);
			}
			else
			{
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::BeginAutoFire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndAutoFire);
			}

			if (IsStabbableWeapon)
			{
				EnhancedInputComponent->BindAction(StabAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::OnBeginStab);
				EnhancedInputComponent->BindAction(StabAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::StabButtonReleased);
			}
			else
			{
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Reload);
			}
		}
	}
}


void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}


void UTP_WeaponComponent::BeginAutoFire_Implementation()
{
	if (!IsActive || hasEgg || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle) || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (CurrentAmmo <= 0)
	{
		return;
	}
	
	IsFiring = true;
	FiringTime = 0;
	OnBeginFire();
}


void UTP_WeaponComponent::EndAutoFire_Implementation()
{
	if (IsFiring)
	{
		IsFiring = false;
		FiringTime = 0;
		OnEndFire();
		
	}
}


void UTP_WeaponComponent::FireRaycast()
{
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	if(FireMode == EFireMode::FullAuto){
		FiringTime+=TimeBetweenShots;
	}
	
	FVector StartTraceLocation = Character->FirstPersonCameraComponent->GetComponentLocation();
	FVector Direction = Character->FirstPersonCameraComponent->GetForwardVector();
	float Spread = SpreadCurve->GetFloatValue(FiringTime) * MaxSpread;
	FVector SpreadDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(Direction, Spread);
	FVector EndTraceLocation = StartTraceLocation + SpreadDirection * Range;
	
	bool SuccesfulRaycast = UKismetSystemLibrary::LineTraceMulti
	(GetWorld(),StartTraceLocation,EndTraceLocation,
	ShootTraceChannel, false, ActorsToIgnore,
	EDrawDebugTrace::None, HitResults, true);
	
	if (!SuccesfulRaycast)
	{
		return;
	}
	for (FHitResult HitResult : HitResults)
	{
		bool HitActor = ExecuteGetHit(HitResult.GetActor(), HitResult);
		bool HitComp = ExecuteGetHit(HitResult.GetComponent(), HitResult);

		if (!HitComp && !HitActor && HitResult.bBlockingHit)
		{
			HitWall(HitResult);
		}
	}
}
bool UTP_WeaponComponent::ExecuteGetHit(UObject* PotentialHittableObject, FHitResult& HitResult)
{

	if (UKismetSystemLibrary::DoesImplementInterface(PotentialHittableObject, UHittable::StaticClass()))
	{
		IHittable::Execute_GetHit(PotentialHittableObject, Damage, Character->GetController(), Character, HitResult);
		return true;
	}
	return false;
	
}
void UTP_WeaponComponent::FireProjectile()
{
	if (!ProjectileClass)
		return;;
	UWorld* const World = GetWorld();
	if (!World)
		return;
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	// FQuat a = SpawnRotation.Quaternion();
	// a *= FRotator(1,FMath::RandRange(-Spread,Spread),FMath::RandRange(-Spread,Spread)).Quarternio;
	// Spawn the projectile at the muzzle

	ATP_FirstPersonProjectile* Projectile = World->SpawnActor<ATP_FirstPersonProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

	if (Projectile != nullptr)
	{
		// FVector FireDir = FVector(1,FMath::RandRange(-Spread,Spread),FMath::RandRange(-Spread,Spread));
		// FireDir.Normalize();
		Projectile->SetupProjectile(FVector::Zero(), Damage, ApplyForce);
	}
}

void UTP_WeaponComponent::Fire_Implementation()
{
	if (!IsFiring || !IsActive || hasEgg || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle) || Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (CurrentAmmo <= 0)
	{
		if (AutoReload)
		{
			Reload();
		}
		if (FireMode == EFireMode::FullAuto)
		{
			EndAutoFire();
		}

		return;
	}
	ModifyCurrentAmmo(-1);
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, TimeBetweenShots, false);

	// Try and fire a projectile
	switch (WeaponType)
	{
	case EWeaponType::RayCast:
		{
			FireRaycast();
			break;
		}

	case EWeaponType::Projectile:
		{
			FireProjectile();
			break;
		}
	}

	OnFire();
	Character->OnFire();

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::ModifyCurrentAmmo(float AmmoDelta)
{
	CurrentAmmo += AmmoDelta;
	CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, ClipSize);
}


#pragma region Stab functions

void UTP_WeaponComponent::PerformStab_Implementation()
{
	if (!IsActive || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle) || !CanStab)
	{
		return;
	}
	FHitResult HitResultForStab;
	const bool SuccessfulRaycast = ShootStabRaycast(HitResultForStab);

	if (!SuccessfulRaycast)
	{
		return;
	}


	if (!CanStabThisTarget(HitResultForStab.GetActor()))
	{
		return;
	}
	ExecuteStabbableInterface(HitResultForStab.GetActor());

	if (StabSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, StabSound, Character->GetActorLocation());
	}
}

void UTP_WeaponComponent::FinishStab_Implementation()
{
	CanStab = false;
	if (UnstabSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UnstabSound, Character->GetActorLocation());
	}
}

void UTP_WeaponComponent::StabButtonReleased_Implementation()
{
	LockedStab = nullptr;
	CanStab = true;
	SetStartedPerformingStab(false);
}


bool UTP_WeaponComponent::ShootStabRaycast(FHitResult& OutHitResult) const
{
	const TArray<AActor*> ActorsToIgnore;
	const FVector SphereTraceStart = Character->FirstPersonCameraComponent->GetComponentLocation();
	const FVector SphereTraceEnd = SphereTraceStart + Character->FirstPersonCameraComponent->GetForwardVector() * StabRange;

	return UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), SphereTraceStart, SphereTraceEnd, StabRaycastSphereRadius,
		StabTraceChannel, false, ActorsToIgnore,
		DrawStabTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		OutHitResult, true);
}

bool UTP_WeaponComponent::CanStabThisTarget(const AActor* Target)
{
	// if (LockedStab && LockedStab != HitResultForStab.GetActor())
	// {
	// 	return;
	// }

	return !LockedStab || LockedStab == Target;
}

void UTP_WeaponComponent::ExecuteStabbableInterface(AActor* PotentialStabbalbeActor)
{
	if (!UKismetSystemLibrary::DoesImplementInterface(PotentialStabbalbeActor, UStabbable::StaticClass()))
	{
		return;
	}

	IStabbable::Execute_ApplyStabbing(PotentialStabbalbeActor, this);

	if (IStabbable::Execute_IsOneTimeStab(PotentialStabbalbeActor))
	{
		FinishStab();
		return;
	}
	if (IStabbable::Execute_IsLockableStab(PotentialStabbalbeActor))
	{
		LockedStab = PotentialStabbalbeActor;
	}
}

void UTP_WeaponComponent::AttachEgg(AEggsplosive* Egg)
{
	hasEgg = true;
	AttachedEgg = Egg;
}


#pragma endregion


void UTP_WeaponComponent::ActivateWeapon_Implementation()
{
	IsActive = true;
	SetVisibility(true);
}

bool UTP_WeaponComponent::DeactivateWeapon_Implementation()
{
	if (hasEgg) return false;
	IsActive = false;
	SetVisibility(false);
	return true;
}


void UTP_WeaponComponent::Reload_Implementation()
{
	if (IsStabbableWeapon || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle))
		return;

	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UTP_WeaponComponent::Fire, ReloadTime, false);

	CurrentAmmo = ClipSize;
	if (ReloadSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReloadSound, Character->GetActorLocation());
	}

	if (ReloadAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(ReloadAnimation, 1.f);
		}
	}
}

float UTP_WeaponComponent::GetAmmoPercent() const
{
	return (float)CurrentAmmo / ClipSize;
}

void UTP_WeaponComponent::ThrowEgg(TEnumAsByte<EHasEgg>& Branches)
{
	if (hasEgg)
	{
		AttachedEgg->ThrowEgg(Character->GetFirstPersonCameraComponent()->GetForwardVector(), EggForce);
		hasEgg = false;
		Branches = EHasEgg::Egg;
		//GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, CooldownAfterThrowingEgg, false);
	}
	else
	{
		Branches = EHasEgg::NoEgg;
	}
}
#pragma region CommentedOut
// void UTP_WeaponComponent::Stab_Implementation()
// {
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OLD STAB!"));
//
// 	
// 	if (!IsActive || GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle) || IsStabbing || Character == nullptr || Character->GetController() == nullptr)
// 	{
// 		return;
// 	}
//
// 	// if (hasEgg)
// 	// {
// 	// 	// AttachedEgg->ThrowEgg(GetRightVector(),EggForce);
// 	// 	
// 	// }
//
// 	IsStabbing = true;
//
// 	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UTP_WeaponComponent::Stabbing, TimeBetweenStabTick, true);
//
// 	if (StabSound != nullptr)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(this, StabSound, Character->GetActorLocation());
// 	}
//
// 	OnBeginStab();
// 	// Try and play a firing animation if specified
// 	// if (StabAnimation != nullptr)
// 	// {
// 	// 	// Get the animation object for the arms mesh
// 	// 	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
// 	// 	if (AnimInstance != nullptr)
// 	// 	{
// 	// 		AnimInstance->Montage_Play(StabAnimation, 1.f);
// 	// 	}
// 	// }
// }

// void UTP_WeaponComponent::Unstab_Implementation()
// {
// 	if (!IsActive || !IsStabbing)
// 	{
// 		return;
// 	}
// 	IsStabbing = false;
// 	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, CooldownAfterStab, false);
//
//
// 	if (UnstabSound != nullptr)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(this, UnstabSound, Character->GetActorLocation());
// 	}
//
// 	OnEndStab();
//
// 	// Try and play a firing animation if specified
// 	// if (UnstabAnimation != nullptr)
// 	// {
// 	// 	// Get the animation object for the arms mesh
// 	// 	UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
// 	// 	if (AnimInstance != nullptr)
// 	// 	{
// 	// 		AnimInstance->Montage_Play(UnstabAnimation, 1.f);
// 	// 	}
// 	// }
// }

//
// void UTP_WeaponComponent::Stabbing()
// {
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OLD STAB2!"));
//
// 	FHitResult HitResult;
// 	// const FName TraceTag("DebugTrace");
// 	// GetWorld()->DebugDrawTraceTag = TraceTag;
// 	this->GetWorld()->LineTraceSingleByChannel(HitResult, Character->GetCameraLocationplusRangeDirection(0), Character->GetCameraLocationplusRangeDirection(StabRange),
// 	                                           ECollisionChannel::ECC_Visibility, FCollisionQueryParams(FName(TEXT("TraceTag")), false, Character));
// 	if (HitResult.GetActor())
// 	{
// 		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("" + HitResult.GetActor()->GetName()));
// 		if (HitResult.GetActor()->ActorHasTag(DeadEnemyTag))
// 		{
// 			CurrentAmmo += AmmoRefillOnDeadEnemy;
// 		}
// 		//EinarRemoved
// 		// else if (AEnemyBase* enemy = Cast<AEnemyBase>(HitResult.GetActor()))
// 		// {
// 		//
// 		// 	CurrentAmmo += AmmoRefillOnAliveEnemy;
// 		// 	UGameplayStatics::ApplyDamage(enemy, StabDamage, Character->GetController(), Character, UDamageType::StaticClass());
// 		// }
// 		else if (AEggsplosive* egg = Cast<AEggsplosive>(HitResult.GetActor()))
// 		{
// 			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Found AttachedEgg!"));
// 			hasEgg = true;
// 			egg->AttachToWeapon(this);
// 			AttachedEgg = egg;
// 			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Got AttachedEgg!"));
// 			Unstab();
// 		}
// 		else
// 		{
// 			Unstab();
// 		}
// 		CurrentAmmo = FMath::Clamp(CurrentAmmo, 0, ClipSize);
//
// 	}
// 	else
// 	{
// 		Unstab();
// 	}
// }

// void UTP_WeaponComponent::AutoFire_Implementation()
// {
// 	
// }
#pragma endregion
