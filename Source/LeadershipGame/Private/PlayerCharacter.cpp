// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TArray<UActorComponent*> components = GetComponentsByTag(UMeshComponent::StaticClass(), MeshTag);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void APlayerCharacter::SetArmsMesh(UMeshComponent* NextMesh)
{
	ArmsMesh = NextMesh;
}

void APlayerCharacter::AddWeapon(ABaseWeapon* Weapon)
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

void APlayerCharacter::RemoveWeapon(ABaseWeapon* Weapon)
{
	if (Weapon)
	{
		Weapon->OnLeaveInventory();
		Equipment.RemoveSingle(Weapon);
	}
}

void APlayerCharacter::Equip(ABaseWeapon* Weapon)
{
	if (Weapon && Equipment.Contains(Weapon))
	{
		SetEquippedIndex(FindEquipmentIndex(Weapon));
	}
}

void APlayerCharacter::EquipByIndex(int WeaponIndex)
{
	if (Equipment.IsValidIndex(WeaponIndex))
	{
		SetEquippedIndex(WeaponIndex);
	}
}

int APlayerCharacter::FindEquipmentIndex(ABaseWeapon* Weapon)
{
	return Equipment.Find(Weapon);
}

ABaseWeapon* APlayerCharacter::GetEquippedWeapon()
{
	if (Equipment.IsValidIndex(EquippedIndex))
	{
		return Equipment[EquippedIndex];
	}

	return nullptr;
}

void APlayerCharacter::SetEquippedIndex(int NextIndex)
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

FName APlayerCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

UMeshComponent* APlayerCharacter::GetSpecifcPawnMesh() const
{
	return ArmsMesh;
}