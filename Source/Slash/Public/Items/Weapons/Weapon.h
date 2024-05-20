// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "WeaponTypes.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;


UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public: 
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DisableEmbers();
	void DisableSphereCollision();
	void PlayEquipSound();
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	void SetVisibility();

	TArray<AActor*> IgnoreActors;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;
protected: 
	virtual void BeginPlay() override;

	
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool ActorIsSameType(AActor* OtherActor);
	void ExecuteGetHit(FHitResult& BoxHit);


	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);





private: 

	void BoxTrace(FHitResult& BoxHit);


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowDebug = false;


	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBox;

	AActor* AttackedActor;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;


public:
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; };
	AActor* GetAttackedActor() { return AttackedActor; };
	AActor* DeleteActor() { return AttackedActor = nullptr; };



};