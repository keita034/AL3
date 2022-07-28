#pragma once
#include "DebugText.h"
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "input.h"

#include "BaseEnemyState.h"
#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"
#include"TimedCall.h"

class Player;
// GameScene�̑O���錾
class GameScene;

//�s���t�F�[�Y
enum class Phase {
	Approach, //�ڋ߂���
	Leave,    //���E����

};

/// <summary>
///�G
/// </summary>
class Enemy {
  public:

	  ~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(std::shared_ptr<Model> model,uint32_t textureHandle, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// /// <param name="viewProjection">�r���[�v���W�F�N�V����(�Q�Ɠn��)</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	/// <summary>
	/// �ڋ߃t�F�[�Y������
	/// </summary>
	void approachPhaseInt();

	/// <summary>
	/// �v���C���[�̃A�h���X���Z�b�g
	/// </summary>
	/// <param name="player">�v���C���[�̃A�h���X</param>
	void SetPlayer(std::shared_ptr<Player> player) { player_ = player; }

	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition();

	//�Փ˂����m������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();

	/// <summary>
	/// �Q�[���V�[���̃A�h���X���Z�b�g
	/// </summary>
	/// <param name="gameScene">�Q�[���V�[���̃A�h���X</param>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; };

	///�f�X�t���O�Q�b�^�[
	bool IsDead() const;

	//���ˊԊu
	static const int kFireInterval = 60;

	void ChangeState(BaseEnemyState* baseEnemyState);

	int32_t GetFileTimer();

	void SetFileTimer(int32_t time);

	WorldTransform GetWorldTransform();

	void MoveTranslation(Vector3& move);

	Vector3 GetApproachVelocity();

	Vector3 GetLeaveVelocity();

	/// <summary>
	/// �e�𔭎˂��A�^�C�}�[�����Z�b�g����R�[���o�b�N�֐�
	/// </summary>
	void FireReset();

  private:

	static void (Enemy::*phaseFuncTable[])();

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	std::shared_ptr<Model> model_;

	//�e�N�X�`���n���h��
	uint32_t texturehandle_ = 0u;

	//�L�[�{�[�h����
	Input* input_ = nullptr;

	//�f�o�b�O�p�\��
	DebugText* debugText_ = nullptr;

	//���x
	Vector3 approachVelocity_ = {0, 0, -0.3f};
	Vector3 leaveVelocity_ = {-0.1f, 0.1f, -0.1f};

	//���˃^�C�}�[
	int32_t fileTimer_ = 0;

	//���L����
	std::shared_ptr<Player> player_;

	//���a
	const float radius_ = 1.0f;

	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	std::unique_ptr<BaseEnemyState> state_;

	//���������̃��X�g
	std::list<std::unique_ptr<TimedCall>> timedCalls_;

};

inline bool Enemy::IsDead() const { return isDead_; }

inline void Enemy::ChangeState(BaseEnemyState* baseEnemyState) { state_.reset(baseEnemyState); }

inline int32_t Enemy::GetFileTimer() { return fileTimer_; }

inline void Enemy::SetFileTimer(int32_t time) { fileTimer_ = time; }

inline WorldTransform Enemy::GetWorldTransform() { return worldTransform_; }

inline void Enemy::MoveTranslation(Vector3& move) { worldTransform_.translation_ += move; }

inline Vector3 Enemy::GetApproachVelocity() { return approachVelocity_; }

inline Vector3 Enemy::GetLeaveVelocity() { return leaveVelocity_; }
