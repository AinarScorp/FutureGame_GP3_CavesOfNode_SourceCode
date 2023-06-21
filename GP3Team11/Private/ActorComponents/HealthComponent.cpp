#include "ActorComponents/HealthComponent.h"

#include "Components/CapsuleComponent.h"

UHealthComponent::UHealthComponent(){}

void UHealthComponent::TakeDamage_Implementation(int Damage)
{
	if(IsDead) return;
	CurrentHealth -= Damage;
	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		IsDead = true;
		OnDie.Broadcast();
	}
}

void UHealthComponent::Heal_Implementation(int HealAmount)
{
	CurrentHealth += HealAmount;
	if(CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

float UHealthComponent::GetHealthPercent() const
{
	return (float)CurrentHealth / (float)MaxHealth;
}


