// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "BaseWeapon.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class LEADERSHIPGAME_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Internal setter for EquippedIndex. Used by AShooterCharacter::EquipWeapon
	void SetEquippedIndex(int NextIndex);

	// Find index of weapon in Equipment array
	int FindEquipmentIndex(ABaseWeapon* Weapon);

	int EquippedIndex;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABaseWeapon*> Equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAiming;

	UFUNCTION(BlueprintCallable, Category = "ShooterCharacter|Weapon")
	virtual void AddWeapon(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "ShooterCharacter|Weapon")
	virtual void RemoveWeapon(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "ShooterCharacter|Weapon")
	virtual ABaseWeapon* GetEquippedWeapon();

	/** Equip weapon from inventory */
	UFUNCTION(BlueprintCallable, Category = "ShooterCharacter|Weapon")
	virtual void Equip(ABaseWeapon* Weapon);

	/** Equip weapon from inventory, by index */
	UFUNCTION(BlueprintCallable, Category = "ShooterCharacter|Weapon")
	virtual void EquipByIndex(int WeaponIndex);

	virtual FName GetWeaponAttachPoint();

	virtual UMeshComponent* GetSpecifcPawnMesh();
};
