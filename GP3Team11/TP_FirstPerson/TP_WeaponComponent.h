//Made By Udit Shroff and Einar Hallik
#pragma once
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class ATP_FirstPersonCharacter;
class AEggsplosive;

UENUM(BlueprintType)
enum class EFireMode : uint8 {
	Single       UMETA(DisplayName="Single"),
	FullAuto       UMETA(DisplayName="Full Auto"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Projectile       UMETA(DisplayName="Projectile"),
	RayCast       UMETA(DisplayName="Raycast"),
};

UENUM(BlueprintType)
enum EHasEgg 
{
	Egg,
	NoEgg
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3TEAM11_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Parameters")
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Parameters")
	int ClipSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	EFireMode FireMode;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	float RateOfFire;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category="WeaponComponent|SetMeUp|Parameters", meta=(EditCondition="WeaponType != EWeaponType::Projectile",EditConditionHides))
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	bool ApplyForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	bool hasEgg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	float EggForce = 5000;
	
	//NOT NEEDED PRB
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	float TimeBetweenStabTick;
	//NOT NEEDED PRB

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	float CooldownAfterStab;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	float CooldownAfterThrowingEgg;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters", meta=(EditCondition = "!IsStabbableWeapon",EditConditionHides))
	float ReloadTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters", meta=(EditCondition = "!IsStabbableWeapon",EditConditionHides))
	bool AutoReload;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	float MaxSpread;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Parameters")
	UCurveFloat* SpreadCurve;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Gameplay")
	FVector MuzzleOffset;
	
	UPROPERTY(EditDefaultsOnly, Category="WeaponComponent|SetMeUp|Projectile", meta=(EditCondition="WeaponType == EWeaponType::Projectile",EditConditionHides))
	TSubclassOf<class ATP_FirstPersonProjectile> ProjectileClass;



#pragma region Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Input", meta=(AllowPrivateAccess = "true"))
    class UInputMappingContext* FireMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Input", meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Input", meta=(AllowPrivateAccess = "true",EditCondition = "IsStabbableWeapon",EditConditionHides))
	UInputAction* StabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|SetMeUp|Input", meta=(AllowPrivateAccess = "true",EditCondition = "!IsStabbableWeapon",EditConditionHides))
	UInputAction* ReloadAction;
#pragma endregion

#pragma region Anmations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponent|SetMeUp|Animations")
    UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponComponent|SetMeUp|Animations", meta=(EditCondition = "!IsStabbableWeapon",EditConditionHides))
	UAnimMontage* ReloadAnimation;
#pragma endregion

#pragma region Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Sounds")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Sounds")
	USoundBase* StabSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Sounds", meta=(EditCondition = "IsStabbableWeapon",EditConditionHides))
	USoundBase* UnstabSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WeaponComponent|SetMeUp|Sounds", meta=(EditCondition = "!IsStabbableWeapon",EditConditionHides))
	USoundBase* ReloadSound;
#pragma endregion 

private:
	/** The Character holding this weapon*/
	UPROPERTY()
	ATP_FirstPersonCharacter* Character;
	FTimerHandle FireTimerHandle;
	float TimeBetweenShots;
	float FiringTime;

	UPROPERTY()
	AEggsplosive* AttachedEgg;
public:
	//Constructor
	/** Sets default values for this component's properties */
	UTP_WeaponComponent();
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//FUNCTIONS
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(ATP_FirstPersonCharacter* TargetCharacter);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="WeaponComponent|Fire")
	void BeginAutoFire();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="WeaponComponent|Fire")
	void Fire();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="WeaponComponent|Fire")
	void EndAutoFire();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="WeaponComponent|Weapon")
	void ActivateWeapon();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="WeaponComponent|Weapon")
	bool DeactivateWeapon();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "WeaponComponent|Fire")
	void Reload();
	

	
	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	void ThrowEgg(TEnumAsByte<EHasEgg>& Branches);

	UFUNCTION(BlueprintCallable)
	float GetAmmoPercent() const;


	
	UFUNCTION(BlueprintImplementableEvent)
    void OnBeginFire();

	UFUNCTION(BlueprintImplementableEvent)
    void OnFire();

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndFire();


	void SetActionBindings();
	
	UFUNCTION(BlueprintCallable, Category = "WeaponComponent|Ammo")
	void ModifyCurrentAmmo(float AmmoDelta);
	

