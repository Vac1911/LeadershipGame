// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

class APlayerCharacter;

// This class exists so that ShootableInterface and Weapon are not circular dependencies, and to provide a stronger type binding than AActor*

UCLASS()
class LEADERSHIPGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

protected:
	class APlayerCharacter* OwningCharacter;

	void SetOwningPawn(APlayerCharacter* NewOwner);

	/** is weapon currently equipped? */
	uint32 bIsEquipped : 1;

public:
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* MeshSkeleton;

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

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip();

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	/** check if it's currently equipped */
	bool IsEquipped() const;

	/** attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

	void OnEnterInventory(APlayerCharacter* NewOwner);

	void OnLeaveInventory();
};
