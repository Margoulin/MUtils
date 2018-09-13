#include "Transform.hpp"

#include <vector>

Transform::Transform()
	:Scale(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(Vector3F const& posValue, Quaternion const& rotValue, Vector3F const& scaleValue)
{
	Position = posValue;
	Rotation = rotValue;
	Scale = scaleValue;
}
	
Transform::Transform(Matrix4x4F const& transformationMatrix)
{
	Position.x = transformationMatrix[12];
	Position.y = transformationMatrix[13];
	Position.z = transformationMatrix[14];
	Rotation = Quaternion::MatrixToQuaternion(transformationMatrix);
}

auto	Transform::GetLocalMatrix() const -> Matrix4x4F
{
	Matrix4x4F	translateMat = Matrix4x4F::Translate(Matrix4x4F::identity, Position);
	Matrix4x4F	scaleMat = Matrix4x4F::Scale(Matrix4x4F::identity, Scale);
	return Matrix4x4F::Mult(translateMat, Matrix4x4F::Mult(Quaternion::QuaternionToMatrix(Rotation), scaleMat));
}

auto	Transform::operator*(Transform const& other) const -> Transform
{
	Quaternion	tempRotation = other.GetQuaternionRotation() * this->Rotation;
	Vector3F	tempScale = other.Scale * this->Scale;
	Vector3F	tempPosition = other.GetQuaternionRotation() * (other.Position * this->Scale) + other.Position;
	return Transform(tempPosition, tempRotation, tempScale);
}

auto	Transform::operator=(const Transform& other) -> Transform&
{
	Position = other.Position;
	Rotation = other.Rotation;
	Scale = other.Scale;
	return *this;
}