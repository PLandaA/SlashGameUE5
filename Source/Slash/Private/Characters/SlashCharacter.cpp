// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GroomComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Items/Weapons/WeaponTypes.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"




ASlashCharacter::ASlashCharacter() {
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;


    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);

    GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
    GetMesh()->SetGenerateOverlapEvents(true);


    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(GetRootComponent());
    SpringArm->TargetArmLength = 300;
    SpringArm->bUsePawnControlRotation = true;

    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(SpringArm);
    ViewCamera->bUsePawnControlRotation = false;

    Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
    Hair->SetupAttachment(GetMesh());
    Hair->AttachmentName = FString("head");


    Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
    Eyebrows->SetupAttachment(GetMesh());
    Eyebrows->AttachmentName = FString("head");

}

void ASlashCharacter::Tick(float DeltaTime) {
    if (Attributes && SlashOverlay) {
        Attributes->RegenStamina(DeltaTime);
        SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());
    }
    Super::Tick(DeltaTime);
}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnchancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
        EnchancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
        EnchancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
        EnchancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
        EnchancedInputComponent->BindAction(EKeyAction, ETriggerEvent::Started, this, &ASlashCharacter::EKeyPressed);
        EnchancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
        EnchancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);

    }

}

float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) {
    HandleDamage(DamageAmount);
    SetHUDHealth();
    return DamageAmount;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) {
    Super::GetHit_Implementation(ImpactPoint, Hitter);

    SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
    if (Attributes && Attributes->GetHealthPercent() > 0.f) {

        ActionState = EActionState::EAS_HitReaction;
    }

}

void ASlashCharacter::SetOverlappingItem(AItem* Item) {
    OverlappingItem = Item;

}

void ASlashCharacter::AddSouls(ASoul* Soul) {
    if (Attributes && SlashOverlay) {
        Attributes->AddSouls(Soul->GetSouls());
        SlashOverlay->SetSouls(Attributes->GetSouls());

    }
}

void ASlashCharacter::AddGold(ATreasure* Treasure) {
    if (Attributes && SlashOverlay) {
        Attributes->AddGold(Treasure->GetGold());
        SlashOverlay->SetGold(Attributes->GetGold());


    }
}

void ASlashCharacter::Jump() {
    if (IsUnoccupied()) {
        Super::Jump();
    }


}

void ASlashCharacter::BeginPlay() {
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
            Subsystem->AddMappingContext(SlashContext, 0);
        }

    }

    Tags.Add(FName("EngageableTarget"));
    InitializeSlashOverlay();



}

void ASlashCharacter::Move(const FInputActionValue& Value) {

    if (ActionState != EActionState::EAS_Unoccupied) return;
    const FVector2D movementVector = Value.Get<FVector2D>();

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    AddMovementInput(ForwardDirection, movementVector.Y);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(RightDirection, movementVector.X);

}

void ASlashCharacter::Look(const FInputActionValue& Value) {
    const FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerPitchInput(LookAxisVector.Y);
    AddControllerYawInput(LookAxisVector.X);

}

void ASlashCharacter::EKeyPressed() {
    AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
    if (OverlappingWeapon/* && !hasWeapon*/) {
        //Destroy Weapon!
       if (EquippedWeapon) {
            EquippedWeapon->Destroy();

        }
        if (OverlappingWeapon->WeaponState == EWeaponState::EWS_OneHandedWeapon) {
            OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
            CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
            Weapons.Add(OverlappingWeapon);
            //OverlappingWeapon->SetVisibility();
        }
        else if (OverlappingWeapon->WeaponState == EWeaponState::EWS_TwoHandedWeapon) {
            OverlappingWeapon->Equip(GetMesh(), FName("LeftHandSocket"), this, this);
            CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;;
            Weapons.Add(OverlappingWeapon);
            //OverlappingWeapon->SetVisibility();
        }
        EquipWeapon(OverlappingWeapon);
    }
    else {
        if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon) {
            return;
        }
        if (CanDisarm()) {
            Disarm();
        }
        else if (CanArm()) {
            Arm();

        }
    }

}

void ASlashCharacter::Attack() {
    if (CanAttack()) {
        if (EquippedWeapon) {
            ClearAttackTimer();
            CombatTarget = EquippedWeapon->GetAttackedActor();
            if (CombatTarget) {
                StartAttackTimer();
            }

        }
        if (CharacterState == ECharacterState::ECS_EquippedOneHandedWeapon) {

            PlayAttackMontage();
        }
        else if (CharacterState == ECharacterState::ECS_EquippedTwoHandedWeapon) {
            PlayTwoHandedAttackMontage();
        }
        ActionState = EActionState::EAS_Attacking;
    }
}

