// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "BaseWeapon.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LEADERSHIPGAME_API APlayerCharacter : public AShooterCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	UMeshComponent* ArmsMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "PlayerCharacter|Mesh")
	FName MeshTag;

	UPROPERTY(EditAnywhere, Category = "PlayerCharacter|Mesh")
	FName WeaponAttachPoint;

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Mesh")
	void SetArmsMesh(UMeshComponent* NextMesh);

	virtual FName GetWeaponAttachPoint() override;

	virtual UMeshComponent* GetSpecifcPawnMesh() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
