#include "Enemy.h"
#include "EnemyStateApproach.h"
#include "GameScene.h"
#include <cassert>

Enemy::~Enemy() {}

// ������
void Enemy::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, const Vector3& position) {
	// NUUL�|�C���^�v�`�F�b�N

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	texturehandle_ = textureHandle;

	model_ = model;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;
	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	state_ = std::make_unique<EnemyStateApproach>();

	//�ڋ߃t�F�[�Y������
	approachPhaseInt();
}

// �X�V����
void Enemy::Update() {

	//�ړ�����
	//�폜
	timedCalls_.remove_if([](std::unique_ptr<TimedCall>& call) { return call->IsFinished(); });

	//�ړ�����
	state_->Update(this);

	//���[���h�s��v�Z
	MyMath::AffineTransformation(worldTransform_);
}

// �`��
void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, texturehandle_);
}

// �e����
void Enemy::Fire() {

	assert(player_);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//�v���C���[�̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�G�L����->���L�����̍����x�N�g�������߂�
	Vector3 velocity = MyMath::Vector3Sub(playerPos, enemyPos);
	//�x�N�g���̐��K��
	velocity = MyMath::Vector3Normalize(velocity);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;

	// �e�𐶐����A������
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_,worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);
	//�e��o�^����
	gameScene_->AddEnemyBullet(newBullet);
}

//�ڋ߃t�F�[�Y������
void Enemy::approachPhaseInt() {

	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind(&Enemy::FireReset, this), kFireInterval));
}

// ���[���h���W������
Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { isDead_ = true; }

float Enemy::GetRadius() { return radius_; }

void Enemy::FireReset() {
	Fire();
	//���˃^�C�������Z�b�g����
	timedCalls_.push_back(std::make_unique<TimedCall>(std::bind(&Enemy::FireReset, this), kFireInterval));
}