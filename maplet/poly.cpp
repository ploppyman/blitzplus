
#include "std.h"
#include "poly.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

int Poly::n_reps;

const float PLANAR_EPSILON=1.0f/256.0f;//1024.0f;

Poly::Rep::Rep():_n(1),_id(0),_size(0){
	++n_reps;
}

Poly::Rep::Rep( int sz,const Vertex *verts ):_n(1),_id(0),_size(sz),_brush(0),_plane(Poly::plane(sz,verts)){
	memcpy( _verts,verts,_size*sizeof(Vertex) );
	++n_reps;
}

Poly::Rep::Rep( int sz,const Vertex *verts,const Plane &p ):_n(1),_id(0),_size(sz),_brush(0),_plane(p){
	memcpy( _verts,verts,_size*sizeof(Vertex) );
	++n_reps;
}

Poly::Rep::Rep( Rep *t ):_n(1),_id(t->_id),_size(t->_size),_brush(t->_brush),_plane(t->_plane){
	memcpy( _verts,t->_verts,_size*sizeof(Vertex) );
	++n_reps;
}

Poly::Rep::~Rep(){
	--n_reps;
}

void Poly::Rep::map(){
	for( int k=0;k<_size;++k ){
		Vector t(1);
		const Vector &v=_verts[k].coords;
		switch( _plane.majorAxis() ){
		case 0:	//x
			if( _plane.n.x>0 ){
				t.x=v.z;t.y=-v.y;
			}else{
				t.x=-v.z;t.y=-v.y;
			}
			break;
		case 1:	//y
			if( _plane.n.y>0 ){
				t.x=v.x;t.y=-v.z;
			}else{
				t.x=v.x;t.y=v.z;
			}
			break;
		case 2:	//z
			if( _plane.n.z>0 ){
				t.x=-v.x;t.y=-v.y;
			}else{
				t.x=v.x;t.y=-v.y;
			}
			break;
		}
		_verts[k].tex_coords=t;
	}
}

void *Poly::Rep::operator new( size_t sz ){
	return qtl::qmalloc(sz);
}

void Poly::Rep::operator delete( void *q ){
	qtl::qfree(q);
}

Poly::Poly():_rep(new Rep()){
}

Poly::~Poly(){
	if( !--_rep->_n ) delete _rep;
}

Poly::Poly( const Poly &t ):_rep(t._rep){
	++_rep->_n;
}

Poly::Poly( const Plane &p ):_rep(new Rep()){
	*this=intersect(p,INFINITE_BOX);
}

Poly::Poly( int n,const Vertex *verts ):_rep(new Rep(n,verts)){
}

Poly::Poly( int n,const Vertex *verts,const Plane &p ):_rep(new Rep(n,verts,p)){
}

Poly &Poly::operator=( const Poly &t ){
	++t._rep->_n;
	if( !--_rep->_n ) delete _rep;
	_rep=t._rep;
	return *this;
}

void Poly::write(){
	if( _rep->_n==1 ) return;
	--_rep->_n;
	_rep=new Rep(_rep);
}

void Poly::setId( int id ){
	write();
	_rep->_id=id;
}

void Poly::setBrush( Brush *brush ){
	write();
	_rep->_brush=brush;
}

int Poly::planar( const Plane &p )const{
	for( int k=0;k<size();++k ){
		if( fabs(p.distance(vertex(k).coords))>PLANAR_EPSILON ) return 0;
	}
	return normal().dot(p.n)>0 ? 1 : -1;
}

int Poly::classify( const Plane &p )const{

	int n_p=0,n_f=0,n_b=0;

	for( int k=0;k<size();++k ){
		float d=p.distance(vertex(k).coords);
		if( fabs(d)<=PLANAR_EPSILON ) ++n_p;
		else if( d>0 ) ++n_f;
		else ++n_b;
	}

	if( n_p==size() ) return PLANAR;

	if( n_f && n_b ) return SPLIT;

	return n_f ? FRONT : BACK;
}

Poly Poly::clip( const Plane &p,int side )const{
	Poly out[2];
	split( p,out );
	return out[side];
}

Poly &Poly::flip(){
	write();
	for( int k=0;k<size()/2;++k ){
		swap( _rep->_verts[k],_rep->_verts[size()-1-k] );
	}
	_rep->_plane=-_rep->_plane;
	return *this;
}

Poly Poly::flipped()const{
	Vertex verts[MAX_VERTS];
	for( int k=0;k<size();++k ){
		verts[k]=vertex(size()-k-1);
	}
	Poly p( size(),verts,-plane() );
	p.setId(id());p.setBrush(brush());
	return p;
}

