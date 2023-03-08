// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UnitInterface.h"
#include "UnitAIController.h"
#include "Unit/UnitGroup.h"
#include "GameFramework/Character.h"
#include "UnitCharacter.generated.h"

UCLASS()
class LEADERSHIPGAME_API AUnitCharacter : public ACharacter, public IUnitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUnitCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/*
	* Unit Interface
	*/
	UUnitGroup* ParentGroup;

	bool HasCompletedOrder = false;

	AUnitAIController* GetUnitController();

	FVector GetLocation_Implementation() override;

	UObject* GetCurrentOrder_Implementation() override;

	/** Remove Current Order */
	void StopCurrentOrder_Implementation() override;

	/** Currently Unit is idle */
	bool IsIdle_Implementation() override;

	/** Is the current order complete? */
	bool GetOrderIsComplete_Implementation() override;

	void SetOrderIsComplete_Implementation(bool IsComplete) override;

	UObject* GetParentGroup_Implementation() /*override*/;

	void SetParentGroup_Implementation(UObject* Parent) /*override*/;
};
