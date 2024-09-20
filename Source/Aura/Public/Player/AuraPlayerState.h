// vib

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAuraAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged, int32 /** New Value */);


/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return  AttributeSet; }

	
	UFUNCTION(BlueprintCallable, Category = "Player Level")
	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }
	UFUNCTION(BlueprintCallable, Category = "Player Level")
	void SetPlayerLevel(int32 NewLevel);
	UFUNCTION(BlueprintCallable, Category = "Player Level")
	void AddPlayerLevel(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Player Level")
	void LevelUp();
	FOnStatChanged OnPlayerLevelChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Experience")
	FORCEINLINE int32 GetXP() const { return XP; }
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void SetXP(int32 NewXP);
	UFUNCTION(BlueprintCallable, Category = "Experience")
	void AddXP(int32 Amount);
	FOnStatChanged OnXPChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Attribute Points")
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	UFUNCTION(BlueprintCallable, Category = "Attribute Points")
	void AddAttributePoints(int32 Amount);
	FOnStatChanged OnAttributePointsChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Spell Points")
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	UFUNCTION(BlueprintCallable, Category = "Spell Points")
	void AddSpellPoints(int32 Amount);
	FOnStatChanged OnSpellPointsChanged;
	
	/** Data Asset containing info about each Level Up. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level Data")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;


protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_PlayerLevel)
	int32 PlayerLevel = 1;
	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_XP)
	int32 XP;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_SpellPoints)
	int32 SpellPoints = 2;
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldAttributePoints);

};
