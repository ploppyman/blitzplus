
#include "std.h"
#include "prim.h"

Prim::Prim():_tform(Vector(0)),_group(0),_gtform(Vector(0)),_valid(true){
}

Prim::~Prim(){
	if( _group ) bbError( "Prim deleted while in group" );
}

void Prim::invalidate(){
	_valid=false;
}

void Prim::setGroup( GroupPrim *group ){
	_group=group;
	invalidate();
}

void Prim::setLocalTform( const Transform &tform ){
	_tform=tform;
	invalidate();
}

void Prim::setGlobalTform( const Transform &tform ){
	_tform=_group ? -_group->globalTform() * tform : tform;
	invalidate();
}

const Transform &Prim::globalTform()const{
	if( _valid ) return _gtform;
	_gtform=_group ? _group->globalTform() * _tform : _tform;
	_valid=true;
	return _gtform;
}

//************************* Group Prim ***************************

void GroupPrim::invalidate(){
	Prim::invalidate();
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->invalidate();
	}
}

void GroupPrim::update( Box &box ){
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->update( box );
	}
}

void GroupPrim::glRender( Brush *b ){
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->glRender( b );
	}
}

void GroupPrim::insert( BSPNode *bsp,Model &out ){
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->insert( bsp,out );
	}
}

bool GroupPrim::planar( const Vector &v ){
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		if( (*it)->planar(v) ) return true;
	}
	return false;
}

void GroupPrim::clear(){
	list<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->setGroup(0);
	}
	_prims.clear();
}

void GroupPrim::add( Prim *prim ){
	if( prim->group() ) prim->group()->erase( prim );
	prim->setGroup( this );
	_prims.push_back( prim );
}

void GroupPrim::erase( Prim *prim ){
	prim->setGroup(0);
	_prims.remove( prim );
}

//************************* Leaf Prim ****************************

LeafPrim::LeafPrim():_kind(BSPNode::HOLLOW),_brush(0){
}

LeafPrim::~LeafPrim(){
}

void LeafPrim::update( Box &box ){
	Model model=baseModel();
	model.transform( globalTform() );
	model.update(box);
}

void LeafPrim::glRender( Brush *b ){

	Model model=baseModel();
	model.transform( globalTform() );
	model.setBrush( _brush );

	model.glRender( b );
}

void LeafPrim::insert( BSPNode *bsp,Model &mod ){

	if( !_brush ) return;

	Model out;

	Model model=baseModel();
	model.transform( globalTform() );
	model.setBrush( _brush );

	bsp->insert( this,model );
	bsp->clip( this,model,out );
	bsp->clip( mod,out );

	mod.swap( out );
}

bool LeafPrim::planar( const Vector &v ){

	Vector t=-globalTform() * v;
	const Model &mod=baseModel();

	for( int k=0;k<mod.size();++k ){

		float d=mod[k].plane().distance( v );

		if( fabs(d)<=PLANAR_EPSILON ) return true;
	}
	return false;
}

void LeafPrim::setSolid( bool solid ){
	_kind=solid ? BSPNode::SOLID : BSPNode::HOLLOW;
}

void LeafPrim::setHollow( bool hollow ){
	_kind=hollow ? BSPNode::HOLLOW : BSPNode::SOLID;
}

void LeafPrim::setLeafKind( int kind ){
	_kind=kind;
}

void LeafPrim::setBrush( Brush *b ){
	_brush=b;
}

//************************* Cube Prim ****************************

int CubePrim::tag(){
	return 'CUBE';
}

const Model &CubePrim::baseModel(){

	static Model model;
	if( model.size() ) return model;

	static int t[]={5,7,6,4,7,3,2,6,3,1,0,2,1,5,4,0,1,3,7,5,4,6,2,0 };

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

/*

Prim::Prim():_kind(BSPNode::HOLLOW),_tform( Matrix(1) ),_brush(0){
}

Prim::~Prim(){
}

void Prim::setSolid( bool solid ){
	_kind=solid ? BSPNode::SOLID : BSPNode::HOLLOW;
}

void Prim::setHollow( bool hollow ){
	_kind=hollow ? BSPNode::HOLLOW : BSPNode::SOLID;
}

void Prim::setLeafKind( int kind ){
	_kind=kind;
}

void Prim::setTransform( const Transform &t ){
	_tform=t;
}

void Prim::setBrush( Brush *b ){
	_brush=b;
}

//************************ Group Prim ****************************

GroupPrim::GroupPrim(){
	_bsp=new BSPNode();
}

GroupPrim::~GroupPrim(){
	delete _bsp;
}

const Model &GroupPrim::model(){
	return _model;
}

bool GroupPrim::planar( const Vector &v ){
	vector<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		if( (*it)->planar(v) ) return true;
	}
	return false;
}

void GroupPrim::insert( BSPNode *bsp,Model &mod ){

	vector<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->insert( bsp,mod );
	}
}

void GroupPrim::update( const Transform &cur_tform ){

	Transform tform=cur_tform * transform();

	vector<Prim*>::iterator it;
	for( it=_prims.begin();it!=_prims.end();++it ){
		(*it)->update( tform );
	}

	delete _bsp;
	_bsp=new BSPNode();
	_model.clear();

	insert( _bsp,_model );
}

void GroupPrim::add( Prim *prim ){

	_prims.push_back( prim );

	prim->insert( _bsp,_model );
}

void GroupPrim::erase( Prim *prim ){
}

//************************* Leaf Prim ****************************

LeafPrim::LeafPrim(){
}

LeafPrim::~LeafPrim(){
}

const Model &LeafPrim::model(){
	return _model;
}

bool LeafPrim::planar( const Vector &v ){

	for( int k=0;k<_model.size();++k ){

		float d=_model[k].plane().distance( v );

		if( fabs(d)<=PLANAR_EPSILON ) return true;
	}
	return false;
}

void LeafPrim::insert( BSPNode *bsp,Model &mod ){

	Model out;

	bsp->insert( this,_model );
	bsp->clip( this,_model,out );
	bsp->clip( mod,out );

	mod.swap( out );
}

void LeafPrim::update( const Transform &cur_tform ){
	_model=baseModel().transformed( cur_tform * transform() );
	_model.setBrush( brush() );
}

//************************* Sphere Prim ****************************

const Model &SpherePrim::baseModel(){

	static Model model;
	if( model.size() ) return model;

	int subs=8;

	float off=0;//.5f;

	int subs2=subs*2,r;

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

	return model;
}
*/
