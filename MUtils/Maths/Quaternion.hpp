#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include <string>

#include "Vector.hpp"
#include "Matrix.hpp"

class Quaternion
{
public:
	static const	Quaternion	identity;

	Quaternion() = default;
	Quaternion(float x, float y, float z, float w);
	Quaternion(Vector4F values);

	static	auto	Euler(float, float, float) -> Quaternion;
	static	auto	Euler(Vector3F const&) -> Quaternion;

	static	auto	Lerp(Quaternion const& first, Quaternion const& second, float const& t) -> Quaternion;
	static	auto	Slerp(Quaternion const& first, Quaternion const& second, float const& t) -> Quaternion;
	static	auto	Inverse(const Quaternion& value) -> Quaternion;
	static	auto	AngleAxis(float angle, Vector3F const& axis) -> Quaternion;
	static	auto	Dot(Quaternion const&, Quaternion const&) -> float;
	static  auto	MatrixToQuaternion(Matrix4x4F const&) -> Quaternion;
	static	auto	QuaternionToMatrix(Quaternion const&) -> Matrix4x4F;

	auto	Rotate(Vector3F const&) -> void;
	auto	Rotate(float const, float const, float const) -> void;
	auto	Normalize() -> void;
	auto	Normalized() const -> Quaternion;
	auto	IsNormalized() const -> bool;
	auto	ToString() const -> std::string;

	auto	GetEulerAngles() const -> Vector3F;
	
	static auto	GetEulerAngles(Quaternion const& q) -> Vector3F;
	
	auto	GetVectorPart() const -> Vector3F;
	auto	GetConjugate() const -> Quaternion;
	
	auto	Set(float const, float const, float const, float const) -> void;
	auto	Set(Quaternion const&) -> void;

	auto	operator*(Vector3F const&) const -> Vector3F;

	auto	operator*(Quaternion const&) const -> Quaternion;
	auto	operator[](int) const -> float;
	auto	operator!=(Quaternion const&) -> bool;
	auto	operator==(Quaternion const&) -> bool;
	auto	operator=(const Quaternion&) -> void;

	float X = 0.f;//i
	float Y = 0.f;//j
	float Z = 0.f;//k
	float W = 1.f;//real

private:
	auto	getMagnitude() const -> float;	
};

#endif /*__QUATERNION_HPP__*/
