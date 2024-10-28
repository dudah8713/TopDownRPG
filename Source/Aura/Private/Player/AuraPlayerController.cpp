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

	// ���� �÷��̾� ����ý����� UEnhancedInputLocalPlayerSubsystem�� �����ɴϴ�.
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);

	// �Է� ���� ���ؽ�Ʈ(AuraContext)�� ����ý��ۿ� �߰��մϴ�.
    // �̴� Ư�� �Է�(Ű����, ���콺, �����е� ��)�� ���� ���ǵ� �ൿ���� �����ϰ� �����ϴ� ������ �մϴ�.
    // ���� ���, AuraContext���� Ư�� Ű�� � ���(�̵�, ���� ��)�� ����Ǵ��� ������ �� �ֽ��ϴ�.
    // �� ���ؽ�Ʈ�� �߰������ν� ���� ������ �ش� ���ؽ�Ʈ�� ���ǵ� �Է� ������ Ȱ��ȭ�˴ϴ�.
    // �켱������ 0���� �����Ͽ� �� ���ؽ�Ʈ�� �ٸ� �Է� ���κ��� �켱������ ����ǵ��� �մϴ�
	SubSystem->AddMappingContext(AuraContext, 0);

	// ���콺 Ŀ���� ȭ�鿡 ǥ������ ���θ� �����մϴ�.
	bShowMouseCursor   = true;
	// �⺻ ���콺 Ŀ���� �����մϴ�. ���⼭�� �⺻ Ŀ�� ����� ����մϴ�.
	DefaultMouseCursor = EMouseCursor::Default;

	// ���Ӱ� UI ��忡�� ����� �Է� ��带 �����մϴ�.
	FInputModeGameAndUI InputModeData;
	// ���콺�� ����Ʈ�� ����� �ʵ��� �����մϴ�. (������ ������ ���콺 Ŀ���� ���� �� ������ �ǹ�)
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// �Է� ĸó �߿��� ���콺 Ŀ���� ������ �ʵ��� �����մϴ�.
	InputModeData.SetHideCursorDuringCapture(false);
	// ������ �Է� ��带 �÷��̾� ��Ʈ�ѷ��� �����մϴ�.
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
	// �̵� �Է��� ó���ϴ� �Լ��Դϴ�.
	// �̵� �Է¿� ���� ó���� ���⿡ �ۼ��մϴ�.
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
	// ���콺 Ŀ�� �Ʒ��� ��ġ���� ��Ʈ(�浹)�� ������Ʈ�� ��ġ�� �˻��ϴ� �� ���
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
