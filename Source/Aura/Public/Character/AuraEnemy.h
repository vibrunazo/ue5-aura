// vib

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

class UOverlayWidgetController;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** End of Enemy Interface */

	/** Combat Interface */
	virtual int32 GetPlayerLevel() override;
	/** End of Combat Interface */
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> HealthBarWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
private:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 PlayerLevel = 1;
};
