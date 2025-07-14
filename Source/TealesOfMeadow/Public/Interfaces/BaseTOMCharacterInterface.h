#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TOMUtility.h"
#include "BaseTOMCharacterInterface.generated.h"

//-------------------------------------------------------------------------------------------------------------
UINTERFACE(MinimalAPI)
class UBaseTOMCharacterInterface : public UInterface
{
	GENERATED_BODY()
};
//-------------------------------------------------------------------------------------------------------------
class TEALESOFMEADOW_API IBaseTOMCharacterInterface
{
	GENERATED_BODY()

public:

	virtual void Dodge() = 0;
   virtual bool GetIsDodging() const = 0;
   virtual bool GetIsAttackingPure() const = 0;

	virtual bool ReserveAttackToken(int Amount) = 0;
   virtual void ReturnAttackToken(int Amount) = 0;

	virtual EOverlayStates GetOverlayState() const = 0;
};
//-------------------------------------------------------------------------------------------------------------
