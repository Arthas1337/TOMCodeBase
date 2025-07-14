#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TOMUtility.h"
#include "EnemyAIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

class TEALESOFMEADOW_API IEnemyAIInterface
{
	GENERATED_BODY()
public:

	virtual float GetAttackRadius() const = 0;
	virtual float GetDefendRadius() const = 0;
	virtual void SetMovementSpeed(EMovementSpeed MovementType) = 0;

	virtual bool ReserveAttackToken(int Amount) = 0;
   virtual void ReturnAttackToken(int Amount) = 0;
};
