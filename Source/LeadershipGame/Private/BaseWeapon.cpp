// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "ShooterCharacter.h"

ABaseWeapon::ABaseWeapon()
{
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	MeshSkeleton = FindComponentByClass<USkeletalMeshComponent>();
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
	AttachWeapon();

	bIsEquipped = true;
}

void ABaseWeapon::OnUnEquip()
{
	DetachWeapon();
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
	//SetOwningPawn(NULL);

	if (IsEquipped())
	{
		OnUnEquip();
	}
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
	else {
		UE_LOG(LogTemp, Warning, TEXT("ABaseWeapon::SetOwningPawn - NewOwner was the same as current owner"));
	}
}

void ABaseWeapon::AttachWeapon()
{
	FName AttachPoint = OwningCharacter->GetWeaponAttachPoint();
	UMeshComponent* PawnMesh = OwningCharacter->GetSpecifcPawnMesh();
	if (!PawnMesh) {
		UE_LOG(LogTemp, Fatal, TEXT("ABaseWeapon::AttachWeapon - OwningCharacter->GetSpecifcPawnMesh() was null"));
	}
	if (!MeshSkeleton) {
		UE_LOG(LogTemp, Fatal, TEXT("ABaseWeapon::AttachWeapon - MeshSkeleton was null"));
	}
	MeshSkeleton->SetHiddenInGame(false);
	MeshSkeleton->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachPoint);
	//MeshSkeleton->AttachToComponent(PawnMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false), AttachPoint);

	/*FQuat QuatRotation = FQuat::MakeFromRotator(FRotator(0, -90, 0));
	AddActorLocalRotation(QuatRotation, false, 0, ETeleportType::None);*/

	FTransform AttachmentTransform = MeshSkeleton->GetSocketTransform(AttachPoint, ERelativeTransformSpace::RTS_Component);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("SocketLocation: %s"), *AttachmentTransform.GetLocation().ToString()));
	AddActorLocalOffset(-AttachmentTransform.GetLocation(), false, 0, ETeleportType::None);
}

void ABaseWeapon::DetachWeapon()
{
	MeshSkeleton->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	MeshSkeleton->SetHiddenInGame(true);
}