#include "Typedefs.h"

#ifndef MATHPHYSICS_CORE
#define MATHPHYSICS_CORE

class Vector2D
{
public:

  Vector2D();
	Vector2D( const Vector2D& ) ;	// copy constructor
	Vector2D( Scalar element[2] ) ; // initialize with an array
	Vector2D( double x, double y ) ;

	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from const vectors

	Scalar Length() ;
	void   Normalize() ;	// modifies the vector to be unit length
	Vector2D Normalized() const ; // returns a new vector that is unit length

	// Scalar-Vector product
	Vector2D operator  - ( void ) const ; // return negative of a vector
	void     operator *= ( Scalar ) ;
	void     operator /= ( Scalar ) ;
	Vector2D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
	Vector2D operator / ( Scalar ) const ; // for the case when the operand order is Vector / Scalar
	friend Vector2D operator * ( Scalar, Vector2D& ) ; // for the case when the operand order is Scalar * Vector

	// vector addition
	Vector2D operator +  ( Vector2D& ) const ;
	Vector2D operator -  ( Vector2D& ) const ;
	void     operator += ( Vector2D& ) ;
	void     operator -= ( Vector2D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector2D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector2D Orthogonal() const ;

	bool     operator == ( const Vector2D& ) const ;
	bool     operator != ( const Vector2D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector2D& Zero_Vector() ;


protected:
	static const int dimension = 2 ;
	Scalar v[dimension] ;
};

class Vector3D
{
public:

	Vector3D();
	Vector3D( const Vector3D& ) ;	// copy constructor
	Vector3D( Scalar element[3] ) ; // initialize with an array
	Vector3D( double x, double y, double z ) ;

	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from const vectors

	Scalar Length() const ; 
	void   Normalize() ;
	Vector3D Normalized() const ; // return a new unit vector

	// Scalar-Vector product
	Vector3D operator  - ( void ) const ; // negate a vector
	Vector3D operator * ( Scalar ) const ; // for the case when the operand order is Vector * Scalar
friend
	Vector3D operator * ( Scalar, Vector3D& ) ; // for the case when the operand order is Scalar * Vector
	Vector3D operator /  ( Scalar ) ;
	void     operator *= ( Scalar ) ;
	void     operator /= ( Scalar ) ;

	// vector addition
	Vector3D operator +  ( Vector3D& ) const ;
	Vector3D operator -  ( Vector3D& ) const ;
	void     operator += ( Vector3D& ) ;
	void     operator -= ( Vector3D& ) ;

	// the Dot-Product
	Scalar   operator *  ( Vector3D& ) const ;
	// Construct a vector that is orthogonal (perpendicular) to the given vector
	Vector3D Cross_Product( const Vector3D& other ) const ;
	Vector3D operator %   ( const Vector3D& ) const ;	// overload for cross product

	bool     operator == ( const Vector3D& ) const ;
	bool     operator != ( const Vector3D& ) const ;

	// use this to test whethere or not a vector == zero vector
	// it returns a reference to a constant static vector full of 0's
	static const Vector3D& Zero_Vector() ;


protected:
	static const int dimension = 3 ;
	Scalar v[dimension] ;
};

class Vector4D
{
public:

	Vector4D() ;
	Vector4D( const Vector4D& ) ;	// copy constructor
	Vector4D( Scalar element[4] ) ; // initialize with an array
	Vector4D( double x, double y, double z, double w ) ;
	Vector4D( double x, double y, double z ) ;  // w == 1.0 is implicit
	Vector4D( Vector3D, Scalar w ) ; // w == 0.0 or 1.0

	Vector3D ToVector3D() ;			// returns the 1st 3 components as a Vector3D
	Scalar& operator [] ( int index ) ;			// use to both read and write elements, just like a normal array
	Scalar  operator [] ( int index ) const ;	// use to read elements from a const vector


	Scalar Length() const ;
	Scalar Length3D() const ; // of the normal
	void   Normalize() ;
	void   Normalize3D() ; // the normal only
	Vector4D Normalized() const ; // return a new unit vector


	void     operator *= ( Scalar ) ;
	Vector4D operator *  ( Scalar ) const ;
	friend Vector4D operator * ( Scalar, const Vector4D& ) ;
	void     operator /= ( Scalar ) ;
	Vector4D operator /  ( Scalar ) ;


	void     operator += ( const Vector4D& ) ;
	void     operator -= ( const Vector4D& ) ;
	Vector4D operator +  ( const Vector4D& ) const ;
	Vector4D operator -  ( const Vector4D& ) const ;
	Scalar   operator *  ( const Vector4D& ) const ;	// the DOT PRODUCT
	Vector4D operator -  ( void ) const ;	// negative sign

	Vector4D Cross_Product( const Vector4D& ) const ;
	Vector4D operator %   ( const Vector4D& ) const ;	// overload for cross product
	Vector3D operator %   ( const Vector3D& ) const ;	// cross product with a Vector3D
	friend Vector4D Cross_Product( Vector4D, Vector4D ) ;

	bool     operator == ( const Vector4D& ) const ;
	bool     operator != ( const Vector4D& ) const ;

	static Vector4D& Zero_Vector() ;

protected:
	static const int dimension = 4 ;
	Scalar v[dimension] ;
};

class Matrix2D
{
public:
	Matrix2D(void) ;						// set to the zero matrix
	Matrix2D( Vector2D Bx, Vector2D By ) ;  // set mtrix columns to these 2 basis vectors

	void Identity() ;						// set to the identity matrix
	void Scaling( Vector2D scaling ) ;		// set to a scaling matrix
	void Rotation( double angle ) ;			// set to a rotation matrix 
	void Shearing( Vector2D shearing ) ;	// set to a shear matrix 

	void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	void Scale( Vector2D scale ) ;			// multiply by a new scaling matrix
	void Rotate( double angle ) ;			// multiply by a new rotation matrix 
	void Shear( Vector2D shear ) ;			// multiply by a new shearing matrix

	Matrix2D operator +  ( const Matrix2D& ) const ;	// matrix addition
	Matrix2D operator -  ( const Matrix2D& ) const ;	// matrix subtraction
	Matrix2D operator *  ( const Matrix2D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix2D& ) ;			// multiply a matrix by another matrix
	Vector2D operator *  ( const Vector2D& ) const ;	// matrix * vector. Used to transform vertices
friend
	void     operator *= ( Vector2D&, const Matrix2D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix2D Inverse() ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place

	// comparisions	
	bool     operator == ( const Matrix2D& ) const ;
	bool     operator != ( const Matrix2D& ) const ;
	static   const Matrix2D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

private:
	static const int dimension = 2 ;
	Vector2D m[dimension];
};

class Matrix3D
{
public:
	Matrix3D(void) ;						// set to the zero matrix
	Matrix3D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set mtrix columns to these 2 basis vectors

	Vector3D Basis( int column ) ;		// get a basis vector (column)

	void Identity() ;						// set to the identity matrix
	void Scaling( Scalar uniform_scale ) ;	// set to a new scaling matrix
	void Scaling( Vector3D scaling ) ;		// set to a scaling matrix
	void Rotation( double angle, int axis ) ;			// set to a rotation matrix 
	void Shearing( Vector2D shearing, int axis ) ;	// set to a shear matrix 

	void Scale( Scalar uniform_scale ) ;	// multiply by a new scaling matrix
	void Scale( Vector3D scale ) ;			// multiply by a new scaling matrix
	void Rotate( double angle, int axis ) ;			// multiply by a new rotation matrix 
	void Shear( Vector2D shear, int axis ) ;			// multiply by a new shearing matrix

	Matrix3D operator +  ( const Matrix3D& ) const ;	// matrix addition
	Matrix3D operator -  ( const Matrix3D& ) const ;	// matrix subtraction
	Matrix3D operator *  ( const Matrix3D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix3D& ) ;			// multiply a matrix by another matrix
	Vector3D operator *  ( const Vector3D& ) const ;	// matrix * vector. Used to transform vertices
friend
	void     operator *= ( Vector3D&, const Matrix3D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix3D Inverse() ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place
	Matrix3D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements

	// comparisions	
	bool     operator == ( const Matrix3D& ) const ;
	bool     operator != ( const Matrix3D& ) const ;
	static   const Matrix3D& Identity_Matrix() ;	// return an identity matrix to be used in comparisions with other matrices

protected:
	Scalar Determinant2D( int row, int col ) const ;

private:
	static const int dimension = 3 ;
	Vector3D m[dimension];
};

class Matrix4D
{
public:
	Matrix4D(void) ;						// set to the zero matrix
	Matrix4D( Vector3D Bx, Vector3D By, Vector3D Bz ) ;  // set matrix columns to these 3 basis vectors; 4th column will be [0 0 0 1]
	Matrix4D( Vector4D Bx, Vector4D By, Vector4D Bz, Vector4D Bw ) ;  // set matrix columns to these basis vectors

	void Identity() ;								// set to the identity matrix
	void Scaling( Scalar uniform_scale ) ;			// set to a scaling matrix
	void Scaling( Vector3D scaling ) ;				// set to a scaling matrix
	void Rotation( double angle, int axis ) ;		// set to a rotation matrix 
	void Shearing( Vector2D shearing, int axis ) ;	// set to a shear matrix 
	void Translation( Vector3D& translation ) ;		// set to a 3D translation matrix 

	void Scale( Scalar uniform_scale ) ;		// multiply by a new scaling matrix
	void Scale( Vector3D scale ) ;				// multiply by a new scaling matrix
	void Rotate( double angle, int axis ) ;		// multiply by a new rotation matrix 
	void Shear( Vector2D shear, int axis ) ;	// multiply by a new shearing matrix
	void Translate( Vector3D& translation ) ;	// multiply by a new 3D translation matrix 
	void Translate( Vector4D& translation ) ;	// multiply by a new 3D translation matrix 

	Matrix4D operator +  ( const Matrix4D& ) const ;	// matrix addition
	Matrix4D operator -  ( const Matrix4D& ) const ;	// matrix subtraction
	void     operator += ( const Matrix4D& )       ;	// matrix addition
	void     operator -= ( const Matrix4D& )       ;	// matrix subtraction
	Matrix4D operator *  ( const Matrix4D& ) const ;	// matrix multiplication
	void     operator *= ( const Matrix4D& ) ;			// multiply a matrix by another matrix

	Vector4D operator *  ( const Vector4D& ) const ;	// matrix * vector. Used to transform vertices
friend
	Vector4D operator * ( Vector4D&, Matrix4D& ) ;		 // vector * matrix ; e.g transforming a plane
friend
	void     operator *= ( Vector4D&, const Matrix4D& ) ;// transposed-vector * matrix . Used to transform normals.

	Scalar   Determinant() const ;
	Matrix4D Inverse() const ; // returns a new matrix that is the inverse of this matrix
	void     Invert() ; // inverts this matrix in place
	Matrix4D Transpose() const ;
	Scalar   Trace() const ;	// sum of diagonal elements
	
	bool     operator == ( const Matrix4D& ) const ;
	bool     operator != ( const Matrix4D& ) const ;
	static   const Matrix4D& Identity_Matrix() ;	// return an identity matrix to be used in comparison with other matrices

protected:
	Scalar Determinant2D( int row, int col ) const ;
	Scalar Determinant3D( int row, int col ) const ;

private:
	static const int dimension = 4 ;
	Vector4D m[dimension] ;
};

class Point 
{
 public:
   static Point rectangular(float x, float y);      // Rectangular coordinates
   static Point polar(float radius, float angle);   // Polar coordinates
};

#endif
