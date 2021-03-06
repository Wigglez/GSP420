#include "Typedefs.h"
#include <assert.h>
#include "CoreMathPhysics.h"

Matrix4D::Matrix4D(Void)
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = (Scalar)0.0 ;
}

Matrix4D:: Matrix4D( Vector3D Bx, Vector3D By, Vector3D Bz )
{
	Identity() ;
	for ( Int row = 0 ; row < dimension-1 ; row++ ) m[row][X] = Bx[row] ;
	for ( Int row = 0 ; row < dimension-1 ; row++ ) m[row][Y] = By[row] ;
	for ( Int row = 0 ; row < dimension-1 ; row++ ) m[row][Z] = Bz[row] ;
}

Matrix4D:: Matrix4D( Vector4D Bx, Vector4D By, Vector4D Bz, Vector4D Bw )
{
	Identity() ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][X] = Bx[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][Y] = By[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][Z] = Bz[row] ;
	for ( Int row = 0 ; row < dimension ; row++ ) m[row][W] = Bw[row] ;
}

Void Matrix4D::Identity()
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] = ( row == col ) ? (Scalar)1.0 : (Scalar)0.0 ;
}

Void Matrix4D::Scaling( Scalar uniform_scale )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension-1 ; i++ )
		m[i][i] = uniform_scale ;
}

Void Matrix4D::Scaling( Vector3D scaling )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension-1 ; i++ )
		m[i][i] = scaling[i] ;
}

Void Matrix4D::Scale( Scalar uniform_scale )
{
	Matrix4D scaling_matrix ;
	scaling_matrix.Scaling( Vector3D( uniform_scale, uniform_scale, uniform_scale ) ) ;
	*this *= scaling_matrix ;
}

Void Matrix4D::Scale( Vector3D scaling )
{
	Matrix4D scaling_matrix ;
	scaling_matrix.Scaling( scaling ) ;
	*this *= scaling_matrix ;
}

Void Matrix4D::Rotation( Double radians, Int axis )
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

Void Matrix4D::Rotate( Double angle, Int axis )
{
	Matrix4D rotation_matrix ;
	rotation_matrix.Rotation( angle, axis ) ;
	*this *= rotation_matrix ;
}

Void Matrix4D::Shearing( Vector2D shearing, Int axis )
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

Void Matrix4D::Shear( Vector2D shear, Int axis )
{
	Matrix4D shearing_matrix ;
	shearing_matrix.Shearing( shear, axis ) ;
	*this *= shearing_matrix ;
}

Void Matrix4D::Translation( Vector3D& translation )
{
	Identity() ;
	for ( Int i = 0 ; i < dimension-1 ; i++ )
		m[i][W] = translation[i] ;
}

Void Matrix4D::Translate( Vector3D& translation )
{
	Matrix4D translation_matrix ;
	translation_matrix.Translation( translation ) ;
	*this *= translation_matrix ;
}

Void Matrix4D::Translate( Vector4D& translation )
{
	Translate( Vector3D( translation[X], translation[Y], translation[Z] ) ) ;
}

Matrix4D Matrix4D:: operator + ( const Matrix4D& other ) const
{
	Matrix4D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] + other.m[row][col] ;

	return result ;
}

Void Matrix4D:: operator += ( const Matrix4D& other )
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] += other.m[row][col] ;
}

Matrix4D Matrix4D:: operator - ( const Matrix4D& other ) const
{
	Matrix4D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[row][col] = m[row][col] - other.m[row][col] ;

	return result ;
}

Void Matrix4D:: operator -= ( const Matrix4D& other )
{
	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			m[row][col] -= other.m[row][col] ;
}


Matrix4D Matrix4D:: operator * ( const Matrix4D& other ) const
{
	Matrix4D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				result.m[row][col] += m[row][k] * other.m[k][col] ;

	return result ;
}

Void Matrix4D:: operator *= ( const Matrix4D& other )
{
	Matrix4D tmp ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			for ( Int k = 0 ; k < dimension ; k++ )
				tmp.m[row][col] += other.m[row][k] * m[k][col] ;

	*this = tmp ;
}