int Poly::split( const Plane &p,Poly out[2] )const{

	switch( classify(p) ){
	case PLANAR:
		if( p.n.dot(normal())>0 ) out[0]=*this;
		else out[1]=*this;
		return PLANAR;
	case FRONT:
		out[0]=*this;
		return FRONT;
	case BACK:
		out[1]=*this;
		return BACK;
	}

	int n_out[2]={0,0};
	Vertex verts[2][MAX_VERTS];

	Vertex vert=vertex(size()-1);
	float dist=p.distance( vert.coords );
	bool plnr=fabs(dist)<=PLANAR_EPSILON;

	for( int i=0;i<size();++i ){
		//store 'previous' vertex info
		Vertex p_vert=vert;
		float p_dist=dist;
		bool p_plnr=plnr;

		//get current vertex info
		vert=vertex(i);
		dist=p.distance( vert.coords );
		plnr=fabs(dist)<=PLANAR_EPSILON;

		if( plnr ){
			verts[0][n_out[0]++]=vert;
			verts[1][n_out[1]++]=vert;
			continue;
		}

		if( !p_plnr && ((dist>0)!=(p_dist>0)) ){
			Vertex clip;
			float t=p_dist/(p_dist-dist);
			float omt=1-t;
			clip.coords=vert.coords*t + p_vert.coords*omt;
			verts[0][n_out[0]++]=clip;
			verts[1][n_out[1]++]=clip;
		}
		int side=dist<0;
		verts[side][n_out[side]++]=vert;
	}

	if( n_out[0]<3 || n_out[1]<3 ) bbError( "Illegal Poly::split result",true );

	out[0]=Poly( n_out[0],verts[0],plane() );
	out[0].setId(id());out[0].setBrush(brush());
	out[1]=Poly( n_out[1],verts[1],plane() );
	out[1].setId(id());out[1].setBrush(brush());

	return SPLIT;
}

float glAlpha=.5f;

void Poly::glRender( Brush *b )const{

	static Brush null_brush;

	if( !b ) b=brush();
	if( !b ) b=&null_brush;

	if( int tex=b->tex() ){
		_rep->map();
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D,tex );
		glTexEnvi( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE );
	}

	int n=plane().majorAxis();
	float t=normal()[n];
	Vector q=b->color()*fabs(t)/(3-n);
	glColor4f( q.x,q.y,q.z,glAlpha );

	glBegin( GL_POLYGON );
	for( int k=0;k<size();++k ){
		glTexCoord3fv( vertex(k).tex_coords );
		glVertex3fv( vertex(k).coords );
	}
	glEnd();

	glDisable( GL_TEXTURE_2D );
}

Poly &Poly::transform( const Transform &t ){
	write();
//	Matrix m=t.m.cofactored();
	for( int k=0;k<size();++k ){
		_rep->_verts[k].coords=t*_rep->_verts[k].coords;
//		_rep->_verts[k].normal=m*_rep->_verts[k].normal;
	}
	_rep->_plane=Poly::plane(size(),_rep->_verts);
	return *this;
}

Poly Poly::transformed( const Transform &t )const{
	Vertex verts[MAX_VERTS];
	memcpy( verts,_rep->_verts,size()*sizeof(Vertex) );
//	Matrix m=t.m.cofactored();
	for( int k=0;k<size();++k ){
		verts[k].coords=t*verts[k].coords;
//		verts[k].normal=m*verts[k].normal;
	}
	Poly p(size(),verts);
	p.setId(id());p.setBrush(brush());
	return p;
}

Plane Poly::plane( int n,const Vertex *verts ){

	Vector c(0),nm(0);

	for( int k=0;k<n;++k ){

		const Vector &v0=verts[k].coords;
		const Vector &v1=verts[(k+1)%n].coords;
		const Vector &v2=verts[(k+2)%n].coords;

		c+=v0;

		Vector t=(v1-v0).cross(v2-v0);
		if( t.length()>EPSILON ) nm+=t;
	}

	return Plane( c/n,nm );
}

Poly Poly::intersect( const Plane &p,const Box &box ){

	//find maximal Plane axis
	int ax_n=p.majorAxis();

	//find other axis'
	int ax_u=(ax_n+1)%3,ax_v=(ax_n+2)%3;

	//create Poly verts
	Vector t;
	Vertex verts[4];

	for( int k=0;k<4;++k ){
		float u=(k==1||k==2) ? box.b[ax_u] : box.a[ax_u];
		float v=(k==2||k==3) ? box.b[ax_v] : box.a[ax_v];
		//solve Plane equ for n at u,v
		float n=(p.n[ax_u]*u+p.n[ax_v]*v+p.d)/-p.n[ax_n];
		t[ax_u]=u;t[ax_v]=v;t[ax_n]=n;
		verts[k].coords=t;
	}

	//flip Poly for negative Planes
	if( p.n[ax_n]<0 ) swap( verts[1],verts[3] );

	Poly poly(4,verts,p);

	poly=poly.clip( box.plane(ax_n*2) );
	if( poly.valid() ) poly=poly.clip( box.plane(ax_n*2+1) );

	return poly;

}

Poly Poly::intersect( const Box &b,const Plane &p ){
	return intersect( p,b );
}
