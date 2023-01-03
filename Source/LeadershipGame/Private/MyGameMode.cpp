// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"

//AMyGameMode::AMyGameMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
//{
//	GameStateClass = AMyGameState::StaticClass();
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("AMyGameMode::Constructor"));
//}

void AMyGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("AMyGameMode::HandleMatchHasStarted"));
	AMyGameState* ThisGameState = GetGameState<AMyGameState>();
	ThisGameState->SpawnFireteam(FVector::Zero(), UnitClass);
}
