// Copyright Next Level


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed
	// MMORPG���� Ư�� �ɷ��� ����� �� �ɷ��� ȿ���� ��� �÷��̾�� ���� �ʿ�� ������, 
	// �ɷ� ����ڴ� ��Ȯ�� ������ �޾ƾ� �մϴ�. 
	// ���� ���, ��ų�� ���� �ð��̳� ��ٿ��� ����ڰ� �� �� �־�� ������, 
	// �������� ����/����� ������ �������� �����ϴ� ���� �� ȿ������ �� �ֽ��ϴ�.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
