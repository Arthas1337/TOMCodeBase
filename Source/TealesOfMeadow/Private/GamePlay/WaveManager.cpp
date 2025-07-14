#include "GamePlay/WaveManager.h"
#include "TOMEnemy.h"

DEFINE_LOG_CATEGORY(LogWaveManger);

void AWaveManager::StartWave(int32 WaveIndex)
{
    if (!Waves.IsValidIndex(WaveIndex)) return;
    CurrentWaveIndex = WaveIndex;
    const FWaveInfo& Wave = Waves[WaveIndex];

    EnemiesToSpawn = 0;
    for (int32 count : Wave.EnemyCounts)
        EnemiesToSpawn += count;

    RemainingPerType = Wave.EnemyCounts;
    NextEnemyTypeIndex = 0; 
    AliveEnemies = 0;

    SpawnNextEnemy();
}


void AWaveManager::SpawnNextEnemy()
{
    if (EnemiesToSpawn <= 0) return;

    const FWaveInfo& Wave = Waves[CurrentWaveIndex];

    for (int32 i = 0; i < Wave.EnemyCounts[CurrentCountEnemyToSpawn]; ++i)
    {

         while (NextEnemyTypeIndex < RemainingPerType.Num() && RemainingPerType[NextEnemyTypeIndex] <= 0)
         {
             NextEnemyTypeIndex++;
         }

         if (NextEnemyTypeIndex >= RemainingPerType.Num()) return;

         TSubclassOf<ATOMEnemy> EnemyClass = Wave.EnemyClasses[NextEnemyTypeIndex];
         AActor* SpawnPoint = Wave.SpawnPoints.Num() > 0 ? Wave.SpawnPoints[NextEnemyTypeIndex % Wave.SpawnPoints.Num()] : nullptr;
         FTransform SpawnTransform = SpawnPoint->GetActorTransform();

         UE_LOG(LogWaveManger, Display, TEXT("Spawning wave %d, type idx %d"), CurrentWaveIndex, NextEnemyTypeIndex);
         UE_LOG(LogWaveManger, Display, TEXT(" EnemyClass = %s"), *GetNameSafe(EnemyClass));
         UE_LOG(LogWaveManger, Display, TEXT(" SpawnPoint = %s"), *GetNameSafe(SpawnPoint));

         if (!EnemyClass)
         {
             UE_LOG(LogWaveManger, Error, TEXT("  -> EnemyClass is null!"));
             return;
         }
         if (!SpawnPoint)
         {
             UE_LOG(LogWaveManger, Error, TEXT("  -> SpawnPoint is null or array empty!"));
             return;
         }


         FActorSpawnParameters Params;
         Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
         const auto NewEnemy = GetWorld()->SpawnActor<ATOMEnemy>(EnemyClass, SpawnTransform, Params);

         if (NewEnemy)
         {
             AliveEnemies++;
             EnemiesToSpawn--;

             RemainingPerType[NextEnemyTypeIndex]--;

             NewEnemy->OnEnemyDead.AddDynamic(this, &AWaveManager::OnEnemyDead);
         }
         else
         {
             UE_LOG(LogWaveManger, Error, TEXT("  -> SpawnActor returned nullptr!"));
         }
    }

    ++CurrentCountEnemyToSpawn;
}

void AWaveManager::OnEnemyDead()
{
    AliveEnemies--;

    if (EnemiesToSpawn > 0 && AliveEnemies <= 0)
    {
        SpawnNextEnemy();
    }
    else if (AliveEnemies <= 0)
    {
        EndWave();
    }
}

void AWaveManager::EndWave()
{
    GetWorld()->GetTimerManager().SetTimer(WaveTimerHandle, this, &AWaveManager::AdvanceWave, 5.0f, false);
    CurrentCountEnemyToSpawn = 0;
}

void AWaveManager::AdvanceWave()
{
    StartWave(CurrentWaveIndex + 1);
}
