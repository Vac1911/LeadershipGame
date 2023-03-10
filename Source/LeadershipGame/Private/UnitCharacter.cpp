// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitCharacter.h"

// Sets default values
AUnitCharacter::AUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AUnitAIController::StaticClass();

}

// Called when the game starts or when spawned
void AUnitCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUnitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

AUnitAIController* AUnitCharacter::GetUnitController()
{
	return Cast<AUnitAIController>(GetController());
}

FVector AUnitCharacter::GetLocation_Implementation()
{
	return GetActorLocation();
}

UObject* AUnitCharacter::GetCurrentOrder_Implementation()
{
	return GetUnitController()->GetCurrentOrder();
}

void AUnitCharacter::StopCurrentOrder_Implementation()
{
	GetUnitController()->StopCurrentOrder();
	SetOrderIsComplete_Implementation(true);
}

bool AUnitCharacter::IsIdle_Implementation()
{
	return GetUnitController()->IsIdle();
}

bool AUnitCharacter::GetOrderIsComplete_Implementation()
{
	return HasCompletedOrder;
}

void AUnitCharacter::SetOrderIsComplete_Implementation(bool IsComplete)
{
	HasCompletedOrder = IsComplete;
}

UObject* AUnitCharacter::GetParentGroup_Implementation()
{
	return Cast<UObject>(ParentGroup);
}

void AUnitCharacter::SetParentGroup_Implementation(UObject* Parent)
{
	ParentGroup = Cast<UUnitGroup>(Parent);
}

