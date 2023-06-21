#include "ActorComponents/WeaponSwitcher.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GP3Team11/TP_FirstPerson/TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"

UWeaponSwitcher::UWeaponSwitcher() {}

void UWeaponSwitcher::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, this, &UWeaponSwitcher::NextWeapon);
			EnhancedInputComponent->BindAction(HolsterWeaponAction, ETriggerEvent::Started, this, &UWeaponSwitcher::HolsterWeapon);
		}
	}
}


void UWeaponSwitcher::AddWeapon(UTP_WeaponComponent* Weapon)
{
	if(Weapons.Num() == 0 || WeaponsHolstered)
	{
		Weapon->ActivateWeapon();
		CurrentWeaponIndex = Weapons.Num();
		WeaponsHolstered = false;
	}
	else
		Weapon->DeactivateWeapon();

	Weapons.Add(Weapon);
}

void UWeaponSwitcher::NextWeapon()
{
	if(!bCanSwitchWeapons)
		return;
	if(WeaponsHolstered)
	{
		CurrentWeapon()->ActivateWeapon();
		WeaponsHolstered = false;
	}
	else if(Weapons.Num() <= 1)
		return;
	else if(CurrentWeapon()->DeactivateWeapon())
	{
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
		CurrentWeapon()->ActivateWeapon();
	}
}

float UWeaponSwitcher::GetAmmoPercent() const
{
	if(Weapons.Num() == 0 || WeaponsHolstered)
		return 0;
	return CurrentWeapon()->GetAmmoPercent();
}

void UWeaponSwitcher::HolsterWeapon()
{
	if(Weapons.Num() == 0)
		return;
	if(WeaponsHolstered)
	{
		CurrentWeapon()->ActivateWeapon();
		WeaponsHolstered = false;
	}
	else if(CurrentWeapon()->DeactivateWeapon())
	{
		WeaponsHolstered = true;
	}
}
