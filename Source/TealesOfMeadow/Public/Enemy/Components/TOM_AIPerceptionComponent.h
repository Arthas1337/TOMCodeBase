#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "TOM_AIPerceptionComponent.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UTOM_AIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:

	TObjectPtr<AActor> GetClosestActor() const;
	TObjectPtr<AActor> GetDamageInstigatorActor() const;

	FName FriendlyTagName = "GangMember";

   mutable TArray<AActor*> KnownSeenActors;

};
//-------------------------------------------------------------------------------------------------------------
