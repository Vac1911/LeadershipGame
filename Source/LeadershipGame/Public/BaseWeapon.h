// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class AShooterCharacter;


UCLASS()
class LEADERSHIPGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Set the pawn which owns this weapon */
	void SetOwningPawn(AShooterCharacter* NewOwner);

	/** Attaches the weapon mesh to character mesh */
	void AttachWeapon();

	/** Detaches the weapon mesh from character mesh */
	void DetachWeapon();

	/** is weapon currently equipped? */
	uint32 bIsEquipped : 1;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MeshSkeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AShooterCharacter* OwningCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MagCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmmoCount;

	UFUNCTION(BlueprintCallable, Category = "ActorComponent|Weapon")
	virtual void Shoot(FVector Start, FVector Forward);

	UFUNCTION(BlueprintCallable, Category = "ActorComponent|Weapon")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = "ActorComponent|Weapon")
	virtual void OnKill();

	/** Weapon is being equipped by owner pawn */
	virtual void OnEquip();

	/** Weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	/** Check if it's currently equipped */
	bool IsEquipped() const;

	/** Weapon is entering a character's inventory */
	void OnEnterInventory(AShooterCharacter* NewOwner);

	/** Weapon is leaving a character's inventory */
	void OnLeaveInventory();
};
