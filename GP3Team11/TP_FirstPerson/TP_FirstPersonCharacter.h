#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TP_WeaponComponent.h"
#include "ActorComponents/WeaponSwitcher.h"
#include "ActorComponents/HealthComponent.h"
#include "Settings/GameSettings.h"
#include "TP_FirstPersonCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class ATP_FirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:
#pragma region Components
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	UHealthComponent* Health;

	// UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	// UWeaponManager* PlayerWeaponManager;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	UWeaponSwitcher* WeaponSwitching;

#pragma endregion

	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	float WalkSpeed = 400;
	
	UPROPERTY(EditDefaultsOnly, Category = "CharacterSetup")
	float SprintSpeed = 600;

	/** Pawn mesh: 1st person view (arms; seen only by self) */

	/** First person camera */

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Sprint Input Action*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

private:
	UPROPERTY()
	UGameSettings* settings;
	
public:
	ATP_FirstPersonCharacter();

protected:
	virtual void BeginPlay() override;

public:
	
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	// /** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);
	//
	// /** Getter for the bool */
	// UFUNCTION(BlueprintCallable, Category = Weapon)
	// bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);
	void Unsprint(const FInputActionValue& Value);
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintImplementableEvent)
	void OnFire();
};
