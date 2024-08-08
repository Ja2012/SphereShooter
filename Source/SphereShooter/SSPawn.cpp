#include "SSPawn.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "SSSphere.h"
#include "Components/SceneComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ASSPawnLogCategory, All, All)

ASSPawn::ASSPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);
     
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    CameraComponent->SetOrthoWidth(1920.f);
    CameraComponent->SetConstraintAspectRatio(true);
    CameraComponent->SetupAttachment(SceneComponent);
    
    NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
    NiagaraComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ASSPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ASSPawn::Shoot(const FInputActionValue& Value)
{
    UE_LOG(ASSPawnLogCategory, Warning, TEXT("Shoot"));
    Roll();
}

void ASSPawn::MoveMouse(const FInputActionValue& Value) 
{
    //UE_LOG(ASSPawnLogCategory, Warning, TEXT("MoveMouse"));
    FVector2D MousePos = Value.Get<FVector2D>();


    //UE_LOG(ASSPawnLogCategory, Warning, TEXT("Mouse coordinates %s"), *MousePos.ToString());
    //FRotator Rot;
    //MousePos.
    //NiagaraComponent->rota;
}

// Called every frame
void ASSPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASSPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if(UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ASSPawn::Shoot);
        Input->BindAction(MouseMove, ETriggerEvent::Triggered, this, &ASSPawn::MoveMouse);
    }
}

void ASSPawn::PawnClientRestart()
{
    Super::PawnClientRestart();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(InputMappingContext, 0);
		}
    }
}

void ASSPawn::SetRollBall(ASSSphere* Ball)
{
    CurrentRollBoll = Ball;
}

void ASSPawn::Roll() 
{
    CurrentRollBoll->Roll();
}
