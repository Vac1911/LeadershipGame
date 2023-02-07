// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EquippedIndex = -1;
}

void AShooterCharacter::AddWeapon(ABaseWeapon* Weapon)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Add Weapon");

	if (Weapon)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Weapon: " + Weapon->GetActorLabel());
		Weapon->OnEnterInventory(this);
		Equipment.AddUnique(Weapon);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "No Weapon");
	}
}

void AShooterCharacter::RemoveWeapon(ABaseWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->OnLeaveInventory();
		Equipment.RemoveSingle(Weapon);
	}
}

void AShooterCharacter::Equip(ABaseWeapon* Weapon)
{
	if (Weapon && Equipment.Contains(Weapon))
	{
		SetEquippedIndex(FindEquipmentIndex(Weapon));
	}
}

void AShooterCharacter::EquipByIndex(int WeaponIndex)
{
	if (Equipment.IsValidIndex(WeaponIndex))
	{
		SetEquippedIndex(WeaponIndex);
	}
}

int AShooterCharacter::FindEquipmentIndex(ABaseWeapon* Weapon)
{
	return Equipment.Find(Weapon);
}

ABaseWeapon* AShooterCharacter::GetEquippedWeapon()
{
	if (Equipment.IsValidIndex(EquippedIndex))
	{
		return Equipment[EquippedIndex];
	}

	return nullptr;
}

void AShooterCharacter::SetEquippedIndex(int NextIndex)
{
	ABaseWeapon* PrevWeapon = GetEquippedWeapon();
	ABaseWeapon* NextWeapon = Equipment[NextIndex];

	// unequip previous weapon
	if (PrevWeapon)
	{
		PrevWeapon->OnUnEquip();
	}

	EquippedIndex = NextIndex;

	// equip new weapon
	if (NextWeapon)
	{
		NextWeapon->OnEquip();
	}
}

FName AShooterCharacter::GetWeaponAttachPoint()
{
	return FName();
}

UMeshComponent* AShooterCharacter::GetSpecifcPawnMesh()
{
	return nullptr;
}
