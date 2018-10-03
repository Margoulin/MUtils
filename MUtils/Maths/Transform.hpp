#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Quaternion.hpp"

class MTransform
{
public:
	MTransform();
	MTransform(Vector3F const& posValue, Quaternion const& rotValue, Vector3F const& scaleValue = Vector3F::one);
	MTransform(Matrix4x4F const& transformationMatrix);
	~MTransform() = default;

	auto	Translate(Vector3F const& value) -> void;
	auto	Rotate(Quaternion const& value) -> void;
	auto	Scale(Vector3F const& value) -> void;

	auto	SetPosition(Vector3F const& value) -> void;
	auto	SetScale(Vector3F const& value) -> void;
	auto	SetRotation(Vector3F const& value) -> void;
	auto	SetRotation(Quaternion const& value) -> void;

	auto	GetLocalMatrix() -> Matrix4x4F const&;
	auto	GetPosition() const -> Vector3F { return position; }
	auto	GetScale() const -> Vector3F { return scale; }
	auto	GetEulerRotation() const -> Vector3F { return rotation.GetEulerAngles(); }
	auto	GetRotation() const -> Quaternion { return rotation; }

	auto	operator=(const MTransform&) -> MTransform&;

protected:
	Matrix4x4F	localMatrix;
	Quaternion	rotation;
	Vector3F	position;
	Vector3F	scale;
	bool		localMatrixDirty = true;
};


#endif /*__TRANSFORM_HPP__*/