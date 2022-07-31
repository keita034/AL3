#include "MyMath.h"
namespace MyMath
{
	using namespace MathUtility;

	float ChangeRadi(const float& angle)
	{
		float radian = angle * PI / 180;
		return radian;
	}

	float ChangeDira(const float& angle)
	{
		float radian = angle * 180 / PI;
		return radian;
	}

	Matrix4 IdentityMatrix()
	{
		Matrix4 matIdentity = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

		return matIdentity;
	}

	void ScalingForm(Matrix4& matScal, Vector3& scale)
	{

		//スケーリング倍率を行列に設定する
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
	}

	Matrix4 ScalingForm(Vector3& scale)
	{
		Matrix4 matScal = IdentityMatrix();
		//スケーリング倍率を行列に設定する
		matScal.m[0][0] = scale.x;
		matScal.m[1][1] = scale.y;
		matScal.m[2][2] = scale.z;
		return matScal;
	}

	void RotationXForm(Matrix4& matRotX, float angle)
	{
		// X軸回転行列の各要素を設定する
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationXForm(float angle)
	{
		Matrix4 matRotX = IdentityMatrix();

		// X軸回転行列の各要素を設定する
		matRotX.m[1][1] = std::cos(angle);
		matRotX.m[1][2] = std::sin(angle);

		matRotX.m[2][1] = -std::sin(angle);
		matRotX.m[2][2] = std::cos(angle);

		return matRotX;
	}

	void RotationYForm(Matrix4& matRotY, float angle)
	{
		// Y軸回転行列の各要素を設定する
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);
	}

	Matrix4 RotationYForm(float angle)
	{
		Matrix4 matRotY = IdentityMatrix();

		// Y軸回転行列の各要素を設定する
		matRotY.m[0][0] = std::cos(angle);
		matRotY.m[0][2] = -std::sin(angle);

		matRotY.m[2][0] = std::sin(angle);
		matRotY.m[2][2] = std::cos(angle);

		return matRotY;
	}

