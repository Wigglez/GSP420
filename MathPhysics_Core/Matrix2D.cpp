#include "Typedefs.h"
#include <assert.h>
#include "CoreMathPhysics.h"

Matrix2D::Matrix2D(Void)
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = (Scalar)0.0 ;
}

Matrix2D:: Matrix2D( Vector2D Bx, Vector2D By )
{
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][X] = Bx[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][Y] = By[row] ;
}

Void Matrix2D::Identity()
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = ( row == col ) ? (Scalar)1.0 : (Scalar)0.0 ;
}

Void Matrix2D::Scaling( Vector2D scaling )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension ; i++ )
		m[i][i] = scaling[i] ;
}

Void Matrix2D::Scale( Scalar uniform_scale )
{
	Matrix2D scaling_matrix ;
	scaling_matrix.Scaling( Vector2D( uniform_scale, uniform_scale ) ) ;
	*this *= scaling_matrix ;
}

Void Matrix2D::Scale( Vector2D scaling )
{
	Matrix2D scaling_matrix ;
	scaling_matrix.Scaling( scaling ) ;
	*this *= scaling_matrix ;
}

Void Matrix2D::Rotation( Double radians )
{
	Scalar cos = (Scalar) std::cos( radians ) ;
	Scalar sin = (Scalar) std::sin( radians ) ;

	Identity() ;  // not really needed for 2D

	m[0][0] =  cos ;
	m[0][1] = -sin ;
	m[1][0] =  sin ;
	m[1][1] =  cos ;
}

Void Matrix2D::Rotate( Double angle )
{
	Matrix2D rotation_matrix ;
	rotation_matrix.Rotation( angle ) ;
	*this *= rotation_matrix ;
}

Void Matrix2D::Shearing( Vector2D shearing )
{
	Identity() ;
	m[0][1] = shearing[0] ;
	m[1][0] = shearing[1] ;
}

Void Matrix2D::Shear( Vector2D shear )
{
	Matrix2D shearing_matrix ;
	shearing_matrix.Shearing( shear ) ;
	*this *= shearing_matrix ;
}

Matrix2D Matrix2D:: operator + ( const Matrix2D& other ) const
{
	Matrix2D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] + other.m[row][col] ;

	return result ;
}

Matrix2D Matrix2D:: operator - ( const Matrix2D& other ) const
{
	Matrix2D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] - other.m[row][col] ;

	return result ;
}

Matrix2D Matrix2D:: operator * ( const Matrix2D& other ) const
{
	Matrix2D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				result.m[row][col] += other.m[row][k] * m[k][col] ;

	return result ;
}

Void Matrix2D:: operator *= ( const Matrix2D& other )
{
	Matrix2D tmp ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				tmp.m[row][col] += other.m[row][k] * m[k][col] ;

	*this = tmp ;
}

Vector2D Matrix2D:: operator * ( const Vector2D& vector ) const
{
	Vector2D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result[row] += m[row][col] * vector[col] ;

	return result ;
}

Void operator *= ( Vector2D& vector, const Matrix2D& matrix )
{
	Vector2D tmp ;

	for ( Int row = 0 ; row < matrix.dimension ; row++ )
		for ( Int col = 0 ; col < matrix.dimension ; col++ )
			tmp[row] += matrix.m[row][col] * vector[col] ;

	vector = tmp ;
}

Bool Matrix2D:: operator == ( const Matrix2D& other ) const
{
	Bool result = true ;

	for ( Int i = 0 ; i < dimension ; i++ )
		for ( Int j = 0 ; j < dimension ; j++ )
			result &= m[i][j] == other.m[i][j] ;
	return result ;
}

Bool Matrix2D:: operator != ( const Matrix2D& other ) const
{
	return ! (*this == other) ;
}

const Matrix2D& Matrix2D:: Identity_Matrix()
{
	static Matrix2D identity_matrix ;
	static Bool identity_initialize = false ;

	if ( !identity_initialize ) { identity_matrix.Identity() ; identity_initialize = true ; }

	return identity_matrix ;
}



Scalar Matrix2D:: Determinant() const
{
	Scalar result ;

	result = m[X][X] * m[Y][Y] - m[X][Y] * m[Y][X] ;

	return result ;
}

Matrix2D Matrix2D::Inverse() 
{
	Matrix2D result ;

	Scalar determinant_inverse = 1.0f / Determinant() ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = m[(row+1)%2][(col+1)%2] *
								( ( row + col ) % 2 ? -1.0f : 1.0f ) *
								determinant_inverse ;

	return result ;
}

Void Matrix2D::Invert()
{
	Matrix2D inverse ;

	inverse = Inverse() ;

	*this = inverse ;
}