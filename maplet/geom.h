
#ifndef GEOM_H
#define GEOM_H

#include <math.h>

#undef min
#undef max

const float PI=3.14159265359f;				//180 degrees
const float TWOPI=PI*2.0f;					//360 degrees
const float HALFPI=PI*.5f;					//90  degrees
const float QUARTERPI=PI*.25f;				//45  degrees
const float INFINITY=1<<23;					//Big value
const float EPSILON=1/INFINITY;				//Small value

struct Box;

extern const Box INFINITE_BOX;

struct Vector{
	float x,y,z;

	Vector(){}
	Vector( float t ):x(t),y(t),z(t){}
	Vector( float x,float y,float z ):x(x),y(y),z(z){}

	void set( float t ){ x=y=z=t; }
	void set( float _x,float _y,float _z ){ x=_x;y=_y;z=_z; }

	operator float *(){ return &x; }
	operator const float*()const{ return &x; }

	Vector operator-()const{ return Vector( -x,-y,-z ); }

	Vector operator*( float t )const{ return Vector( x*t,y*t,z*t ); }
	Vector operator/( float t )const{ return Vector( x/t,y/t,z/t ); }

	Vector operator+( const Vector &t )const{ return Vector( x+t.x,y+t.y,z+t.z ); }
	Vector operator-( const Vector &t )const{ return Vector( x-t.x,y-t.y,z-t.z ); }
	Vector operator*( const Vector &t )const{ return Vector( x*t.x,y*t.y,z*t.z ); }
	Vector operator/( const Vector &t )const{ return Vector( x/t.x,y/t.y,z/t.z ); }

	Vector &operator*=( float t ){ x*=t;y*=t;z*=t;return *this; }
	Vector &operator/=( float t ){ x/=t;y/=t;z/=t;return *this; }
	Vector &operator+=( const Vector &t ){ x+=t.x;y+=t.y;z+=t.z;return *this; }
	Vector &operator-=( const Vector &t ){ x-=t.x;y-=t.y;z-=t.z;return *this; }
	Vector &operator*=( const Vector &t ){ x*=t.x;y*=t.y;z*=t.z;return *this; }
	Vector &operator/=( const Vector &t ){ x/=t.x;y/=t.y;z/=t.z;return *this; }

	Vector &resize( float d ){ return operator*=(d/length()); }
	Vector &normalize(){ return operator*=(1/length()); }

	float length()const{ return sqrt( x*x+y*y+z*z ); }
	float distance( const Vector &t )const{ return operator-( t ).length(); }
	float dot( const Vector &t )const{ return x*t.x+y*t.y+z*t.z; }
	float yaw()const{ return -atan2( x,z ); }
	float pitch()const{ return -atan2( y,sqrt( x*x+z*z ) ); }
	Vector resized( float d )const{ return (operator*(1/length()))*d; }
	Vector normalized()const{ return operator*(1/length()); }
	Vector cross( const Vector &t )const{ return Vector( y*t.z-z*t.y,z*t.x-x*t.z,x*t.y-y*t.x ); }
	Vector min( const Vector &t )const{ return Vector(x<t.x?x:t.x,y<t.y?y:t.y,z<t.z?z:t.z); }
	Vector max( const Vector &t )const{ return Vector(x>t.x?x:t.x,y>t.y?y:t.y,z>t.z?z:t.z); }
};

struct Plane;

struct Box{
	Vector a,b;

	Box(){}
	Box( const Vector &t ):a(t),b(t){}
	Box( const Vector &q,const Vector &t):a(q),b(q){update(t);}

	void update( const Vector &t ){ a=a.min(t);b=b.max(t); }

	float width()const{ return b.x-a.x; }
	float height()const{ return b.y-a.y; }
	float depth()const{ return b.z-a.z; }
	float size( int axis )const{ return b[axis]-a[axis]; }
	float volume()const{ return width()*height()*depth(); }

	Plane plane( int axis )const;
	Vector centre()const{ return (a+b)/2; }
	Vector corner( int n )const{ return Vector(((n&1)?b:a).x,((n&2)?b:a).y,((n&4)?b:a).z); }

	void glRender()const;
};

struct Line{
	Vector o,d;

	Line(){}
	Line( const Vector &o,const Vector &d ):o(o),d(d){}

	Line operator-()const{ return Line(o+d,-d); }

	Vector operator[]( float t )const{ return o+d*t; }

	Line &normalize(){ d.normalize();return *this; }
	Line normalized()const{ return Line(o,d.normalized()); }

	Vector nearest( const Line &t )const;
	Vector nearest( const Vector &q )const{	return o+d*(d.dot(q-o)/d.dot(d)); }
	float distance( const Vector &q )const{ return q.distance( nearest(q) ); }
};

struct Plane{
	Vector n;
	float d;

	Plane(){}
	Plane( const Vector &n,float d ):n(n),d(d){}
	Plane( const Vector &v,const Vector &n );
	Plane( const Vector &v0,const Vector &v1,const Vector &v2 );

	Plane operator-()const{ return Plane(-n,-d); }
	Plane operator+( const Vector &t )const{ return Plane(n,d-n.dot(t)); }

	float distance( const Vector &t )const{ return n.dot(t)+d; }
	Vector nearest( const Vector &t )const{ return t-n*distance(t); }
	float t_intersect( const Line &t )const{ return -distance(t.o)/n.dot(t.d); }
	Vector intersect( const Line &t )const{ return t[t_intersect(t)]; }

	int majorAxis()const;
};

struct Matrix{
	Vector i,j,k;

	Matrix(){}
	Matrix( float n ):i(n,0,0),j(0,n,0),k(0,0,n){}
	Matrix( const Vector &i,const Vector &j,const Vector &k ):i(i),j(j),k(k){}
	Matrix( float ix,float iy,float iz,float jx,float jy,float jz,float kx,float ky,float kz ):i(ix,iy,iz),j(jx,jy,jz),k(kx,ky,kz){}

	Matrix operator~()const;
	Matrix operator-()const;

	Vector operator*( const Vector &t )const;
	Plane operator *( const Plane &t )const;
	Matrix operator*( const Matrix &t )const;

	float determinant()const;
	Matrix cofactored()const;

	static Matrix identity(){ return Matrix(1); }
	static Matrix yaw( float q ){ return Matrix(Vector(cos(q),0,sin(q)),Vector(0,1,0),Vector(-sin(q),0,cos(q)) ); }
	static Matrix pitch( float q ){ return Matrix(Vector(1,0,0),Vector(0,cos(q),sin(q)),Vector(0,-sin(q),cos(q))); }
};

struct Transform{
	Matrix m;
	Vector v;

	Transform(){}
	Transform( const Vector &v ):m(1),v(v){}
	Transform( const Matrix &m ):m(m),v(0){}
	Transform( const Matrix &m,const Vector &v ):m(m),v(v){}
	Transform( const Vector &i,const Vector &j,const Vector &k,const Vector &v ):m(i,j,k),v(v){}

	Transform operator~()const;
	Transform operator-()const;

	Plane operator*( const Plane &t )const;
	Vector operator*( const Vector &t )const;
	Transform operator*( const Transform &t )const;

	void glLoadMatrix()const;
	void glMultMatrix()const;
};

#endif