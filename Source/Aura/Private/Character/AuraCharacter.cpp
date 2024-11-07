// Copyright Next Level


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// 캐릭터가 이동하는 방향에 따라 회전하도록 설정합니다. 즉, 캐릭터가 이동하는 방향을 향해 자연스럽게 몸을 돌리게 됩니다
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 캐릭터가 회전하는 속도를 지정합니다. Y축(상향)으로 초당 400도 회전할 수 있도록 설정
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	// 캐릭터의 이동을 평면으로 제한합니다. 즉, 캐릭터가 3D 공간에서 수직 방향으로 이동하지 않도록 하여, 캐릭터가 땅 위에서만 움직이게 됩니다.
	GetCharacterMovement()->bConstrainToPlane = true;
	// 캐릭터가 처음 생성될 때 평면에 고정되도록 설정합니다. 이를 통해 캐릭터가 시작할 때 즉시 땅의 평면에 맞춰지게 됩니다.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 케릭터의 회전 피치, 롤, 요를 컨트롤러의 회전값으로 사용하지 않도록 설정합니다.	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;
}

// PossessedBy는 서버에서 캐릭터가 특정 컨트롤러에 의해 소유될 때 호출됩니다. 즉, 캐릭터가 플레이어의 입력을 받아야 할 때 서버에서 먼저 소유권을 할당하는 함수입니다.
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

// OnRep_PlayerState는 클라이언트에서 PlayerState가 서버로부터 복제(Replication)될 때 호출됩니다
// OnRep_PlayerState는 클라이언트 측에서 복제된 데이터 기반으로 수행되는 작업입니다
void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();

}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlaterState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlaterState);
	AuraPlaterState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlaterState, this);
	AbilitySystemComponent = AuraPlaterState->GetAbilitySystemComponent();
	AttributeSet		   = AuraPlaterState->GetAttributeSet();

	if (AAuraPlayerController* AuraPlaterController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlaterController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlaterController, AuraPlaterState, AbilitySystemComponent, AttributeSet);
		}
	}
}
