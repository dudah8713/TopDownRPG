// Copyright Next Level


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed
	// MMORPG에서 특정 능력을 사용할 때 능력의 효과가 모든 플레이어에게 보일 필요는 없지만, 
	// 능력 사용자는 정확한 정보를 받아야 합니다. 
	// 예를 들어, 스킬의 시전 시간이나 쿨다운은 사용자가 볼 수 있어야 하지만, 
	// 세부적인 버프/디버프 정보는 서버에서 관리하는 것이 더 효율적일 수 있습니다.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
