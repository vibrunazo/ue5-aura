// vib


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Mage;
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	if (!AuraPlayerState) return;
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	InitializeDefaultAttributes();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;
	AAuraPlayerState* PS = Cast<AAuraPlayerState>(GetPlayerState());
	if (!PS) return;
	AAuraHUD* HUD = PC->GetHUD<AAuraHUD>();
	if (!HUD) return;
	HUD->InitOverlay(PC, PS, AbilitySystemComponent, AttributeSet);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Ability Actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Ability Actor info for the client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	if (!AuraPlayerState or !AuraPlayerState->LevelUpInfo) return 0;
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	if (!AuraPlayerState) return 0;
	return AuraPlayerState->GetXP();
	
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	if (!AuraPlayerState) return;
	AuraPlayerState->AddXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	// TODO: Get AttributePointsReward and SpellPointsReward
	// Add to Player Level
	// Add to AttributePoints and SpellPoints
	// Fill up Health and Mana
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState());
	if (!AuraPlayerState) return 0;
	return AuraPlayerState->GetPlayerLevel();
}
