
#include "std.h"
#include "model.h"

Model &Model::flip(){
	for( int k=0;k<size();++k ) poly(k).flip();
	return *this;
}

Model Model::flipped()const{
	Model t;
	for( int k=0;k<size();++k ) t.push_back( poly(k).flipped() );
	return t;
}

Model &Model::transform( const Transform &t ){
	for( int k=0;k<size();++k ) poly(k).transform(t);
	return *this;
}

Model Model::transformed( const Transform &t )const{
	Model q;
	for( int k=0;k<size();++k ) q.push_back( poly(k).transformed(t) );
	return q;
}

void Model::update( Box &box ){
	for( int k=0;k<size();++k ){
		const Poly &p=poly(k);
		for( int j=0;j<p.size();++j ){
			box.update( p.vertex(j).coords );
		}
	}
}

void Model::setBrush( Brush *b ){
	for( int k=0;k<size();++k ) poly(k).setBrush(b);
}

void Model::glRender( Brush *b )const{
	for( int k=0;k<size();++k ){
		poly(k).glRender( b );
	}
}

/*

int Model::n_reps;

Model::Rep::Rep():_n(1){
	++n_reps;
}

Model::Rep::Rep( const vector<Poly> &polys ):_n(1),_polys(polys){
	++n_reps;
}

Model::Rep::Rep( Rep *t ):_n(1),_polys(t->_polys){
	++n_reps;
}

Model::Rep::~Rep(){
	--n_reps;
}

Model::Model():_rep(new Rep()){
}

Model::Model( const vector<Poly> &polys ):_rep(new Rep(polys)){
}

Model::Model( const Model &t ):_rep(t._rep){
	++_rep->_n;
}

Model::~Model(){
	if( !--_rep->_n ) delete _rep;
}

Model &Model::operator=( const Model &t ){
	++t._rep->_n;
	if( !--_rep->_n ) delete _rep;
	_rep=t._rep;
	return *this;
}

void Model::write(){
	if( _rep->_n==1 ) return;
	--_rep->_n;
	_rep=new Rep(_rep);
}

Model Model::flipped()const{
	vector<Poly> tmp;
	for( int k=0;k<size();++k ){
		tmp.push_back( _rep->_polys[k].flipped() );
	}
	return tmp;
}

Model &Model::transform( const Transform &t ){
	write();
	for( int k=0;k<size();++k ) _rep->_polys[k].transform(t);
	return *this;
}

Model Model::transformed( const Transform &t )const{

	vector<Poly> polys;
	polys.reserve(size());

	Vertex verts[Poly::MAX_VERTS];

//	Matrix m=t.m.cofactored();

	for( int k=0;k<size();++k ){

		const Poly &p=poly(k);

		memcpy( verts,p.vertices(),p.size()*sizeof(Vertex) );

		for( int j=0;j<p.size();++j ){
			verts[j].coords=t*verts[j].coords;
//			verts[j].normal=m*verts[j].normal;
		}

		polys.push_back( Poly(p.size(),verts) );
	}

	return Model(polys);
}

void Model::glRender()const{
	for( int k=0;k<size();++k ) poly(k).glRender();
}

void Model::glRender( const Vector &color )const{
	for( int k=0;k<size();++k ) poly(k).glRender( color );
}

Model Model::cube(){
	static int t[]={5,7,6,4,7,3,2,6,3,1,0,2,1,5,4,0,1,3,7,5,4,6,2,0 };

	vector<Poly> model;
	Box box( Vector(-1),Vector(1) );

	int n=0;
	for( int k=0;k<6;++k ){
		Vertex verts[4];
		verts[3].coords=box.corner(t[n++]);
		verts[2].coords=box.corner(t[n++]);
		verts[1].coords=box.corner(t[n++]);
		verts[0].coords=box.corner(t[n++]);
		model.push_back( Poly(4,verts) );
	}
	return model;
}

Model Model::sphere( int subs ){

	float off=0;//.5f;

	int subs2=subs*2,r;

	vector<Poly> model;

	Vertex verts[4];

	//top cap
	float p1=1*PI/subs-HALFPI;
	float y1=sin(p1),r1=cos(p1);
	for( r=0;r<subs2;++r ){

		float d1=(r+off)*TWOPI/subs2;
		float d2=(r+1+off)*TWOPI/subs2;

		verts[0]=Vector( 0,-1,0 );
		verts[1]=Vector( cos(d2)*r1,y1,sin(d2)*r1 );
		verts[2]=Vector( cos(d1)*r1,y1,sin(d1)*r1 );

		model.push_back( Poly(3,verts) );
	}

	//bottom cap
	p1=(subs-1)*PI/subs-HALFPI;
	y1=sin(p1);r1=cos(p1);
	for( r=0;r<subs2;++r ){

		float d1=(r+off)*TWOPI/subs2;
		float d2=(r+1+off)*TWOPI/subs2;

		verts[0]=Vector( 0,1,0 );
		verts[1]=Vector( cos(d1)*r1,y1,sin(d1)*r1 );
		verts[2]=Vector( cos(d2)*r1,y1,sin(d2)*r1 );

		model.push_back( Poly(3,verts) );
	}

	//segments
	for( int y=1;y<subs-1;++y ){

		float p1=(y)*PI/subs-HALFPI;
		float p2=(y+1)*PI/subs-HALFPI;

		float y1=sin(p1),r1=cos(p1);
		float y2=sin(p2),r2=cos(p2);

		for( int r=0;r<subs2;++r ){

			float d1=(r+off)*TWOPI/subs2;
			float d2=(r+1+off)*TWOPI/subs2;

			verts[0]=Vector( cos(d1)*r1,y1,sin(d1)*r1 );
			verts[1]=Vector( cos(d2)*r1,y1,sin(d2)*r1 );
			verts[2]=Vector( cos(d2)*r2,y2,sin(d2)*r2 );
			verts[3]=Vector( cos(d1)*r2,y2,sin(d1)*r2 );

			model.push_back( Poly(4,verts) );
		}
	}

	return Model( model );
}
*/