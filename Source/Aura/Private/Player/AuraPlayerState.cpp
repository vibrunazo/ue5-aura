// vib


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Character/AuraCharacter.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, PlayerLevel);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::SetPlayerLevel(int32 NewLevel)
{
	PlayerLevel = NewLevel;
	OnPlayerLevelChanged.Broadcast(PlayerLevel);
}

void AAuraPlayerState::AddPlayerLevel(int32 Amount)
{
	PlayerLevel += Amount;
	OnPlayerLevelChanged.Broadcast(PlayerLevel);
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(XP);
}

void AAuraPlayerState::AddXP(int32 Amount)
{
	const int32 CurrentLevel = PlayerLevel;
	const int32 CurrentXP = XP;
	const int32 NewLevel = LevelUpInfo->FindLevelForXP(XP + Amount);
	int32 NumLevelUps = NewLevel - CurrentLevel;
	XP += Amount;

	while (NumLevelUps > 0)
	{
		LevelUp();
		NumLevelUps--;
	}
	OnXPChanged.Broadcast(XP);
}

void AAuraPlayerState::AddAttributePoints(int32 Amount)
{
	AttributePoints += Amount;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddSpellPoints(int32 Amount)
{
	SpellPoints += Amount;
	OnSpellPointsChanged.Broadcast(SpellPoints);
}

void AAuraPlayerState::LevelUp()
{
	FSingleLevelInfo Rewards = LevelUpInfo->AllLevelsInfo[PlayerLevel];
	int32 AttributePointsReward = Rewards.AttributePointReward;
	int32 SpellPointsReward = Rewards.SpellPointReward;
	// PlayerLevel++;
	AddPlayerLevel(1);
	// Add to AttributePoints and SpellPoints
	int32 APReward = LevelUpInfo->AllLevelsInfo[PlayerLevel].AttributePointReward;
	AddAttributePoints(APReward);
	int32 SPReward = LevelUpInfo->AllLevelsInfo[PlayerLevel].SpellPointReward;
	AddSpellPoints(SPReward);
	
	UAuraAttributeSet* AuraAttributes = Cast<UAuraAttributeSet>(AttributeSet);
	if (!AuraAttributes) return;
	AuraAttributes->TopHealthAndMana();
	if (!AbilitySystemComponent) return;
	AActor* AuraCharacter = AbilitySystemComponent->GetAvatarActor();
	if (!AuraCharacter) return;
	if (!AuraCharacter->Implements<UPlayerInterface>()) return;;
	IPlayerInterface::Execute_LevelUp(AuraCharacter);
	

	// OnPlayerLevelChanged.Broadcast(PlayerLevel);
}

void AAuraPlayerState::OnRep_PlayerLevel(int32 OldLevel)
{
	OnPlayerLevelChanged.Broadcast(PlayerLevel);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP);
	
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldAttributePoints)
{
	OnSpellPointsChanged.Broadcast(SpellPoints);
}
