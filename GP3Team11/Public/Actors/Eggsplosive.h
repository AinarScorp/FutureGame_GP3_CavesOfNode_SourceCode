#pragma once
//Made by Udit Shroff and Einar Hallik

#include "CoreMinimal.h"
#include "ActorComponents/PathTracer.h"
#include "Interface/Hittable.h"
#include "Interface/Stabbable.h"
#include "Eggsplosive.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UTP_WeaponComponent;

UCLASS()
class GP3TEAM11_API AEggsplosive : public AActor,public IHittable,public IStabbable
{
	GENERATED_BODY()
public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eggsplosive|SetMeUp|Parameters")
	float Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eggsplosive|SetMeUp|Parameters")
	float ExplosionRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eggsplosive|SetMeUp|Parameters")
	float ExplodeTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eggsplosive|SetMeUp|Parameters")
	bool bIsCooking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Eggsplosive|SetMeUp|Parameters")
	bool bHasEggsploded;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Eggsplosive|Stab|Tunning", meta = (AllowPrivateAccess = "true")) // add , EditCondition = "IsStabbableWeapon",EditConditionHides
	bool IsLockAbleStab = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Eggsplosive|Stab|Tunning", meta = (AllowPrivateAccess = "true"))
	bool IsOneHitStab = false;
	UPROPERTY(EditAnywhere, Category = "Eggsplosive|Setup")
	TEnumAsByte<ETraceTypeQuery> ExplodeTraceChannel = TraceTypeQuery6;
	UPROPERTY(EditAnywhere, Category = "Eggsplosive|Debugging")
	TEnumAsByte<EDrawDebugTrace::Type> SphereTraceDebug = EDrawDebugTrace::None;
private:
	FTimerHandle ExplodeTimerHandle;
	UPROPERTY()
	UTP_WeaponComponent* AttachedWeapon;
	
	//Constructor
public:
	AEggsplosive();
	
	//Functions
private:
	bool ExecuteExplodeInterface(UObject* PotentialExplodableObject) const;

public:
	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Eggsplode();

	UFUNCTION(BlueprintNativeEvent)
	void Premature_Eggsplode();

	UFUNCTION(BlueprintNativeEvent)
	void CookEgg();

	UFUNCTION(BlueprintNativeEvent)
	void AttachToWeapon(UTP_WeaponComponent* Weapon);
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintNativeEvent)
	void ThrowEgg(FVector Direction, float Speed);

	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser,const FHitResult& Hit) override;
	virtual void ApplyStabbing_Implementation(UTP_WeaponComponent* StabWeapon) override;
	virtual bool IsLockableStab_Implementation() override;
	virtual bool IsOneTimeStab_Implementation() override;
};
