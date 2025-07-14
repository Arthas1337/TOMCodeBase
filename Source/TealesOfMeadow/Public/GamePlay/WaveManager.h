#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Logging/LogMacros.h"
#include "WaveManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWaveManger, Log, All);

class ATOMEnemy;

USTRUCT(BlueprintType)
struct FWaveInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<TSubclassOf<ATOMEnemy>> EnemyClasses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<int32> EnemyCounts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<AActor*> SpawnPoints;

    UPROPERTY(EditAnywhere) 
    bool bSpawnAllAtOnce = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    float SpawnInterval = 1.0f;
};

UCLASS()
class TEALESOFMEADOW_API AWaveManager : public AActor
{
	GENERATED_BODY()
	
public:	

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Waves")
    TArray<FWaveInfo> Waves;

    UFUNCTION(BlueprintCallable)
    void StartWave(int32 WaveIndex);

private:

    void SpawnNextEnemy();

    UFUNCTION()
    void OnEnemyDead();

    void EndWave();
    void AdvanceWave();

    int32 CurrentWaveIndex = 0;
    int32 EnemiesToSpawn = 0;
    int32 AliveEnemies = 0;
    int32 CurrentCountEnemyToSpawn = 0;

    TArray<int32> RemainingPerType;
    int32 NextEnemyTypeIndex = 0;

    FTimerHandle WaveTimerHandle;
};
