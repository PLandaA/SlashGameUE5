
#include "Items/Soul.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/PickUpInterface.h"

void ASoul::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    const double LocationZ = GetActorLocation().Z;
    if (LocationZ > DesiredZ) {
        const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
        AddActorWorldOffset(DeltaLocation);
    }

}

void ASoul::BeginPlay() {
    Super::BeginPlay();

    const FVector Start = GetActorLocation();
    const FVector End = Start - FVector(0.f,0.f, 2000.f);

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
    TArray<AActor*> ActorsToIgnore; 
    ActorsToIgnore.Add(GetOwner());
    FHitResult HitResult;

    UKismetSystemLibrary::LineTraceSingleForObjects(
        this,
        Start,
        End,
        ObjectTypes,
        false,
        ActorsToIgnore, 
        EDrawDebugTrace::None, 
        HitResult, 
        true
        );

    DesiredZ = HitResult.ImpactPoint.Z + 50;

}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
    if (PickUpInterface) {
        PickUpInterface->AddSouls(this);
        SpawnPickupSystem();
        SpawnPickupSound();

        Destroy();
    } 
}