	void RotationZForm(Matrix4& matRotZ, float angle)
	{
		// Z軸回転行列の各要素を設定する
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);
	}

	Matrix4 RotationZForm(float angle)
	{
		Matrix4 matRotZ = IdentityMatrix();
		// Z軸回転行列の各要素を設定する
		matRotZ.m[0][0] = std::cos(angle);
		matRotZ.m[0][1] = std::sin(angle);

		matRotZ.m[1][0] = -std::sin(angle);
		matRotZ.m[1][1] = std::cos(angle);

		return matRotZ;
	}

	void RotationForm(Matrix4& matRot, Vector3& rotaion)
	{
		// matWorld_にZ軸回転行列を掛け算
		matRot *= RotationZForm(rotaion.z);
		// matWorld_にX軸回転行列を掛け算
		matRot *= RotationXForm(rotaion.x);
		// matWorld_にY軸回転行列を掛け算
		matRot *= RotationYForm(rotaion.y);
	}

	Matrix4 RotationForm(Vector3& rotaion)
	{
		Matrix4 matRot = IdentityMatrix();

		// matWorld_にZ軸回転行列を掛け算
		matRot *= RotationZForm(rotaion.z);
		// matWorld_にX軸回転行列を掛け算
		matRot *= RotationXForm(rotaion.x);
		// matWorld_にY軸回転行列を掛け算
		matRot *= RotationYForm(rotaion.y);

		return matRot;
	}

	void TransferForm(Matrix4& matTrans, Vector3& trans)
	{
		//移動量を行列に設定する
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;
	}

	Matrix4 TransferForm(Vector3& trans)
	{
		//平行移動行列を宣言
		Matrix4 matTrans = IdentityMatrix();

		//移動量を行列に設定する
		matTrans.m[3][0] = trans.x;
		matTrans.m[3][1] = trans.y;
		matTrans.m[3][2] = trans.z;

		return matTrans;
	}

	//ワールド行列を生成
	void WorldForm(Matrix4& matWorld, WorldTransform& worldTransform)
	{
		// matWorld_にスケーリング倍率を掛け算
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_にZ軸回転行列を掛け算
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_に移動量を掛け算
		matWorld *= TransferForm(worldTransform.translation_);
	}

	Matrix4 WorldForm(WorldTransform& worldTransform)
	{
		Matrix4 matWorld = IdentityMatrix();

		// matWorld_にスケーリング倍率を掛け算
		matWorld *= ScalingForm(worldTransform.scale_);

		// matWorld_に回転行列を掛け算
		matWorld *= RotationForm(worldTransform.rotation_);

		// matWorld_に移動量を掛け算
		matWorld *= TransferForm(worldTransform.translation_);

		return matWorld;
	}

	void AffineTransformation(WorldTransform& worldTransform)
	{
		//単位行列を代入
		worldTransform.matWorld_ = IdentityMatrix();

		// matWorld_にスケーリング倍率を掛け算
		worldTransform.matWorld_ *= ScalingForm(worldTransform.scale_);

		// matWorld_に回転行列を掛け算
		worldTransform.matWorld_ *= RotationForm(worldTransform.rotation_);

		// matWorld_に移動量を掛け算
		worldTransform.matWorld_ *= TransferForm(worldTransform.translation_);

		//行列の転送
		worldTransform.TransferMatrix();
	}

	void ParenChildUpdate(WorldTransform& worldTransform)
	{
		worldTransform.matWorld_ = WorldForm(worldTransform);

		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;

		worldTransform.TransferMatrix();
	}

	Vector3 VecMatMul(Vector3& vec, Matrix4& mat)
	{
		Vector3 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

		return retVec;
	}

	const Vector3 Vector3Sub(const Vector3& v1, const Vector3& v2)
	{
		Vector3 temp(v1);
		return temp -= v2;
	}

	const Vector3 Vector3Mul(const Vector3& v, float s)
	{
		Vector3 temp(v);
		temp.x *= s;
		temp.y *= s;
		temp.z *= s;
		return temp;
	}

	float Vector3Length(Vector3 vec)
	{
		return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	Vector3& Vector3Normalize(Vector3& vec)
	{
		float len = Vector3Length(vec);

		if (len != 0)
		{
			return vec /= len;
		}

		return vec;
	}

	const Vector3 Vector3Add(const Vector3& v1, const Vector3& v2)
	{
		Vector3 tmp(v1);
		tmp.x += v2.x;
		tmp.y += v2.y;
		tmp.z += v2.z;

		return tmp;
	}

	const Vector4 Vector4div(const Vector4& v1, float s)
	{
		Vector4 tmp(v1);
		tmp.x /= s;
		tmp.y /= s;
		tmp.z /= s;
		tmp.w /= s;

		return tmp;

	}

	Vector3 VecMatMatMulWdiv(Vector3& vec, Matrix4& mat)
	{
		Vector4 retVec = {};

		retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + 1 * mat.m[3][0];

		retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + 1 * mat.m[3][1];

		retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + 1 * mat.m[3][2];

		retVec.w = vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + 1 * mat.m[3][3];

		retVec = Vector4div(retVec , retVec.w);

		return { retVec.x, retVec.y, retVec.z };
	}

	//ワールド座標を取得
	Vector3 GetWorldPosition(WorldTransform& worldTransform_)
	{
		//ワールド座標を入れる変数
		Vector3 worldPos;
		//ワールド行列の平行移動成分を取得(ワールド座標)
		worldPos.x = worldTransform_.matWorld_.m[3][0];
		worldPos.y = worldTransform_.matWorld_.m[3][1];
		worldPos.z = worldTransform_.matWorld_.m[3][2];

		return worldPos;
	}

	Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
	{
		Vector3 temp;
		temp.x = v1.x + (v2.x - v1.x) * t;
		temp.y = v1.y + (v2.y - v1.y) * t;
		temp.z = v1.z + (v2.z - v1.z) * t;

		return temp;
	}

	Vector3 Slerp(Vector3& v1, Vector3& v2, float t)
	{
		float omega = std::acosf(MyMath::Vector3Dot(
			MyMath::Vector3Normalize(v1),
			MyMath::Vector3Normalize(v2)
		));

		float sinOmega = std::sinf(omega);

		Vector3 termOne = v1 * (std::sinf(omega * (1.0f - t)) / sinOmega);
		Vector3 termTwo = v2 * (std::sinf(omega * (t)) / sinOmega);

		return termOne + termTwo;
	}

	Vector3 HermiteGetPoint(Vector3 p0, Vector3 p1, Vector3 v0, Vector3 v1, float t)
	{
		Vector3 c0 = 2.0f * p0 + -2.0f * p1 + v0 + v1;
		Vector3 c1 = -3.0f * p0 + 3.0f * p1 + -2.0f * v0 - v1;
		Vector3 c2 = v0;
		Vector3 c3 = p0;

		float t2 = t * t;
		float t3 = t2 * t;
		return c0 * t3 + c1 * t2 + c2 * t + c3;
	}

	Vector3 CatmullRomSpline(std::vector<Vector3>& points, float t)
	{

		float length = points.size();
		float progress = (length - 1) * t;
		float index = std::floor(progress);
		float weight = progress - index;

		if (Approximately(weight, 0.0f) && index >= length - 1)
		{
			index = length - 2;
			weight = 1;
		}

		Vector3 p0 = points[index];
		Vector3 p1 = points[index + 1];
		Vector3 p2;
		Vector3 p3;

		if (index > 0)
		{
			p2 = 0.5f * (points[index + 1] - points[index - 1]);
		}
		else
		{
			p2 = points[index + 1] - points[index];
		}

		if (index < length - 2)
		{
			p3 = 0.5f * (points[index + 2] - points[index]);
		}
		else
		{
			p3 = points[index + 1] - points[index];
		}

		return HermiteGetPoint(p0, p1, p2, p3, weight);
	}

	bool Approximately(float a, float b)
	{
		float tmp = 1e-06f * Max(std::fabs(a), std::fabs(b));

		float tmp2 = MyMath::EPSILON * 8.0f;

		if (std::fabs(b - a) < Max(tmp, tmp2))
		{
			return true;
		}

		return false;
	}

	float Max(float a, float b)
	{
		return (a > b) ? a : b;
	}

} // namespace MyMath

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
	return m1;
}
