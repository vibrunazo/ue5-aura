// vib


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet *AS = PS->GetAttributeSet();
	const FWidgetControllerParams WidgetControllerParams = FWidgetControllerParams(PC, PS, ASC, AS);
	return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
}
