// -------------------------------------------------------
#ifndef TNODE3D_H
#define TNODE3D_H
// -------------------------------------------------------

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "Constants.h"
#include "TNode2D.h"

template <class T>
class TNode3D
{
public:
	T x, y, z;

public:
	TNode3D(T x0 = 0, T y0 = 0, T z0 = 0) : x(x0), y(y0), z(z0) {} // default constructor with parameters OK
																   // TNode3D(const TNode3D<T> &rhs) : x(rhs[0]), y(rhs[1]), z(rhs[2]) {} // copy constructor, done by compiler OK
																   // ~TNode3D(void){} // destructor, done by compiler OK

																   // TNode3D<T>& operator= (const TNode3D<T> &rhs); // done by compiler OK
	TNode3D<T>& operator= (const T &rhs); // OK

	void Assign(T x0, T y0, T z0);

	TNode3D<T>& operator+=(const TNode3D<T> &rhs);
	TNode3D<T>& operator+=(T d);
	TNode3D<T>& operator-=(const TNode3D<T> &rhs);
	TNode3D<T>& operator-=(T d);
	TNode3D<T>& operator*=(const T &rhs);

	T&	     operator[](int indx);
	const T& operator[](int indx) const; // the 1st const is for return const TNode3D<T>, the 2nd for overload

	TNode3D<T> operator+ (const TNode3D<T> &rhs) const;
	TNode3D<T> operator- (const TNode3D<T> &rhs) const;
	TNode3D<T> operator* (const TNode3D<T> &rhs) const;
	TNode3D<T> operator/ (const TNode3D<T> &rhs) const;
	TNode3D<T> operator+ (const T &t) const;
	TNode3D<T> operator- (const T &t) const;
	TNode3D<T> operator* (const T &t) const;
	TNode3D<T> operator/ (const T &t) const;

	TNode3D<std::complex<T> > operator* (const std::complex<T> &t) const;
	TNode3D<std::complex<T> > operator+ (const TNode3D<std::complex<T> > &rhs) const;

	bool operator==(const TNode3D<T> &rhs) const; //Only suitable for int
	TNode3D<T> operator-() const;
	TNode3D<T> Cross(const TNode3D<T> &rhs) const; // cross product OK
	TNode3D<std::complex<T> > Cross(const TNode3D<std::complex<T> > &rhs) const; // real cross complex OK

	T Dot(const TNode3D<T> &rhs) const; // real dot real or complex dot complex OK
	std::complex<T> Dot(const TNode3D<std::complex<T> > &rhs) const; // real dot complex OK

	T Sum() const;
	REAL AbsSquare(void) const;
	TNode3D<T> Abs(void) const; // OK
	double Norm(void) const;
	TNode3D<T> Sqrt() const;
	TNode3D<T> Log() const;
	TNode3D<T> Sph2Cart(void) const; // spherical coordinate to Cartesian coordinate OK
	TNode3D<T> Cart2Sph() const;

	T Max(void); // cannot be used by complex
	TNode3D<int> Conv2Int(void); // cannot be used by complex
	bool InUnit(void); // here only for int, <cstdlib>
	TNode2D<T> GetThetaPhiComp(T theta, T phi); // get theta phi components from Cartesian vector OK
};

