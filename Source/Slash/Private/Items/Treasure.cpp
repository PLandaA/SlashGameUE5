#include "Items/Treasure.h"
#include "Characters/SlashCharacter.h"


void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

    IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
    if (PickUpInterface) {
        PickUpInterface->AddGold(this);
        SpawnPickupSound();
        Destroy();
    }

}
