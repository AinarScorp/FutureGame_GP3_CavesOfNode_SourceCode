#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Components/ActorComponent.h"
#include "WeaponSwitcher.generated.h"

//Forward Declarations
class UTP_WeaponComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponSwitcher : public UActorComponent
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool WeaponsHolstered;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanSwitchWeapons = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CurrentWeaponIndex;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<UTP_WeaponComponent*> Weapons;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputMappingContext* InputContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputAction* NextWeaponAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UInputAction* HolsterWeaponAction;
	
	//Constructors
	public:	
	// Sets default values for this component's properties
	UWeaponSwitcher();
	
	//Functions
	public:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	UTP_WeaponComponent* CurrentWeapon() const { return Weapons[CurrentWeaponIndex];}

	UFUNCTION(BlueprintCallable)
	void AddWeapon(UTP_WeaponComponent* Weapon);
	
	UFUNCTION(BlueprintCallable)
	void NextWeapon();

	UFUNCTION(BlueprintCallable)
	void HolsterWeapon();
	
	UFUNCTION(BlueprintCallable)
	float GetAmmoPercent() const;

};
