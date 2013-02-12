#include "Typedefs.h"
#include <assert.h>
#include "CoreMathPhysics.h"

Matrix3D::Matrix3D(Void)
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = (Scalar)0.0 ;
}

Matrix3D:: Matrix3D( Vector3D Bx, Vector3D By, Vector3D Bz )
{
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][X] = Bx[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][Y] = By[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][Z] = Bz[row] ;
}

Vector3D Matrix3D:: Basis( Int column )
{
	Vector3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		result[row] = m[row][column] ;

	return result ;
}


Void Matrix3D::Identity()
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = ( row == col ) ? (Scalar)1.0 : (Scalar)0.0 ;
}

Void Matrix3D::Scaling( Scalar uniform_scale )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension ; i++ ) m[i][i] = uniform_scale ;
}

Void Matrix3D::Scaling( Vector3D scaling )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension ; i++ ) m[i][i] = scaling[i] ;
}

Void Matrix3D::Scale( Scalar uniform_scale )
{
	Matrix3D scaling_matrix ;
	scaling_matrix.Scaling( Vector3D( uniform_scale, uniform_scale, uniform_scale ) ) ;
	*this *= scaling_matrix ;
}

Void Matrix3D::Scale( Vector3D scaling )
{
	Matrix3D scaling_matrix ;
	scaling_matrix.Scaling( scaling ) ;
	*this *= scaling_matrix ;
}

Void Matrix3D::Rotation( Double radians, Int axis )
{
	Scalar cos = (Scalar) std::cos( radians ) ;
	Scalar sin = (Scalar) std::sin( radians ) ;

	Identity() ;  // really needed for 3D

	switch ( axis )
	{
		case X:
			m[Y][Y] =  cos ;
			m[Y][Z] = -sin ;
			m[Z][Y] =  sin ;
			m[Z][Z] =  cos ;
			break ;

		case Y:
			m[Z][Z] =  cos ;
			m[Z][X] = -sin ;
			m[X][Z] =  sin ;
			m[X][X] =  cos ;
			break ;

		case Z:
			m[X][X] =  cos ;
			m[X][Y] = -sin ;
			m[Y][X] =  sin ;
			m[Y][Y] =  cos ;
			break ;
		default:
			assert( false ) ;
			break ;
	}
}

Void Matrix3D::Rotate( Double angle, Int axis )
{
	Matrix3D rotation_matrix ;
	rotation_matrix.Rotation( angle, axis ) ;
	*this *= rotation_matrix ;
}

Void Matrix3D::Shearing( Vector2D shearing, Int axis )
{
	Identity() ;
	switch ( axis )
	{
		case Z:
			m[X][Z] = shearing[0] ;
			m[Y][Z] = shearing[1] ;
			break ;
		case Y:
			m[Z][Y] = shearing[0] ;
			m[X][Y] = shearing[1] ;
			break ;
		case X:
			m[Y][X] = shearing[0] ;
			m[Z][X] = shearing[1] ;
			break ;
		default:
			assert( false ) ;
			break ;
	}
}

Void Matrix3D::Shear( Vector2D shear, Int axis )
{
	Matrix3D shearing_matrix ;
	shearing_matrix.Shearing( shear, axis ) ;
	*this *= shearing_matrix ;
}

Matrix3D Matrix3D:: operator + ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] + other.m[row][col] ;

	return result ;
}

Matrix3D Matrix3D:: operator - ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] - other.m[row][col] ;

	return result ;
}

Matrix3D Matrix3D:: operator * ( const Matrix3D& other ) const
{
	Matrix3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				result.m[row][col] += other.m[row][k] * m[k][col] ;

	return result ;
}

Void Matrix3D:: operator *= ( const Matrix3D& other )
{
	Matrix3D tmp ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				tmp.m[row][col] += other.m[row][k] * m[k][col] ;

	*this = tmp ;
}

Vector3D Matrix3D:: operator * ( const Vector3D& vector ) const
{
	Vector3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result[row] += m[row][col] * vector[col] ;

	return result ;
}

Void operator *= ( Vector3D& vector, const Matrix3D& matrix )
{
	Vector3D tmp ;

	for ( Int row = 0 ; row < matrix.dimension ; row++ )
		for ( Int col = 0 ; col < matrix.dimension ; col++ )
			tmp[row] += matrix.m[row][col] * vector[col] ;

	vector = tmp ;
}

Scalar Matrix3D:: Determinant2D( Int row, Int col ) const
{
	Scalar result ;

	Int row1, row2, col1, col2 ;
	row1 = (row+1)%dimension ;
	row2 = (row+2)%dimension ;
	col1 = (col+1)%dimension ;
	col2 = (col+2)%dimension ;

	result = m[row1][col1] * m[row2][col2] - m[row1][col2] * m[row2][col1] ;

	return result ;
}

Scalar Matrix3D:: Determinant() const
{
	Scalar result = 0 ;

	for ( Int col = 0 ; col < dimension ; col++ )
		result += m[0][col] * Determinant2D( 0, col ) ;

	return result ;
}


Matrix3D Matrix3D::Inverse() 
{
	Matrix3D result ;

	Scalar one_over_determinant = 1.0f / Determinant() ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = Determinant2D( row, col ) * one_over_determinant ;

	return result ;
}

Void Matrix3D::Invert()
{
	Matrix3D inverse ;

	inverse = Inverse() ;

	*this = inverse ;
}

Matrix3D Matrix3D:: Transpose() const
{
	Matrix3D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = m[row][col] ;

	return result ;
}

Scalar Matrix3D:: Trace() const
{
	Scalar result = 0.0 ;

	for ( Int i = 0 ; i < dimension ; i++ ) result += m[i][i] ;

	return result ;
}

Bool Matrix3D:: operator == ( const Matrix3D& other ) const
{
	Bool result = true ;

	for ( Int i = 0 ; i < dimension ; i++ )
		for ( Int j = 0 ; j < dimension ; j++ )
			result &= m[i][j] == other.m[i][j] ;
	return result ;
}

Bool Matrix3D:: operator != ( const Matrix3D& other ) const
{
	return ! (*this == other) ;
}

const Matrix3D& Matrix3D:: Identity_Matrix()
{
	static Matrix3D identity_matrix ;
	static Bool identity_initialize = false ;

	if ( !identity_initialize ) { identity_matrix.Identity() ; identity_initialize = true ; }

	return identity_matrix ;
}


