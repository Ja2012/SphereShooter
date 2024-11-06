// SPDX-License-Identifier: MIT

#include "Player/SsPawn.h"

#include "Sphere/SsBaseSphere.h"
#include "GameModes/SsGameLevelGameMode.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"


DEFINE_LOG_CATEGORY_STATIC(ASSPawnLogCategory, All, All)

ASsPawn::ASsPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    // CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    // CameraComponent->SetOrthoWidth(1920.f);
    // CameraComponent->SetConstraintAspectRatio(true);
    CameraComponent->bUsePawnControlRotation = true;
    CameraComponent->SetupAttachment(SceneComponent);

    AimBeamNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
    AimBeamNiagaraComponent->SetupAttachment(SceneComponent);
}

void ASsPawn::BeginPlay()
{
    Super::BeginPlay();

    GameMode = Cast<ASsGameLevelGameMode>(GetWorld()->GetAuthGameMode());

    AimBeamNiagaraComponent->SetWorldLocation(GameMode->GetRollBallSpawn()->GetActorLocation() + FVector(0, 0, 5.f));
    AimBeamNiagaraComponent->SetAsset(AimBeamNiagaraSystem);
    AimBeamNiagaraComponent->SetVariableVec3(AimBeamLengthVarName, AimBeamLengthVarValue);
}

void ASsPawn::ShootRollBall(const FInputActionValue& Value)
{
    if (!AimBeamNiagaraComponent || !IsValid(CurrentRollBoll)) return;
    Roll(AimBeamNiagaraComponent->GetForwardVector() * ShootScaleImpulse);
}



void ASsPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ASsPawn::ShootRollBall);
    }
}

void ASsPawn::PawnClientRestart()
{
    Super::PawnClientRestart();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = //
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

void ASsPawn::Roll(const FVector& Impulse)
{
    if (CurrentRollBoll)
    {
        CurrentRollBoll->Roll(Impulse);
        CurrentRollBoll = nullptr;
    }
}

void ASsPawn::SetAimBeamRotation(FRotator Rotator)
{
     if (!AimBeamNiagaraComponent) return;
     AimBeamNiagaraComponent->SetWorldRotation(Rotator);
}
