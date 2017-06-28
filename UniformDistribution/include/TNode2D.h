// ---------------------------------------------------------------
#ifndef TNODE2D_H
#define TNODE2D_H
// ---------------------------------------------------------------

#include <cassert>
#include <cmath>
#include "Constants.h"

template <class T>
class TNode2D
{
public:
	T x, y;

public:
	TNode2D(T x0 = 0, T y0 = 0) : x(x0), y(y0) {} // default constructor with parameters OK 
												  // TNode2D(const TNode2D<T> &rhs) : x(rhs[0]), y(rhs[1]) {} // copy constructor, done by compiler OK 
												  // ~TNode2D(void){} // destructor, done by compiler OK

												  // TNode2D<T>& operator= (const TNode2D<T> &rhs); // done by compiler OK
	TNode2D<T>& operator= (const T &rhs); // OK

	void Assign(T x0, T y0); // OK

	T& operator[](int indx);
	const T& operator[](int indx)const; // 1st const for const TNode2D<T> , 2nd for overload
	TNode2D<T>& operator+=(const TNode2D<T> &rhs); // OK

	TNode2D<T> operator+ (const TNode2D<T> &rhs) const;
	//TSpectrum<T> operator- (const TSpectrum<T> &rhs) const;
	TNode2D<T> operator* (const TNode2D<T> &rhs) const;
	//TSpectrum<T> operator/ (const TSpectrum<T> &rhs) const;

	//TSpectrum<T> operator+ (const T &t) const;
	//TSpectrum<T> operator- (const T &t) const;
	TNode2D<T> operator* (const T &t) const;
	TNode2D<T> operator/ (const T &t) const;

	bool operator== (const TNode2D<T> &rhs)const;
	bool operator< (const TNode2D<T> &rhs) const; // inorder to be used in std::map, not used finally OK

	T Dot(const TNode2D<T> &rhs) const;
	std::complex<T> Dot(const TNode2D<std::complex<T> > &rhs) const;

	void SetZero();
	TNode2D<T> Conj(void) const; // OK
	TNode2D<T> ConjMinus2(void) const; // conj of both x and y, then -(y) OK

	TNode2D<T> Sph2Cart() const;
	TNode2D<T> Cart2Sph() const;
};



//=================================================================
// ----------------------------------------------------------------
// template <class T>
// inline TNode2D<T>& TNode2D<T>::operator= (const TNode2D<T> &rhs){
//	x = rhs[0];		y = rhs[1];
//	return *this;
// }
//------------------------------------------------------------------
template <class T>
inline TNode2D<T>& TNode2D<T>::operator= (const T &rhs) {
	x = rhs;	y = rhs;
	return *this;
}
// -----------------------------------------------------------------
template <class T>
inline void TNode2D<T>::Assign(T x0, T y0) {
	x = x0;		y = y0;
	return;
}
// -----------------------------------------------------------------
template <class T>
inline T& TNode2D<T>::operator[](int indx) {
	return (indx == 0) ? x : y;
}
// ------------------------------------------------------------------
template <class T>
inline const T& TNode2D<T>::operator[](int indx) const {
	return (indx == 0) ? x : y;
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T>& TNode2D<T>::operator+= (const TNode2D<T> &rhs) {
	x += rhs[0];	y += rhs[1];
	return *this;
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::operator+ (const TNode2D<T> &rhs) const
{
	return TNode2D<T>(x + rhs.x, y + rhs.y);
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::operator* (const TNode2D<T> &rhs) const
{
	return TNode2D<T>(x*rhs.x, y*rhs.y);
}
//------------------------------------------------------------------

//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::operator* (const T &t) const
{
	return TNode2D<T>(x*t, y*t);
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::operator/ (const T &t) const
{
	return TNode2D<T>(x / t, y / t);
}
//-----------------------------------------------------------------
template <class T>
inline bool TNode2D<T>::operator==(const TNode2D<T> &rhs) const
{
	if (x == rhs.x && y == rhs.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//------------------------------------------------------------------
template <class T>
inline bool TNode2D<T>::operator< (const TNode2D<T>& rhs) const {
	if (x<rhs.x)					return true;

	if (x == rhs.x && y<rhs.y)		return true;
	else							return false;
}
//-----------------------------------------------------------------
template <class T>
inline T TNode2D<T>::Dot(const TNode2D<T> &rhs) const
{
	return T(x*rhs.x + y*rhs.y);
}
//-----------------------------------------------------------------
template <class T>
inline std::complex<T>  TNode2D<T>::Dot(const TNode2D<std::complex<T> > &rhs) const
{
	return std::complex<T>(x*rhs.x + y*rhs.y);
}
//------------------------------------------------------------------
template <class T>
inline void TNode2D<T>::SetZero()
{
	x = 0;
	y = 0;
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::Conj(void) const {
	return TNode2D<T>(std::conj(x), std::conj(y));
}
//------------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::ConjMinus2(void) const {
	return TNode2D<T>(std::conj(x), -std::conj(y));
}
// -----------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::Sph2Cart() const
{
	return TNode2D<T>(x*cos(y), x*sin(y));
}
// -----------------------------------------------------------------
template <class T>
inline TNode2D<T> TNode2D<T>::Cart2Sph() const
{
	TNode2D<T> temp;
	if ((std::abs(x) <= 1.0e-8) && (std::abs(y) <= 1.0e-8)) // hard coding!
	{
		temp.x = T(0);
		temp.y = T(0);
	}

	else
	{
		temp.x = std::sqrt(x*x + y*y);

		if (std::abs(y) < 1e-8) // hard coding!
		{
			if (x > 0) { temp.y = 0; }
			else { temp.y = PI; }
		}

		else if (y > 0)
		{
			temp.y = acos(x / temp.x);
		}
		else
		{
			temp.y = 2.0f*PI - acos(x / temp.x);
		}
	}

	return temp;
}
// -----------------------------------------------------------------
#endif