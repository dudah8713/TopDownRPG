// Copyright Next Level

#include "Player/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	// 로컬 플레이어 서브시스템인 UEnhancedInputLocalPlayerSubsystem을 가져옵니다.
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);

	// 입력 매핑 컨텍스트(AuraContext)를 서브시스템에 추가합니다.
    // 이는 특정 입력(키보드, 마우스, 게임패드 등)에 대해 정의된 행동들을 설정하고 적용하는 역할을 합니다.
    // 예를 들어, AuraContext에는 특정 키에 어떤 기능(이동, 공격 등)이 연결되는지 정의할 수 있습니다.
    // 이 컨텍스트를 추가함으로써 게임 내에서 해당 컨텍스트에 정의된 입력 매핑이 활성화됩니다.
    // 우선순위를 0으로 설정하여 이 컨텍스트가 다른 입력 매핑보다 우선적으로 적용되도록 합니다
	SubSystem->AddMappingContext(AuraContext, 0);

	// 마우스 커서를 화면에 표시할지 여부를 설정합니다.
	bShowMouseCursor   = true;
	// 기본 마우스 커서를 설정합니다. 여기서는 기본 커서 모양을 사용합니다.
	DefaultMouseCursor = EMouseCursor::Default;

	// 게임과 UI 모드에서 사용할 입력 모드를 설정합니다.
	FInputModeGameAndUI InputModeData;
	// 마우스가 뷰포트에 잠기지 않도록 설정합니다. (윈도우 밖으로 마우스 커서가 나갈 수 있음을 의미)
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 입력 캡처 중에도 마우스 커서를 숨기지 않도록 설정합니다.
	InputModeData.SetHideCursorDuringCapture(false);
	// 설정된 입력 모드를 플레이어 컨트롤러에 적용합니다.
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// 이동 입력을 처리하는 함수입니다.
	// 이동 입력에 대한 처리를 여기에 작성합니다.
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotaion(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	// 마우스 커서 아래의 위치에서 히트(충돌)된 오브젝트나 위치를 검사하는 데 사용
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 *  A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 *	B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 *	C. LastActor is valid && ThisActor is null
	 *		- UnHighlight LastActor
	 *	D. Both actors are valid, but LastActor != ThisActor
	 *		- UnHighlight LastActor, and Highlight ThisActor
	 *	E. Both actors are valid, and are the same actor
	 *		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}
