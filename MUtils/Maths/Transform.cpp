#include "Transform.hpp"

MTransform::MTransform()
	:scale(1.0f, 1.0f, 1.0f)
{
}

MTransform::MTransform(Vector3F const& posValue, Quaternion const& rotValue, Vector3F const& scaleValue)
{
	position = posValue;
	rotation = rotValue;
	scale = scaleValue;
}
	
MTransform::MTransform(Matrix4x4F const& transformationMatrix)
{
	position.x = transformationMatrix[12];
	position.y = transformationMatrix[13];
	position.z = transformationMatrix[14];
	rotation = Quaternion::MatrixToQuaternion(transformationMatrix);
}

auto	MTransform::Translate(Vector3F const& value) -> void
{ 
	position += value;
	localMatrixDirty = true;
}

auto	MTransform::Rotate(Quaternion const& value) -> void
{ 
	rotation = rotation * value;
	localMatrixDirty = true;
}

auto	MTransform::Scale(Vector3F const& value) -> void
{
	scale = scale * value;
	localMatrixDirty = true;
}

auto	MTransform::SetPosition(Vector3F const& value) -> void
{
	position = value;
	localMatrixDirty = true;
}

auto	MTransform::SetScale(Vector3F const& value) -> void
{
	scale = value;
	localMatrixDirty = true;
}

auto	MTransform::SetRotation(Vector3F const& value) -> void
{
	rotation = Quaternion::Euler(value);
	localMatrixDirty = true;
}

auto	MTransform::SetRotation(Quaternion const& value) -> void
{
	rotation = value;
	localMatrixDirty = true;
}

auto	MTransform::GetLocalMatrix() -> Matrix4x4F const&
{
	if (localMatrixDirty)
	{
		Matrix4x4F	translateMat = Matrix4x4F::Translate(Matrix4x4F::identity, position);
		Matrix4x4F	scaleMat = Matrix4x4F::Scale(Matrix4x4F::identity, scale);
		localMatrix = Matrix4x4F::Mult(translateMat, Matrix4x4F::Mult(Quaternion::QuaternionToMatrix(rotation), scaleMat));
		localMatrixDirty = true;
	}
	return localMatrix;
}

auto	MTransform::operator=(const MTransform& other) -> MTransform&
{
	position = other.position;
	rotation = other.rotation;
	scale = other.scale;
	localMatrixDirty = true;
	return *this;
}