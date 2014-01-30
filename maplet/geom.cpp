
#include "std.h"
#include "geom.h"

const Box INFINITE_BOX( Vector(-256),Vector(256) );

//************************** Box *****************************

void Box::glRender()const{
	static int t[]={
		5,7,6,4,7,3,2,6,3,1,0,2,1,5,4,0,1,3,7,5,4,6,2,0 };

	glBegin( GL_QUADS );
	for( int k=0;k<24;++k ){
		Vector v=corner( t[k] );
		glVertex3fv( &v.x );
	}
	glEnd();
}

Plane Box::plane( int n )const{
	switch( n ){
	case 0:return Plane( Vector( 1,0,0 ),-a.x );	//(+ve X)
	case 1:return Plane( Vector( -1,0,0 ),b.x );	//(-ve X)
	case 2:return Plane( Vector( 0,1,0 ),-a.y );	//(+ve Y)
	case 3:return Plane( Vector( 0,-1,0 ),b.y );	//(-ve Y)
	case 4:return Plane( Vector( 0,0,1 ),-a.z );	//(+ve Z)
	case 5:return Plane( Vector( 0,0,-1 ),b.z );	//(-ve Z)
	}
	return Plane();
}

//************************** Line ****************************

Vector Line::nearest( const Line &t )const{
	Vector n=t.d.cross( d.cross( t.d ) ).normalized();
	return Plane(n,-t.o.dot(n)).intersect(*this);
}


//************************** Plane ***************************

Plane::Plane( const Vector &v0,const Vector &v1,const Vector &v2 ){
	n=(v1-v0).cross(v2-v0);
	if( n.length()<=EPSILON ) bbError( "Illegal plane normal",true );
	n.normalize();
	d=-n.dot(v0);
}

Plane::Plane( const Vector &v0,const Vector &nm ):n(nm.normalized()){
	d=-n.dot(v0);
}

int Plane::majorAxis()const{
	int ax=0;
	float max=fabs(n.x),d;
	if( (d=fabs(n.y))>max ) {ax=1;max=d;}
	return fabs(n.z)>max ? 2 : ax;
}

//************************** Matrix **************************

float Matrix::determinant()const{
	return i.x*(j.y*k.z-j.z*k.y )-i.y*(j.x*k.z-j.z*k.x )+i.z*(j.x*k.y-j.y*k.x );
}

Matrix Matrix::cofactored()const{
	Matrix m;
	m.i.x= (j.y*k.z-j.z*k.y);m.i.y=-(j.x*k.z-j.z*k.x);m.i.z= (j.x*k.y-j.y*k.x);
	m.j.x=-(i.y*k.z-i.z*k.y);m.j.y= (i.x*k.z-i.z*k.x);m.j.z=-(i.x*k.y-i.y*k.x);
	m.k.x= (i.y*j.z-i.z*j.y);m.k.y=-(i.x*j.z-i.z*j.x);m.k.z= (i.x*j.y-i.y*j.x);
	return m;
}

Matrix Matrix::operator~()const{
	return Matrix(
	i.x,j.x,k.x,
	i.y,j.y,k.y,
	i.z,j.z,k.z );
}

Matrix Matrix::operator-()const{
	float t=1.0f/determinant();
	return Matrix(
	 t*(j.y*k.z-j.z*k.y),-t*(i.y*k.z-i.z*k.y), t*(i.y*j.z-i.z*j.y),
	-t*(j.x*k.z-j.z*k.x), t*(i.x*k.z-i.z*k.x),-t*(i.x*j.z-i.z*j.x),
	 t*(j.x*k.y-j.y*k.x),-t*(i.x*k.y-i.y*k.x), t*(i.x*j.y-i.y*j.x) );
}

Vector Matrix::operator*( const Vector &t )const{
	return Vector( 
	i.x*t.x+j.x*t.y+k.x*t.z,
	i.y*t.x+j.y*t.y+k.y*t.z,
	i.z*t.x+j.z*t.y+k.z*t.z );
}

Plane Matrix::operator*( const Plane &t )const{
	Vector p=*this*(t.n*-t.d);
	Vector n=(cofactored()*t.n).normalized();
	return Plane( n,-n.dot(p) );
}

Matrix Matrix::operator*( const Matrix &q )const{
	return Matrix(
	i.x*q.i.x+j.x*q.i.y+k.x*q.i.z,i.y*q.i.x+j.y*q.i.y+k.y*q.i.z,i.z*q.i.x+j.z*q.i.y+k.z*q.i.z,
	i.x*q.j.x+j.x*q.j.y+k.x*q.j.z,i.y*q.j.x+j.y*q.j.y+k.y*q.j.z,i.z*q.j.x+j.z*q.j.y+k.z*q.j.z,
	i.x*q.k.x+j.x*q.k.y+k.x*q.k.z,i.y*q.k.x+j.y*q.k.y+k.y*q.k.z,i.z*q.k.x+j.z*q.k.y+k.z*q.k.z );
}

//************************ Transform  ************************

Transform Transform::operator~()const{
	return Transform( ~m,~m*-v );
}

Transform Transform::operator-()const{
	return Transform( -m,-m*-v );
}

Plane Transform::operator*( const Plane &t )const{
	return m*t+v;
}

Vector Transform::operator*( const Vector &t )const{
	return m*t+v;
}

Transform Transform::operator*( const Transform &t )const{
	return Transform( m*t.m,m*t.v+v );
}

void Transform::glLoadMatrix()const{
	float t[]={
	m.i.x,m.i.y,m.i.z,0,
	m.j.x,m.j.y,m.j.z,0,
	m.k.x,m.k.y,m.k.z,0,
	v.x,v.y,v.z,1 };

	glLoadMatrixf(t);
}

void Transform::glMultMatrix()const{
	float t[]={
	m.i.x,m.i.y,m.i.z,0,
	m.j.x,m.j.y,m.j.z,0,
	m.k.x,m.k.y,m.k.z,0,
	v.x,v.y,v.z,1 };

	glMultMatrixf(t);
}
