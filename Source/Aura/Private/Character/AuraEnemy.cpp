// Copyright Next Level


#include "Character/AuraEnemy.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

#include "Aura/Aura.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraEnemy::HighlightActor()
{
	// Ŀ���� ���� ���ۿ� ������
	GetMesh()->SetRenderCustomDepth(true);
	// �ش� �޽��� Ŀ���� ���� ���ۿ� �������� ��, �� �޽��� ���ٽ� �� 250�� �ο�
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_VALUE);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL_VALUE);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

