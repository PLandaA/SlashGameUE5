// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent() {

    PrimaryComponentTick.bCanEverTick = false;


}


void UAttributeComponent::BeginPlay() {
    Super::BeginPlay();

}

void UAttributeComponent::RecieveDamage(float Damage) {
    Health = FMath::Clamp(Health - Damage, 0, MaxHealth);
    
    
}

void UAttributeComponent::UseStamina(float StaminaCost) {
    Stamina = FMath::Clamp(Stamina - StaminaCost, 0, MaxStamina);

}

float UAttributeComponent::GetHealthPercent() {
    return Health / MaxHealth;
}

float UAttributeComponent::GetStaminaPercent() {
    return Stamina / MaxStamina;
}

bool UAttributeComponent::IsAlive() {
    return Health > 0.f;
}

void UAttributeComponent::AddSouls(int32 NumberOfSouls) {
    Souls += NumberOfSouls;
}

void UAttributeComponent::AddGold(int32 NumberOGold) {
    Gold += NumberOGold;

}

void UAttributeComponent::AddHealth(int32 NumberOfHealth) {
    Health += NumberOfHealth;

}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::RegenStamina(float DeltaTime) {
    Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

