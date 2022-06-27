#include"RailCamera.h"
RailCamera::RailCamera()
{
}

RailCamera::~RailCamera()
{
}

void RailCamera::Initialize(const Vector3& position, const Vector3& rotaion)
{
	//ワールドトランスフォームの初期設定
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotaion;

	//ビュープロジェクションの初期化
	//viewProjection_.farZ = 400.0f;
	viewProjection_.Initialize();

}
void RailCamera::Update(Vector3& move, Vector3& rot)
{
	using namespace MathUtility;

	worldTransform_.translation_ += move;
	//worldTransform_.rotation_ += rot;
	worldTransform_.matWorld_ =	MyMath::WorldForm(worldTransform_);

	viewProjection_.eye = MyMath::GetWorldPosition(worldTransform_);
	
	Vector3 forward(0, 0, 1);
	forward = MyMath::VecMatMul(forward, worldTransform_.matWorld_);
	viewProjection_.target = viewProjection_.eye + forward;

	Vector3 up(0, 1, 0);
	up = MyMath::VecMatMul(up, worldTransform_.matWorld_);
	viewProjection_.up = up;
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

ViewProjection RailCamera::GetViewProjection()
{
	return viewProjection_;
}

WorldTransform* RailCamera::GetWorldTransformPtr()
{
	return &worldTransform_;
}