template <class T> inline T Abs_rd3(const TNode3D<T> &rhs);   //magnitude of real vector
template <class T> inline T Abs_cd3(const TNode3D<std::complex<T> > &rhs); //magnitude of complex vector 

																		   //=================================================================
																		   //-----------------------------------------------------------------
																		   // template <class T>
																		   // inline TNode3D<T>& TNode3D<T>::operator= (const TNode3D<T> &rhs)
																		   // {
																		   //	x = rhs[0];	y = rhs[1];	z = rhs[2];
																		   //	return *this;
																		   // }
																		   //-----------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator= (const T &rhs) {
	x = rhs;	y = rhs;	z = rhs;
	return *this;
}
// ---------------------------------------------------------------
template <class T>
inline void TNode3D<T>::Assign(T x0, T y0, T z0) {
	x = x0;		y = y0;		z = z0;
	return;
}
// ---------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator+=(const TNode3D<T> &rhs) {
	x += rhs[0];	y += rhs[1];	z += rhs[2];
	return *this;
}
// ---------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator +=(T d) {
	x += d;		y += d;		z += d;
	return *this;
}
// ---------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator-=(const TNode3D<T> &rhs) {
	x -= rhs[0];	y -= rhs[1];	z -= rhs[2];
	return *this;
}
// ---------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator -=(T d) {
	x -= d;		y -= d;		z -= d;
	return *this;
}
// ---------------------------------------------------------------
template <class T>
inline TNode3D<T>& TNode3D<T>::operator*=(const T &rhs) {
	x *= rhs;	y *= rhs;	z *= rhs;
	return *this;
}
// -----------------------------------------------------------
template <class T>
inline T& TNode3D<T>::operator[](int indx) {
	return (indx == 0) ? x : ((indx == 1) ? y : z);
}
// ----------------------------------------------------------------
template <class T>
inline const T& TNode3D<T>::operator[](int indx)const {
	return (indx == 0) ? x : ((indx == 1) ? y : z);
}
// -----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator+ (const TNode3D<T> &rhs) const
{
	return TNode3D<T>(x + rhs.x, y + rhs.y, z + rhs.z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator- (const TNode3D<T> &rhs) const
{
	return TNode3D<T>(x - rhs.x, y - rhs.y, z - rhs.z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator* (const TNode3D<T> &rhs) const
{
	return TNode3D<T>(x*rhs.x, y*rhs.y, z*rhs.z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator/ (const TNode3D<T> &rhs) const
{
	return TNode3D<T>(x / rhs.x, y / rhs.y, z / rhs.z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator+ (const T &t) const
{
	return TNode3D<T>(x + t, y + t, z + t);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator- (const T &t) const
{
	return TNode3D<T>(x - t, y - t, z - t);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator* (const T &t) const
{
	return TNode3D<T>(x*t, y*t, z*t);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator/ (const T &t) const
{
	return TNode3D<T>(x / t, y / t, z / t);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<std::complex<T> > TNode3D<T>::operator* (const std::complex<T> &t) const
{
	return TNode3D<std::complex<T> >(x*t, y*t, z*t);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<std::complex<T> > TNode3D<T>::operator+ (const TNode3D<std::complex<T> > &rhs) const
{
	return TNode3D<std::complex<T> >(x + rhs.x, y + rhs.y, z + rhs.z);
}
//------------------------------------------------------------------
template <class T>
inline bool TNode3D<T>::operator==(const TNode3D<T> &rhs) const
{
	if ((x == rhs.x) && (y == rhs.y) && (z == rhs.z))
	{
		return true;
	}
	return false;
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::operator-() const
{
	return TNode3D<T>(-x, -y, -z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Cross(const TNode3D<T> &rhs) const
{
	return TNode3D<T>(y*rhs[2] - z*rhs[1], z*rhs[0] - x*rhs[2], x*rhs[1] - y*rhs[0]);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<std::complex<T> > TNode3D<T>::Cross(const TNode3D<std::complex<T> > &rhs) const
{
	return TNode3D<std::complex<T> >(rhs[2] * y - rhs[1] * z, rhs[0] * z - rhs[2] * x, rhs[1] * x - rhs[0] * y);
}
//-----------------------------------------------------------------
template <class T>
inline T TNode3D<T>::Dot(const TNode3D<T> &rhs) const
{
	return T(x*rhs[0] + y*rhs[1] + z*rhs[2]);
}
//-----------------------------------------------------------------
template <class T>
inline std::complex<T>  TNode3D<T>::Dot(const TNode3D<std::complex<T> > &rhs) const
{
	return std::complex<T>(x*rhs[0] + y*rhs[1] + z*rhs[2]);
}
//-----------------------------------------------------------------
template <class T>
inline T TNode3D<T>::Sum() const
{
	return T(x + y + z);
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Abs(void) const {
	return TNode3D<T>(std::abs(x), std::abs(y), std::abs(z));
}
//-----------------------------------------------------------------
template <class T>
inline double TNode3D<T>::Norm(void) const {
	return sqrt(std::abs(x) + std::abs(y) + std::abs(z));
}
//-----------------------------------------------------------------
template <class T>
inline REAL TNode3D<T>::AbsSquare(void) const {

	REAL temp = 0;
	temp = std::abs(x)*std::abs(x) + std::abs(y)*std::abs(y) + std::abs(z)* std::abs(z);
	return temp;
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Sqrt() const
{
	return TNode3D<T>(std::sqrt(x), std::sqrt(y), std::sqrt(z));
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Log() const
{
	return TNode3D<T>(std::log(x), std::log(y), std::log(z));
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Sph2Cart(void) const
{
	return TNode3D<T>(x*sin(y)*cos(z), x*sin(y)*sin(z), x*cos(y));
}
//-----------------------------------------------------------------
template <class T>
inline TNode3D<T> TNode3D<T>::Cart2Sph() const
{
	TNode3D<T> temp;
	if ((std::abs(x) <= 1.0e-8) && (std::abs(y) <= 1.0e-8) && (std::abs(z) <= 1.0e-8)) // hard coding!
	{
		temp.x = T(0);
		temp.y = T(0);
		temp.z = T(0);
	}

	else
	{
		temp.x = std::sqrt(x*x + y*y + z*z);
		temp.y = acos(z / temp.x);
		if ((std::abs(x) < 1e-8) && (std::abs(y) <1e-8)) // hard coding!
		{
			temp.z = 0.0;
		}
		else
		{
			if (std::abs(y) < 1e-8) // hard coding!
			{
				if (x > 0) { temp.z = 0; }
				else { temp.z = PI; }
			}

			else if (y > 0)
			{
				temp.z = acos(x / std::sqrt(x*x + y*y));
			}
			else
			{
				temp.z = 2.0f*PI - acos(x / std::sqrt(x*x + y*y));
			}
		}
	}

	return temp;
}
// ----------------------------------------------------------------
template <class T>
inline T TNode3D<T>::Max(void) {
	// return (x>y)?std::max(x,z):std::max(y,z);
	return std::max(std::max(x, y), z);
}
// ----------------------------------------------------------------
template <class T>
inline TNode3D<int> TNode3D<T>::Conv2Int(void) {
	return TNode3D<int>(int(x), int(y), int(z));
}
// -----------------------------------------------------------------
template <class T>
inline bool TNode3D<T>::InUnit(void) {
	return ((abs(x) <= 1) && (abs(y) <= 1) && (abs(z) <= 1));
}
//-----------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode3D<T>::GetThetaPhiComp(T theta, T phi) {
	T sinTheta = sin(theta), sinPhi = sin(phi);
	T cosTheta = cos(theta), cosPhi = cos(phi);
	return TNode2D<T>(x*cosTheta*cosPhi + y*cosTheta*sinPhi - z*sinTheta,
		-x*sinPhi + y*cosPhi);
}
// ----------------------------------------------------------------
// ---------------------------------------------------------------- 
template <class T>
inline T Abs_rd3(const TNode3D<T> &rhs) {
	return T(sqrt(rhs[0] * rhs[0] + rhs[1] * rhs[1] + rhs[2] * rhs[2]));
}
// ----------------------------------------------------------------
template <class T>
inline T Abs_cd3(const TNode3D<std::complex<T> > &rhs) {
	return T(sqrt(rhs[0].real()*rhs[0].real() + rhs[1].real()*rhs[1].real() + rhs[2].real()*rhs[2].real() +
		rhs[0].imag()*rhs[0].imag() + rhs[1].imag()*rhs[1].imag() + rhs[2].imag()*rhs[2].imag()));
}
// ----------------------------------------------------------------

// -----------------------------------------------------------------
#endif

