// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameState.h"

void AMyGameState::AddGroup(UUnitGroup* Group)
{
	UnitGroups.Add(Group);
}

UUnitGroup* AMyGameState::SpawnFireteam(FVector Location, TSubclassOf<APawn> UnitType)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("AMyGameState::SpawnFireteam"));
	UUnitGroup* UnitGroup = NewObject<UUnitGroup>();
	if (IsValid(UnitType))
	{
		for (int i = 0; i < 4; i++) {
			FVector SpawnLocation = Location + FVector(0.0f, 0.0f, 100.0f);
			FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

			FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
			SpawnParameters.bNoFail = true;

			APawn* SpawnedActor = GetWorld()->SpawnActor<APawn>(UnitType, SpawnLocation, SpawnRotation, SpawnParameters);
			UnitGroup->Units.Add(SpawnedActor);
		}
		AddGroup(UnitGroup);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UnitType is invalid"));
	}
	return UnitGroup;
}