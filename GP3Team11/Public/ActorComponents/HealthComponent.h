#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDieSignature);

UCLASS( ClassGroup=(Custom),Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3TEAM11_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	int MaxHealth = 100;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	int CurrentHealth = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Health")
	bool IsDead = false;

public:
	UHealthComponent();
	
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Health")
	void TakeDamage(int Damage);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Health")
	void Heal(int HealAmount);

	UFUNCTION(BlueprintCallable)
	float GetHealthPercent() const;
	
	UPROPERTY(BlueprintAssignable)
	FOnDieSignature OnDie;
};
