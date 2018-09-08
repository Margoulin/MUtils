#include "Quaternion.hpp"
#include "Math.hpp"

const Quaternion Quaternion::identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion(float x, float y, float z, float w)
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

Quaternion::Quaternion(Vector4F values)
{
	X = values.x;
	Y = values.y;
	Z = values.z;
	W = values.w;
}

auto	Quaternion::Euler(float x, float y, float z) -> Quaternion
{
	auto	qY = Quaternion::AngleAxis(y, Vector3F::up);
	auto	qZ = Quaternion::AngleAxis(z, Vector3F::forward);
	auto	qX = Quaternion::AngleAxis(x, Vector3F::right);
	auto	temp = 	qY * qX * qZ;
	return temp;
}

auto	Quaternion::Euler(Vector3F const& value) -> Quaternion
{
	return Quaternion::Euler(value.x, value.y, value.z);
}

auto	Quaternion::Inverse(const Quaternion& value) -> Quaternion
{
	if (value.IsNormalized())
		return value.GetConjugate();
	else
		return value.Normalized().GetConjugate();
}

auto	Quaternion::AngleAxis(float angle, Vector3F const& axis) -> Quaternion
{
	auto	nAxis = axis.Normalized();
	float	sin_angle = Sin(angle / 2.0f);
	auto	temp = Quaternion(nAxis.x * sin_angle, nAxis.y * sin_angle,
		nAxis.z * sin_angle, Cos(angle / 2.0f));
	temp.Normalize();

	return temp;
}

auto	Quaternion::Dot(Quaternion const& first, Quaternion const& second) -> float
{
	return first.W * second.W + first.X * second.X
		+ first.Y * second.Y + first.Z * second.Z;
}

auto	Quaternion::MatrixToQuaternion(Matrix4x4F const& mat) -> Quaternion
{
	float t = mat[0] + mat[5] + mat[10] + 1.0f;

	if (t > 0.0f)
	{
		float s = 0.5f / Sqrt(t);

		float x = (mat[6] - mat[9]) * s;
		float y = (mat[8] - mat[2]) * s;
		float z = (mat[1] - mat[4]) *s;
		float w = 0.25f / s;
		return Quaternion(x, y, z, w);
	}
	else
	{
		int idx = 0;
		float temp = mat[0];
		if (mat[5] > temp)
		{
			temp = mat[5];
			idx = 5;
		}
		if (mat[10] > temp)
		{
			temp = mat[10];
			idx = 10;
		}
		switch (idx)
		{
		default:
		case 0:
		{
			float s = Sqrt(1 + mat[0] - mat[5] - mat[10]) * 2;

			float x = 0.25f * s;
			float y = (mat[4] + mat[1]) / s;
			float z = (mat[8] + mat[2]) / s;
			float w = (mat[9] - mat[6]) / s;
			return Quaternion(x, y, z, w);
		}
		case 5:
		{
			float s = Sqrt(1 - mat[0] + mat[5] - mat[10]) * 2;

			float x = (mat[4] + mat[1]) / s;
			float y = 0.25f * s;
			float z = (mat[9] + mat[6]) / s;
			float w = (mat[8] - mat[2]) / s;
			return Quaternion(x, y, z, w);
		}
		case 10:
		{
			float s = Sqrt(1 - mat[0] - mat[5] + mat[10]) * 2;

			float x = (mat[8] + mat[2]) / s;
			float y = (mat[9] + mat[6]) / s;
			float z = 0.25f * s;
			float w = (mat[4] - mat[1]) / s;
			return Quaternion(x, y, z, w);
		}
		}
	}
}

auto	Quaternion::QuaternionToMatrix(Quaternion const& value) -> Matrix4x4F
{
	//		(1-2YY-2ZZ	2XY-2ZW		2XZ+2YW		)
	//	M = (2XY+2ZW	1-2XX-2ZZ	2YZ-2XW		)
	//		(2XZ-2YW	2YZ+2XW		1-2XX-2YY	)

	Matrix4x4F	ret = Matrix4x4F();

	ret[15] = 1.0f;

	float xx = value.X * value.X;
	float yy = value.Y * value.Y;
	float zz = value.Z * value.Z;
	float xy = value.X * value.Y;
	float zw = value.Z * value.W;
	float xz = value.X * value.Z;
	float yw = value.Y * value.W;
	float yz = value.Y * value.Z;
	float xw = value.X * value.W;

	ret[0] = (1.0f - 2.0f * yy - 2.0f * zz);
	ret[1] = (2.0f * xy + 2.0f * zw);
	ret[2] = (2.0f * xz - 2.0f * yw);
	
	ret[4] = (2.0f * xy - 2.0f * zw);
	ret[5] = (1.0f - 2.0f * xx - 2.0f * zz);
	ret[6] = (2.0f * yz + 2.0f * xw);
	
	ret[8] = (2.0f * xz + 2.0f * yw);
	ret[9] = (2.0f * yz - 2.0f * xw);
	ret[10] = (1.0f - 2.0f * xx - 2.0f * yy);

	return ret;
}

