#include "Items/Item.h"
#include "Slash/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickUpInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"



// Sets default values
AItem::AItem() {
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    itemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    itemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); 
    itemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RootComponent = itemMesh;


    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    Sphere->SetupAttachment(GetRootComponent());

    ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
    ItemEffect->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AItem::BeginPlay() {
    Super::BeginPlay();

    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);

    Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);


    //Avg<int32>(1, 3);

    /*UWorld* World = GetWorld();
    SetActorLocation(FVector(0.f, 0.f, 50.f));
    SetActorRotation(FRotator(0.f, 45.f, 0.f));



    FVector Location = GetActorLocation();
    FVector Forward = GetActorForwardVector();
    FVector endLocation = Location + Forward * 100.0f;
    DRAW_SPHERE(Location);
    DRAW_VECTOR(Location, endLocation, FColor::Blue);*/




}

float AItem::transformedSin() {
    return Amplitude * FMath::Sin(runningTime * timeConstant);
}

float AItem::transformedCos() {
    return Amplitude * FMath::Cos(runningTime * timeConstant);

}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
    if (PickUpInterface) {
        PickUpInterface->SetOverlappingItem(this);

    }
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
    IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
    if (PickUpInterface) {
        PickUpInterface->SetOverlappingItem(nullptr);

    }
}

void AItem::SpawnPickupSystem() {
    if (PickupEffect) {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this,
            PickupEffect,
            GetActorLocation()
        );
    }
}

void AItem::SpawnPickupSound() {
    if (PickupSound) {
        UGameplayStatics::SpawnSoundAtLocation(
            this,
            PickupSound,
            GetActorLocation()
        );
    }

}

// Called every frame
void AItem::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    runningTime += DeltaTime;

    const float Z = transformedSin();

    if (ItemState == EItemState::EIS_Hovering) {

        AddActorWorldOffset(FVector(0.f, 0.f, Z));
    }

    //AddActorWorldRotation(FRotator(DeltaTime * 100.f, 0.f, 0.f));



    //float DeltaZ = Amplitude * FMath::Sin(runningTime * timeConstant); // period = 2*pi/K

    //AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ)); 

    //Movement rate in units of cm/s
    /*float movementRate = 50.f;
    float rotationRate = 45.f;*/



    // MovementRate * DeltaTime (cm/s) * (s/frames) == (cm/frame)
    /*AddActorWorldOffset(FVector(movementRate * DeltaTime , 0.f, 0.f));
    AddActorWorldRotation(FRotator(0.f, rotationRate * DeltaTime, 0.f));*/


    /*DRAW_SPHERE_SINGLEFRAME(GetActorLocation());
    DRAW_VECTOR_SINGLEFRAME(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Cyan);*/


}

