

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAttributeMenuWidgetController : public UWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
	
};
