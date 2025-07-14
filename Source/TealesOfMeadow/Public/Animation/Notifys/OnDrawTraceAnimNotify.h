#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "OnDrawTraceAnimNotify.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TEALESOFMEADOW_API UOnDrawTraceAnimNotify : public UAnimNotifyState
{
    GENERATED_BODY()

public:
    virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
//-------------------------------------------------------------------------------------------------------------
