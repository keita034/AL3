#include "EnemyStateApproach.h"
#include"Enemy.h"
#include"EnemyStateLeave.h"

void EnemyStateApproach::Update(Enemy* pEnemy)
{
	//�w�莞�ԂɒB����
	for (const std::unique_ptr<TimedCall>& call : pEnemy->GetTimedCalls())
	{
		call->Update();
	}

	Vector3 move = pEnemy->GetApproachVelocity();
	//�ړ�(�x�N�g�������Z)
	pEnemy->MoveTranslation(move);
	//����̈ʒu�ɓ��������痣�E
	if (pEnemy->GetWorldTransform().translation_.z < 0.0f)
	{
		pEnemy->ChangeState(new EnemyStateLeave());
	}
}
