#include "SSPawn.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "SSSphere.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "SSGameLevelGameMode.h"

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
    CameraComponent->bUsePawnControlRotation = true;
    CameraComponent->SetupAttachment(SceneComponent);

    AimBeamNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
    AimBeamNiagaraComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ASSPawn::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = Cast<APlayerController>(GetController());    
    GameMode = Cast<ASSGameLevelGameMode>(GetWorld()->GetAuthGameMode());

    FVector NiagaraLocation = GameMode->GetPlayerBallLocation();
    NiagaraLocation.Z = 1;
    AimBeamNiagaraComponent->SetWorldLocation(NiagaraLocation);
    AimBeamNiagaraComponent->SetNiagaraVariableVec3(AimBeamLengthVarName, AimBeamLengthVarValue);
    AimBeamNiagaraComponent->SetAsset(AimBeamNiagaraSystem);
}

void ASSPawn::ShootRollBall(const FInputActionValue& Value)
{
    if (!AimBeamNiagaraComponent || !IsValid(CurrentRollBoll)) return;
    Roll(AimBeamNiagaraComponent->GetForwardVector() * ShootScaleImpulse);
}

void ASSPawn::MoveAimBeam(const FInputActionValue& Value) 
{
    if (!AimBeamNiagaraComponent) return;
    float MouseXDelta = Value.Get<float>();
    AimBeamNiagaraComponent->AddRelativeRotation(FRotator(0.f, MouseXDelta, 0.f));

    // TODO bad aim tracking
    return;

    float X, Y;
    PlayerController->GetMousePosition(X, Y);
    GEngine->AddOnScreenDebugMessage(2, 0.f, FColor::Green, FString::Printf(TEXT("%f, %f"), X, Y), false);

    // https://forums.unrealengine.com/t/c-equivalent-of-convert-mouse-location-to-world-space/296018/7?u=ja20121
    FVector MouseLoc; 
    FVector MouseDir; 
    PlayerController->DeprojectMousePositionToWorld(MouseLoc, MouseDir);
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
        Input->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ASSPawn::ShootRollBall);
        Input->BindAction(MouseMoveAction, ETriggerEvent::Triggered, this, &ASSPawn::MoveAimBeam);
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

void ASSPawn::Roll(FVector Impulse) 
{
    CurrentRollBoll->Roll(Impulse);
}
