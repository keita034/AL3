#pragma once
#include"BaseEnemyState.h"

class Enemy;

class EnemyStateApproach : public BaseEnemyState
{
	void Update(Enemy* pEnemy)override;
};