auto	Quaternion::Rotate(Vector3F const& value) -> void
{
	Set(*this * Euler(value));
}

auto	Quaternion::Rotate(float const x, float const y, float const z) -> void
{
	Rotate(Vector3F(x, y, z));
}

auto	Quaternion::Normalize() -> void
{
	float magnitude = getMagnitude();
	X /= magnitude;
	Y /= magnitude;
	Z /= magnitude;
	W /= magnitude;
}

auto	Quaternion::Normalized() const -> Quaternion
{
	float magnitude = getMagnitude();
	return Quaternion(X / magnitude, Y / magnitude, Z / magnitude, W / magnitude);
}

auto	Quaternion::IsNormalized() const -> bool
{
	return AreSame(1.0f, getMagnitude(), 0.00001f);
}

auto	Quaternion::ToString() const -> std::string
{
	return "Quaternion {x: " + std::to_string(X) + ", y: " + std::to_string(Y) + ", z: " + std::to_string(Z) + ", w: " + std::to_string(W) + "}";
}

auto	Quaternion::GetEulerAngles() const -> Vector3F
{
	float xx = X * X;
	float yy = Y * Y;
	float zz = Z * Z;
	float ww = W * W;

	float x = Atan2F((2.0f * (Y * Z + W * X)), (ww - xx - yy + zz));
	float y = ArcSin(-2.0f * (X * Z - W * Y));
	float z = Atan2F(2.0f * (X * Y + W * Z), (ww + xx - yy - zz));

	return Vector3F(x, y, z);
}

auto	Quaternion::GetEulerAngles(Quaternion const& q) -> Vector3F
{
	float xx = q.X *q. X;
	float yy = q.Y *q. Y;
	float zz = q.Z * q.Z;
	float ww = q.W * q.W;

	float x = Atan2F((2.0f * (q.Y * q.Z + q.W * q.X)), (ww - xx - yy + zz));
	float y = ArcSin(-2.0f * (q.X * q.Z - q.W * q.Y));
	float z = Atan2F(2.0f * (q.X * q.Y + q.W * q.Z), (ww + xx - yy - zz));

	return Vector3F(x, y, z);
}

auto	Quaternion::GetVectorPart() const -> Vector3F
{
	return Vector3F(X, Y, Z);
}

auto	Quaternion::GetConjugate() const -> Quaternion
{
	return Quaternion(-X, -Y, -Z, W);
}

/*
auto	Quaternion::GetAngle() const -> float
{
	return ArcCos(W) * 2.0f;
}

auto	Quaternion::GetAxis() const -> Vector3F
{
	float temp = 1.0f - W * W;
	if (temp <= 0.0f)
		return Vector3F(0.0f, 0.0f, 1.0f);
	float temp2 = 1.0f / Sqrt(temp);
	return Vector3F(X * temp2, Y * temp2, Z * temp2);
}
*/

auto	Quaternion::Set(Quaternion const& value) -> void
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = value.W;
}

auto Quaternion::Set(float x, float y, float z, float w) -> void
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

auto	Quaternion::operator*(const Quaternion& value) const -> Quaternion
{
	float	tempW = this->W * value.W - this->X * value.X
		- this->Y * value.Y - this->Z * value.Z;
	float	tempX = value.W * this->X + value.X * this->W
		- value.Y * this->Z + value.Z * this->Y;
	float	tempY = value.W * this->Y + value.X * this->Z
		+ value.Y * this->W - value.Z * this->X;
	float tempZ = value.W * this->Z - value.X * this->Y
		+ value.Y * this->X + value.Z * this->W;

	return Quaternion(tempX, tempY, tempZ, tempW);
}

auto	Quaternion::operator*(const Vector3F& value) const -> Vector3F
{
	auto temp = Vector3F::Cross(this->GetVectorPart(), value) * 2.0f;
	Vector3F ret = value + temp * W + Vector3F::Cross(this->GetVectorPart(), temp);
	return ret;
}

auto	Quaternion::operator[](int value) const -> float
{
	switch (value)
	{
	case 0:
		return X;
	case 1:
		return Y;
	case 2:
		return Z;
	case 3:
		return W;
	default:
		return 0.f;
	}
}

auto	Quaternion::operator!=(Quaternion const& value) -> bool
{
	return (1.0f != Dot(*this, value));
}

auto	Quaternion::operator==(Quaternion const& value) -> bool
{
	return (1.0f == Dot(*this, value));
}

auto	Quaternion::getMagnitude() const -> float
{
	return Sqrt(W * W + X * X + Y * Y + Z * Z);
}

auto	Quaternion::operator=(const Quaternion& value) -> void
{
	X = value.X;
	Y = value.Y;
	Z = value.Z;
	W = value.W;
}