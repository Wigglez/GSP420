#ifndef MATHPHYSICS_CORE
#define MATHPHYSICS_CORE

#include "Typedefs.h"
#include <assert.h>
#include <math.h>

const Double PI = 3.14159265358979323846 ;

// these are to make the code more readable. Use vector[X] instead of vector[0], etc. 
const Int X = 0 ;
const Int Y = 1 ;
const Int Z = 2 ;
const Int W = 3 ;

inline Double Degrees_to_Radians( Double degrees )
{
	return degrees / 180.0 * PI ;
}

inline Double Radians_to_Degrees( Double radians )
{
	return radians * 180.0 / PI ;
}

// Because Floating Point computation contains numerical error, comparing to 0 is not adequate
// Instead, we need to compare to a number close to 0, which we call "epsilon".
// From experience choosing epsilon somewhere between 1.0e-4 and 1.0e-7 is sufficient, but depends on the details of the computation involved.

const Scalar epsilon = Scalar( 1.0e-5 ) ;

// use this instead of comparing to zero; for example, when aVoiding a divide-by-zero
inline Bool Within_Epsilon( Double value )
{
	return value < epsilon && value > -epsilon ;
}

class Vector2D
{
public:

	Vector2D();
	Vector2D( const Vector2D& ) ;	// copy constructor
	Vector2D( Scalar element[2] ) ; // initialize with an array
	Vector2D( Double x, Double y ) ;

