// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "BaseWeapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LEADERSHIPGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Internal setter for EquippedIndex. Used by APlayerCharacter::EquipWeapon
	void SetEquippedIndex(int NextIndex);

	// Find index of weapon in Equipment array
	int FindEquipmentIndex(ABaseWeapon* Weapon);

	int EquippedIndex;

	UMeshComponent* ArmsMesh;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Points;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ABaseWeapon*> Equipment;

	UPROPERTY(EditAnywhere, Category = "PlayerCharacter|Mesh")
		FName MeshTag;

	UPROPERTY(EditAnywhere, Category = "PlayerCharacter|Mesh")
		FName WeaponAttachPoint;

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		void AddWeapon(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		void RemoveWeapon(ABaseWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		ABaseWeapon* GetEquippedWeapon();

	/** Equip weapon from inventory */
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		void Equip(ABaseWeapon* Weapon);

	/** Equip weapon from inventory, by index */
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Weapon")
		void EquipByIndex(int WeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Mesh")
		void SetArmsMesh(UMeshComponent* NextMesh);

	FName GetWeaponAttachPoint() const;

	UMeshComponent* GetSpecifcPawnMesh() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