Vector4D Matrix4D:: operator * ( const Vector4D& vector ) const
{
	Vector4D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result[row] += m[row][col] * vector[col] ;

	return result ;
}

Vector4D operator * ( Vector4D& vector, Matrix4D& matrix )
{
	Vector4D result ;

	for ( Int col = 0 ; col < matrix.dimension ; col++ )
		for ( Int row = 0 ; row < matrix.dimension ; row++ )
			result[col] += matrix.m[row][col] * vector[row] ;

	return result ;
}

Void operator *= ( Vector4D& vector, const Matrix4D& matrix )
{
	Vector4D tmp ;

	for ( Int row = 0 ; row < matrix.dimension ; row++ )
		for ( Int col = 0 ; col < matrix.dimension ; col++ )
			tmp[row] += matrix.m[row][col] * vector[col] ;

	vector = tmp ;
}

Matrix4D Matrix4D:: Transpose() const
{
	Matrix4D result ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = m[row][col] ;

	return result ;
}

static Scalar matrix[3][3] ;

Scalar Matrix4D:: Determinant2D( Int row, Int col ) const
{
	Scalar result ;

	Int row1, row2, col1, col2 ;
	row1 = (row+1)%3 ;
	row2 = (row+2)%3 ;
	col1 = (col+1)%3 ;
	col2 = (col+2)%3 ;

	result = matrix[row1][col1] * matrix[row2][col2] - matrix[row1][col2] * matrix[row2][col1] ;

	return result ;
}

Scalar Matrix4D:: Determinant3D( Int row, Int col ) const
{
	Scalar result = 0 ;

	// copy elements not in row and col Into a 3x3 matrix
	for ( Int i = (row+1)%dimension, ii = 0 ; ii < 3 ; i = (i+1)%dimension, ii++ )
		for ( Int j = (col+1)%dimension, jj = 0 ; jj < 3 ; j = (j+1)%dimension, jj++ )
			matrix[ii][jj] = m[i][j] ;

	for ( Int j = 0 ; j < 3 ; j++ )
		result += matrix[0][j] * Determinant2D( 0, j ) ;

	return result ;
}

Scalar Matrix4D:: Determinant() const
{
	Scalar result = 0 ;

	for ( Int row = 0 ; row < dimension-1 ; row++ )
		for ( Int col = 0 ; col < dimension-1 ; col++ )
			matrix[row][col] = m[row][col] ;

	for ( Int col = 0 ; col < dimension-1 ; col++ )
		result += m[0][col] * Determinant2D( 0, col ) ;

	return result ;
}


Matrix4D Matrix4D::Inverse() const
{
	Matrix4D result ;

	Scalar one_over_determinant = 1.0f / Determinant() ;

	for ( Int row = 0 ; row < dimension ; row++ )
		for ( Int col = 0 ; col < dimension ; col++ )
			result.m[col][row] = Determinant3D( row, col ) * ( ( row + col ) % 2 ? -1.0f : 1.0f ) * one_over_determinant ;

	return result ;
}

Void Matrix4D::Invert()
{
	Matrix4D inverse ;

	inverse = Inverse() ;

	*this = inverse ;
}

Scalar Matrix4D:: Trace() const
{
	Scalar result = 0.0 ;

	for ( Int i = 0 ; i < dimension ; i++ )
		result += m[i][i] ;

	return result ;
}

Bool Matrix4D:: operator == ( const Matrix4D& other ) const
{
	Bool result = true ;

	for ( Int i = 0 ; i < dimension ; i++ )
		for ( Int j = 0 ; j < dimension ; j++ )
			result &= m[i][j] == other.m[i][j] ;
	return result ;
}

Bool Matrix4D:: operator != ( const Matrix4D& other ) const
{
	return ! (*this == other) ;
}

const Matrix4D& Matrix4D:: Identity_Matrix()
{
	static Matrix4D identity_matrix ;
	static Bool identity_initialize = false ;

	if ( !identity_initialize ) { identity_matrix.Identity() ; identity_initialize = true ; }

	return identity_matrix ;
}

