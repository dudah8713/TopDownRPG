// Copyright Next Level


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(50.f);
}


// �� �Լ��� Unreal Engine�� ��Ʈ��ũ ���� �ý��ۿ��� �Ӽ��� ������ �� ȣ��˴ϴ�.
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME_CONDITION_NOTIFY ��ũ�θ� ����Ͽ� �Ӽ��� ��Ʈ��ũ �󿡼� �����ϵ��� �����մϴ�.
	// COND_None: ���� ������ ������ ��Ÿ����, �׻� �����˴ϴ�.
	// REPNOTIFY_Always: �Ӽ� ���� ����� ������ OnRep_ �Լ��� ȣ��ǵ��� �����մϴ�.
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	// �� ��ũ�δ� Unreal�� Gameplay Attribute �ý��ۿ��� �Ӽ� ������ �ݿ��ϰ� �˸��� �߻���ŵ�ϴ�.
	// Health �Ӽ��� ��Ʈ��ũ ���� �ý��ۿ� ����մϴ�.
	// UAuraAttributeSet, Health, OldHealth �Ķ���ͷ� Health �Ӽ��� ����� ��Ȳ���� ���ο� ���� �˸��ϴ�.
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}
