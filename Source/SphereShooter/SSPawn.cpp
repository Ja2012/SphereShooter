#include "SSPawn.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "SSSphere.h"

DEFINE_LOG_CATEGORY_STATIC(ASSPawnLogCategory, All, All)

ASSPawn::ASSPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    SetRootComponent(CameraComponent);
    CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    CameraComponent->SetOrthoWidth(1920.f);
    CameraComponent->SetConstraintAspectRatio(true);

}

// Called when the game starts or when spawned
void ASSPawn::BeginPlay()
{
    Super::BeginPlay();
    SetActorLocation(FVector(0.f, 0.f, 500.f));
    SetActorRotation(FRotator(-90.f, 0.f, 0.f), ETeleportType::None);
}

void ASSPawn::Shoot(const FInputActionValue& Value)
{
    UE_LOG(ASSPawnLogCategory, Warning, TEXT("Shoot"));
    Roll();
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
