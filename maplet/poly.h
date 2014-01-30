
#ifndef POLYGON_H
#define POLYGON_H

#include "geom.h"
#include "brush.h"

extern const float PLANAR_EPSILON;

struct Vertex{
	Vector coords,tex_coords;

	Vertex(){}
	Vertex( const Vector &coords ):coords(coords){}
};

class Poly{
public:
	enum{
		MAX_VERTS=32
	};
	enum{
		PLANAR,FRONT,BACK,SPLIT
	};
private:
	struct Rep{
		int _n;
		int _id;
		int _size;
		Brush *_brush;
		Vertex _verts[MAX_VERTS];
		Plane _plane;

		Rep();
		~Rep();
		Rep( int n,const Vertex *verts );
		Rep( int n,const Vertex *verts,const Plane &p );
		Rep( Rep *t );

		void map();

		void *operator new( size_t sz );
		void operator delete( void *q );
	};
	Rep *_rep;
	void write();
public:
	static int n_reps;

	Poly();
	~Poly();
	Poly( const Poly &t );
	Poly( const Plane &p );
	Poly( int n,const Vertex *verts );
	Poly( int n,const Vertex *verts,const Plane &p );
	Poly &operator=( const Poly &t );

	//1=planar, -1=aplanar, 0=nont
	int planar( const Plane &p )const;

	int classify( const Plane &p )const;
	int split( const Plane &p,Poly out[2] )const;

	void setId( int id );
	int id()const{ return _rep->_id; }

	void setBrush( Brush *b );
	Brush *brush()const{ return _rep->_brush; }

	Poly clip( const Plane &p,int side=0 )const;

	Poly &flip();
	Poly flipped()const;

	Poly &transform( const Transform &t );
	Poly transformed( const Transform &t )const;

	int size()const{ return _rep->_size; }
	bool valid()const{ return _rep->_size>2; }

	const Plane &plane()const{ return _rep->_plane; }
	operator const Plane &()const{ return _rep->_plane; }

	const Vector &normal()const{ return _rep->_plane.n; }
	const Vertex &vertex( int n )const{ return _rep->_verts[n]; }
	const Vertex *vertices()const{ return _rep->_verts; }

	void glRender( Brush *b )const;

	static Plane plane( int n,const Vertex *verts );
	static Poly intersect( const Plane &p,const Box &b );
	static Poly intersect( const Box &b,const Plane &p );
};

#endif