void ASlashCharacter::Dodge() {
    if (IsOcuppied() || !HasEnoughStamina()) {
        return;
    }

    PlayDodgeMontage();
    ActionState = EActionState::EAS_Dodge;
    if (Attributes && SlashOverlay) {
        Attributes->UseStamina(Attributes->GetDodgeCost());
        SlashOverlay->SetStaminaBarPercent(Attributes->GetStaminaPercent());

    }

}



void ASlashCharacter::EquipWeapon(AWeapon* Weapon) {
    OverlappingItem = nullptr;
    EquippedWeapon = Weapon;
    hasWeapon = true;

}

void ASlashCharacter::AttackEnd() {
    ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::DodgeEnd() {
    Super::DodgeEnd();

    ActionState = EActionState::EAS_Unoccupied;

}

bool ASlashCharacter::CanAttack() {
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped;;
}

bool ASlashCharacter::CanDisarm() {
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm() {
    return ActionState == EActionState::EAS_Unoccupied &&
        CharacterState == ECharacterState::ECS_Unequipped &&
        EquippedWeapon;


}

void ASlashCharacter::Arm() {
    PlayEquipMontage(FName("Equip"));
    CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
    ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::Disarm() {
    PlayEquipMontage(FName("Unequip"));
    CharacterState = ECharacterState::ECS_Unequipped;
    ActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::PlayTwoHandedAttackMontage() {
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && TwoHandedAttackMontage) {
        AnimInstance->Montage_Play(TwoHandedAttackMontage);
        const int32 Selection = FMath::RandRange(0, 2);
        FName SectionName = FName();
        switch (Selection) {
            case 0:
                SectionName = FName("HeavyAttack1");
                break;
            case 1:
                SectionName = FName("HeavyAttack2");
                break;

            case 2:
                SectionName = FName("HeavyAttack3");
                break;

            default:
                break;
        }
        AnimInstance->Montage_JumpToSection(SectionName, TwoHandedAttackMontage);

    }
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName) {
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance && EquipMontage) {
        AnimInstance->Montage_Play(EquipMontage);
        AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);

    }
}

void ASlashCharacter::Die_Implementation() {

    Super::Die_Implementation();

    ActionState = EActionState::EAS_Dead;
    DisableMeshCollision();
}

bool ASlashCharacter::HasEnoughStamina() {
    return Attributes && Attributes->GetStamina() > Attributes->GetDodgeCost();
}

bool ASlashCharacter::IsOcuppied() {
    return ActionState != EActionState::EAS_Unoccupied;
}

void ASlashCharacter::ClearAttackTimer() {
    GetWorldTimerManager().ClearTimer(AttackTimer);

}

void ASlashCharacter::StartAttackTimer() {
    const float AttackTime = 2.5f;
    GetWorldTimerManager().SetTimer(AttackTimer, this, &ASlashCharacter::LoseInterest, AttackTime);



}

void ASlashCharacter::LoseInterest() {
    CombatTarget = EquippedWeapon->DeleteActor();
    CombatTarget = nullptr;
}

void ASlashCharacter::AttachWeaponToBack() {
    if (EquippedWeapon) {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));

    }
}

void ASlashCharacter::AttachWeaponToHand() {
    if (EquippedWeapon) {
        EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));

    }
}

void ASlashCharacter::FinishEquipping() {
    ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::HitReactEnd() {
    ActionState = EActionState::EAS_Unoccupied;
    ClearAttackTimer();

}

bool ASlashCharacter::IsUnoccupied() {
    return ActionState == EActionState::EAS_Unoccupied;
}

void ASlashCharacter::InitializeSlashOverlay() {
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController) {
        ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
        if (SlashHUD) {
            SlashOverlay = SlashHUD->GetSlashOverlay();
            if (SlashOverlay && Attributes) {
                SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());
                SlashOverlay->SetStaminaBarPercent(1.f);
                SlashOverlay->SetGold(0);
                SlashOverlay->SetSouls(0);

            }
        }

    }
}

void ASlashCharacter::SetHUDHealth() {
    if (SlashOverlay && Attributes) {
        SlashOverlay->SetHealthBarPercent(Attributes->GetHealthPercent());

    }
}






