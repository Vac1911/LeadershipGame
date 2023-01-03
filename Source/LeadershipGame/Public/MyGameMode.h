// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameState.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LEADERSHIPGAME_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	/*AMyGameMode(const FObjectInitializer& ObjectInitializer);*/

	/** The default pawn class for units. */
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Classes)
	TSubclassOf<APawn> UnitClass;

	/** Transition from WaitingToStart to InProgress. You can call this manually, will also get called if ReadyToStartMatch returns true */
	UFUNCTION(BlueprintCallable, Category = "Game")
	virtual void HandleMatchHasStarted() override;
};