protected:
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool IsActive;

	bool IsFiring;

#pragma region PartOf Shooting mechanic
	//Einar Part
private:
	UPROPERTY(EditAnywhere, Category = "WeaponComponent|Shooting|Settings")
	TEnumAsByte<ETraceTypeQuery> ShootTraceChannel = TraceTypeQuery7;
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HitWall(FHitResult HitResult);
private:
	
	void FireRaycast();
	void FireProjectile();
	bool ExecuteGetHit(UObject* PotentialHittableObject, FHitResult& HitResult);
#pragma endregion 
#pragma region Stab mechanics
	//Einar Part
private:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category="WeaponComponent|Stab|Settings", meta = (AllowPrivateAccess = "true"))
	bool IsStabbableWeapon;
	
	UPROPERTY(EditAnywhere, Category = "WeaponComponent|Stab|Settings")
	TEnumAsByte<ETraceTypeQuery> StabTraceChannel = TraceTypeQuery5;
	
#pragma region Tunning
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|Stab|Tunning",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides, AllowPrivateAccess = "true"))
	int StabDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponComponent|Stab|Tunning",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides, AllowPrivateAccess = "true"))
	int StabRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|Stab|Tunning",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides, AllowPrivateAccess = "true"))
	int AmmoRefillOnAliveEnemy;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponComponent|Stab|Tunning",meta=(EditCondition = "IsStabbableWeapon",EditConditionHides, AllowPrivateAccess = "true"))
	int AmmoRefillOnDeadEnemy;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="WeaponComponent|Stab|Tunning", meta = (AllowPrivateAccess = "true"))
	float StabRaycastSphereRadius = 50;
#pragma endregion
	
#pragma region VisibleForDebugging
	UPROPERTY(VisibleAnywhere, Category="WeaponComponent|Stab|VisibleForDebugging")
	bool StartedPerformingStab = false;
#pragma endregion
	
#pragma region Debugging
	
	UPROPERTY(EditAnywhere, Category = "WeaponComponent|Stab|Debugging")
	bool DrawStabTrace = false;

#pragma endregion
	
	UPROPERTY(BlueprintReadOnly, Category="WeaponComponent|Stab", meta = (AllowPrivateAccess = "true"))
	AActor* LockedStab;
	
	bool CanStab = true;

public:
	
private:
	bool ShootStabRaycast(FHitResult& OutHitResult) const;
	bool CanStabThisTarget(const AActor* Target);
	void ExecuteStabbableInterface(AActor* PotentialStabbalbeActor);
	
public:

#pragma region Getters & Setters
	UFUNCTION(BlueprintCallable, Category= "WeaponComponent|Stab")
	bool GetStartedPerformingStab() const {return StartedPerformingStab;}
	UFUNCTION(BlueprintCallable, Category= "WeaponComponent|Stab")
	void SetStartedPerformingStab(bool Value) { StartedPerformingStab = Value;}
	UFUNCTION(BlueprintCallable, Category = "WeaponComponent|Ammo")
	int GetAmmoRefillOnAliveEnemy() const {return AmmoRefillOnAliveEnemy;}
	UFUNCTION(BlueprintCallable, Category = "WeaponComponent|Ammo")
	int GetAmmoRefillOnDeadEnemy()const {return AmmoRefillOnDeadEnemy;}
	
	int GetStabDamage() const {return StabDamage;}
	ATP_FirstPersonCharacter* GetCharacter() const {return Character;}

#pragma endregion
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBeginStab();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "WeaponComponent|Stab")
	void PerformStab();

	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEndStab();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "WeaponComponent|Stab")
	void FinishStab();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "WeaponComponent|Stab")
	void StabButtonReleased();

	UFUNCTION(BlueprintCallable, Category = "WeaponComponent|Stab")
	void AttachEgg(class AEggsplosive* Egg);
#pragma endregion 

};


#pragma region CommentedOut


// UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Weapon")
// void SingleFire();

// UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category="Weapon")
// void AutoFire();

	


//LearnLater TODO Learn later
// UPROPERTY(BlueprintReadOnly, Category="WeaponComponent|Stab")
// TScriptInterface<class IStabbable> LockedStab;
//
// UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Weapon")
// void Stab();
//
// UFUNCTION(BlueprintCallable, Category = "Weapon")
// void Stabbing();
//
// UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Weapon")
// void Unstab();
#pragma endregion 