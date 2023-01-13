// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "ShooterCharacter.h"

ABaseWeapon::ABaseWeapon()
{
}

void ABaseWeapon::Shoot(FVector Start, FVector Forward)
{
}

void ABaseWeapon::Reload()
{
	if (AmmoCount >= MagSize - MagCount) {
		// If we have enough ammo to fill the magazine: Fill the mag.
		AmmoCount -= MagSize - MagCount;
		MagCount = MagSize;
	}
	else {
		// Otherwise add all remaining ammo to the mag
		MagCount += AmmoCount;
		AmmoCount = 0;
	}
}

void ABaseWeapon::OnKill()
{
}

void ABaseWeapon::OnEquip()
{
	AttachMeshToPawn();

	bIsEquipped = true;
}

void ABaseWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
	bIsEquipped = false;
}


bool ABaseWeapon::IsEquipped() const
{
	return bIsEquipped;
}

void ABaseWeapon::OnEnterInventory(AShooterCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ABaseWeapon::OnLeaveInventory()
{
	SetOwningPawn(NULL);

	if (IsEquipped())
	{
		OnUnEquip();
	}
}

void ABaseWeapon::AttachMeshToPawn()
{
	if (OwningCharacter)
	{
		// Remove and hide both first and third person meshes
		DetachMeshFromPawn();

		FName AttachPoint = OwningCharacter->GetWeaponAttachPoint();
		UMeshComponent* PawnMesh = OwningCharacter->GetSpecifcPawnMesh();
		MeshSkeleton->SetHiddenInGame(false);

		if (PawnMesh)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Mesh: Yes");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Mesh: No");
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Attach: " + AttachPoint.ToString());
		MeshSkeleton->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Attach: No Pawn");
	}
}

void ABaseWeapon::DetachMeshFromPawn()
{
	MeshSkeleton->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	MeshSkeleton->SetHiddenInGame(true);
}

void ABaseWeapon::SetOwningPawn(AShooterCharacter* NewOwner)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, "Owner: " + NewOwner->GetActorLabel());
	if (OwningCharacter != NewOwner)
	{
		SetInstigator(NewOwner);
		OwningCharacter = NewOwner;
		// net owner for RPC calls
		SetOwner(NewOwner);
	}
}