// Copyright Next Level


#include "Character/AuraCharacter.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// ĳ���Ͱ� �̵��ϴ� ���⿡ ���� ȸ���ϵ��� �����մϴ�. ��, ĳ���Ͱ� �̵��ϴ� ������ ���� �ڿ������� ���� ������ �˴ϴ�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// ĳ���Ͱ� ȸ���ϴ� �ӵ��� �����մϴ�. Y��(����)���� �ʴ� 400�� ȸ���� �� �ֵ��� ����
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	// ĳ������ �̵��� ������� �����մϴ�. ��, ĳ���Ͱ� 3D �������� ���� �������� �̵����� �ʵ��� �Ͽ�, ĳ���Ͱ� �� �������� �����̰� �˴ϴ�.
	GetCharacterMovement()->bConstrainToPlane = true;
	// ĳ���Ͱ� ó�� ������ �� ��鿡 �����ǵ��� �����մϴ�. �̸� ���� ĳ���Ͱ� ������ �� ��� ���� ��鿡 �������� �˴ϴ�.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// �ɸ����� ȸ�� ��ġ, ��, �並 ��Ʈ�ѷ��� ȸ�������� ������� �ʵ��� �����մϴ�.	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;
}

// PossessedBy�� �������� ĳ���Ͱ� Ư�� ��Ʈ�ѷ��� ���� ������ �� ȣ��˴ϴ�. ��, ĳ���Ͱ� �÷��̾��� �Է��� �޾ƾ� �� �� �������� ���� �������� �Ҵ��ϴ� �Լ��Դϴ�.
void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the server
	InitAbilityActorInfo();
}

// OnRep_PlayerState�� Ŭ���̾�Ʈ���� PlayerState�� �����κ��� ����(Replication)�� �� ȣ��˴ϴ�
// OnRep_PlayerState�� Ŭ���̾�Ʈ ������ ������ ������ ������� ����Ǵ� �۾��Դϴ�
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