	Scalar& operator [] ( Int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( Int index ) const ;	// use to read elements from const vectors

	Scalar Length() ;
	Void   Normalize() ;	// modifies the vector to be unit length
	Vector2D Normalized() const ; // returns a new vector that is unit length

	// Scalar-Vector product
	Vector2D operator  - ( Void ) const ; // return negative of a vector
	Void     operator *= ( Scalar ) ;
	Void     operator /= ( Scalar ) ;
	Vector2D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
	Vector2D operator / ( Scalar ) const ; // for the case when the operand order is Vector / Scalar
	friend Vector2D operator * ( Scalar, Vector2D& ) ; // for the case when the operand order is Scalar * Vector

	// vector addition
	Vector2D operator +  ( Vector2D& ) const ;
	Vector2D operator -  ( Vector2D& ) const ;
	Void     operator += ( Vector2D& ) ;
	Void     operator -= ( Vector2D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector2D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector2D Orthogonal() const ;

	Bool     operator == ( const Vector2D& ) const ;
	Bool     operator != ( const Vector2D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector2D& Zero_Vector() ;


protected:
	static const Int dimension = 2 ;
	Scalar v[dimension] ;
};

class Vector3D
{
public:

	Vector3D();
	Vector3D( const Vector3D& ) ;	// copy constructor
	Vector3D( Scalar element[3] ) ; // initialize with an array
	Vector3D( Double x, Double y, Double z ) ;

	Scalar& operator [] ( Int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( Int index ) const ;	// use to read elements from const vectors

	Scalar Length() const ; 
	Void   Normalize() ;
	Vector3D Normalized() const ; // return a new unit vector

	// Scalar-Vector product
	Vector3D operator  - ( Void ) const ; // negate a vector
	Vector3D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
friend
	Vector3D operator * ( Scalar, Vector3D& ) ; // for the case when the operand order is Scalar * Vector
	Vector3D operator /  ( Scalar ) ;
	Void     operator *= ( Scalar ) ;
	Void     operator /= ( Scalar ) ;

	// vector addition
	Vector3D operator +  ( Vector3D& ) const ;
	Vector3D operator -  ( Vector3D& ) const ;
	Void     operator += ( Vector3D& ) ;
	Void     operator -= ( Vector3D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector3D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector3D Cross_Product( const Vector3D& other ) const ;
	Vector3D operator %   ( const Vector3D& ) const ;	// overload for cross product

	Bool     operator == ( const Vector3D& ) const ;
	Bool     operator != ( const Vector3D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector3D& Zero_Vector() ;


protected:
	static const Int dimension = 3 ;
	Scalar v[dimension] ;
};

class Vector4D
{
public:

	Vector4D() ;
	Vector4D( const Vector4D& ) ;	// copy constructor
	Vector4D( Scalar element[4] ) ; // initialize with an array
	Vector4D( Double x, Double y, Double z, Double w ) ;
	Vector4D( Double x, Double y, Double z ) ;  // w == 1.0 is implicit
	Vector4D( Vector3D, Scalar w ) ; // w == 0.0 or 1.0

	Vector3D ToVector3D() ;			// returns the 1st 3 components as a Vector3D
	Scalar& operator [] ( Int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( Int index ) const ;	// use to read elements from a const vector


	Scalar Length() const ;
	Scalar Length3D() const ; // of the normal
	Void   Normalize() ;
	Void   Normalize3D() ; // the normal only
	Vector4D Normalized() const ; // return a new unit vector


	Void     operator *= ( Scalar ) ;
	Vector4D operator *  ( Scalar ) const ;
	friend Vector4D operator * ( Scalar, const Vector4D& ) ;
	Void     operator /= ( Scalar ) ;
	Vector4D operator /  ( Scalar ) ;


	Void     operator += ( const Vector4D& ) ;
	Void     operator -= ( const Vector4D& ) ;
	Vector4D operator +  ( const Vector4D& ) const ;
	Vector4D operator -  ( const Vector4D& ) const ;
	Scalar   operator *  ( const Vector4D& ) const ;	// the DOT PRODUCT
	Vector4D operator -  ( Void ) const ;	// negative sign

	Vector4D Cross_Product( const Vector4D& ) const ;
	Vector4D operator %   ( const Vector4D& ) const ;	// overload for cross product
	Vector3D operator %   ( const Vector3D& ) const ;	// cross product with a Vector3D
	friend Vector4D Cross_Product( Vector4D, Vector4D ) ;

	Bool     operator == ( const Vector4D& ) const ;
	Bool     operator != ( const Vector4D& ) const ;

	static Vector4D& Zero_Vector() ;

protected:
	static const Int dimension = 4 ;
	Scalar v[dimension] ;
};

class Matrix2D
{
public:
	Matrix2D(Void) ;						// set to the zero matrix
	Matrix2D( Vector2D Bx, Vector2D By ) ;  // set matrix columns to these 2 basis vectors

	Void Identity() ;						// set to the identity matrix
	Void Scaling( Vector2D scaling ) ;		// set to a scaling matrix
	Void Rotation( Double angle ) ;			// set to a rotation matrix 
	Void Shearing( Vector2D shearing ) ;	// set to a shear matrix 

	Void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	Void Scale( Vector2D scale ) ;			// multiply by a new scaling matrix
	Void Rotate( Double angle ) ;			// multiply by a new rotation matrix 
	Void Shear( Vector2D shear ) ;			// multiply by a new shearing matrix

	Matrix2D operator +  ( const Matrix2D& ) const ;	// matrix addition
	Matrix2D operator -  ( const Matrix2D& ) const ;	// matrix subtraction
	Matrix2D operator *  ( const Matrix2D& ) const ;	// matrix multiplication
	Void     operator *= ( const Matrix2D& ) ;			// multiply a matrix by another matrix
	Vector2D operator *  ( const Vector2D& ) const ;	// matrix * vector. Used to transform vertices
friend
	Void     operator *= ( Vector2D&, const Matrix2D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix2D Inverse() ; // returns a new matrix that is the inverse of this matrix
	Void     Invert() ; // inverts this matrix in place

	// comparisions	
	Bool     operator == ( const Matrix2D& ) const ;
	Bool     operator != ( const Matrix2D& ) const ;
	static   const Matrix2D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

private:
	static const Int dimension = 2 ;
	Vector2D m[dimension];
};

class Matrix3D
{
public:
	Matrix3D(Void) ;						// set to the zero matrix
	Matrix3D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set mtrix columns to these 2 basis vectors

	Vector3D Basis( Int column ) ;		// get a basis vector (column)

	Void Identity() ;						// set to the identity matrix
	Void Scaling( Scalar uniform_scale ) ;	// set to a new scaling matrix
	Void Scaling( Vector3D scaling ) ;		// set to a scaling matrix
	Void Rotation( Double angle, Int axis ) ;			// set to a rotation matrix 
	Void Shearing( Vector2D shearing, Int axis ) ;	// set to a shear matrix 

	Void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	Void Scale( Vector3D scale ) ;			// multiply by a new scaling matrix
	Void Rotate( Double angle, Int axis ) ;			// multiply by a new rotation matrix 
	Void Shear( Vector2D shear, Int axis ) ;			// multiply by a new shearing matrix

	Matrix3D operator +  ( const Matrix3D& ) const ;	// matrix addition
	Matrix3D operator -  ( const Matrix3D& ) const ;	// matrix subtraction
	Matrix3D operator *  ( const Matrix3D& ) const ;	// matrix multiplication
	Void     operator *= ( const Matrix3D& ) ;			// multiply a matrix by another matrix
	Vector3D operator *  ( const Vector3D& ) const ;	// matrix * vector. Used to transform vertices
friend
	Void     operator *= ( Vector3D&, const Matrix3D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix3D Inverse() ; // returns a new matrix that is the inverse of this matrix
	Void     Invert() ; // inverts this matrix in place
	Matrix3D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements

	// comparisions	
	Bool     operator == ( const Matrix3D& ) const ;
	Bool     operator != ( const Matrix3D& ) const ;
	static   const Matrix3D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

protected:
	Scalar Determinant2D( Int row, Int col ) const ;

private:
	static const Int dimension = 3 ;
	Vector3D m[dimension];
};

class Matrix4D
{
public:
	Matrix4D(Void) ;						// set to the zero matrix
	Matrix4D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set matrix columns to these 3 basis vectors; 4th column will be [0 0 0 1]
	Matrix4D( Vector4D Bx, Vector4D By, Vector4D Bz, Vector4D Bw ) ;  // set matrix columns to these basis vectors

	Void Identity() ;								// set to the identity matrix
	Void Scaling( Scalar uniform_scale ) ;			// set to a scaling matrix
	Void Scaling( Vector3D scaling ) ;				// set to a scaling matrix
	Void Rotation( Double angle, Int axis ) ;		// set to a rotation matrix 
	Void Shearing( Vector2D shearing, Int axis ) ;	// set to a shear matrix 
	Void Translation( Vector3D& translation ) ;		// set to a 3D translation matrix 

	Void Scale( Scalar uniform_scale ) ;		// multiply by a new scaling matrix
	Void Scale( Vector3D scale ) ;				// multiply by a new scaling matrix
	Void Rotate( Double angle, Int axis ) ;		// multiply by a new rotation matrix 
	Void Shear( Vector2D shear, Int axis ) ;	// multiply by a new shearing matrix
	Void Translate( Vector3D& translation ) ;	// multiply by a new 3D translation matrix 
	Void Translate( Vector4D& translation ) ;	// multiply by a new 3D translation matrix 

	Matrix4D operator +  ( const Matrix4D& ) const ;	// matrix addition
	Matrix4D operator -  ( const Matrix4D& ) const ;	// matrix subtraction
	Void     operator += ( const Matrix4D& )       ;	// matrix addition
	Void     operator -= ( const Matrix4D& )       ;	// matrix subtraction
	Matrix4D operator *  ( const Matrix4D& ) const ;	// matrix multiplication
	Void     operator *= ( const Matrix4D& ) ;			// multiply a matrix by another matrix

	Vector4D operator *  ( const Vector4D& ) const ;	// matrix * vector. Used to transform vertices
friend
	Vector4D operator * ( Vector4D&, Matrix4D& ) ;		 // vector * matrix ; e.g transforming a plane
friend
	Void     operator *= ( Vector4D&, const Matrix4D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix4D Inverse() const ; // returns a new matrix that is the inverse of this matrix
	Void     Invert() ; // inverts this matrix in place
	Matrix4D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements
	
	Bool     operator == ( const Matrix4D& ) const ;
	Bool     operator != ( const Matrix4D& ) const ;
	static   const Matrix4D& Identity_Matrix() ;	// return an identity matrix to be used in comparison with other matrices

protected:
	Scalar Determinant2D( Int row, Int col ) const ;
	Scalar Determinant3D( Int row, Int col ) const ;

private:
	static const Int dimension = 4 ;
	Vector4D m[dimension] ;
};

class Point {
 public:
   static Point rectangular(Float x, Float y);      // Rectangular coord's
   static Point polar(Float radius, Float angle);   // Polar coordinates

 private:
   Point(Float x, Float y);     // Rectangular coordinates
   Float x_, y_;
 };

#endif