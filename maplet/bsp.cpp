
#include "std.h"
#include "bsp.h"
#include "prim.h"

BSPNode::BSPNode():kind(SOLID),leafPrim(0),parent(0){
	kids[0]=kids[1]=0;
}

BSPNode::BSPNode( BSPNode *p ):kind(SOLID),leafPrim(0),parent(p){
	kids[0]=kids[1]=0;
}

BSPNode::~BSPNode(){
	delete kids[0];
	delete kids[1];
}

//************************** INSERT *********************************

void BSPNode::insert( LeafPrim *prim,const Model &model ){

	int leaf_kind=prim->leafKind();

	int side=leaf_kind==HOLLOW ? 0 : 1;

	if( leaf() ){

		if( kind==leaf_kind ){
			return;
		}

		BSPNode *t=this;
		LeafPrim *p=leafPrim;

		int k;
		for( k=0;k<model.size();++k ){

			if( model[k].id()<0 ) continue;

			t->makeNode( model[k].plane() );

			t->kids[1-side]->kind=p ? p->leafKind() : SOLID;
			t->kids[1-side]->leafPrim=p;

			t=t->kids[side];
		}

		t->kind=leaf_kind;
		t->leafPrim=prim;

		return;
	}

	Model out[2];

	int k;
	for( k=0;k<model.size();++k ){

		Poly p_out[2];

		switch( model[k].split( plane(),p_out ) ){
		case Poly::PLANAR:
			out[0].push_back( model[k] );out[0].back().setId(-1);
			out[1].push_back( model[k] );out[1].back().setId(-1);
			break;
		default:
			if( p_out[0].valid() ) out[0].push_back( p_out[0] );
			if( p_out[1].valid() ) out[1].push_back( p_out[1] );
		}
	}

	if( out[0].size()<3 ) out[0].clear();
	if( out[1].size()<3 ) out[1].clear();

	if( out[0].size() && out[1].size() ){

		Poly p( planePoly );p.setId(-1);

		for( k=0;k<model.size();++k ){

			if( model[k].id()<0 ) continue;

			Poly p_out[2];

			p.split( model[k].plane(),p_out );

			p=p_out[side];
		}

		out[0].push_back(p);
		out[1].push_back(p);
	}

	for( k=0;k<2;++k ){
		if( out[k].size() ) kids[k]->insert( prim,out[k] );
	}
}

//************************* CLIP ***************************

Poly BSPNode::clip( const Poly &p,int leaf_kind,Model &out )const{
	if( leaf() ){
		return kind==leaf_kind ? p : Poly();
	}
	Poly p_out[2];
	int q=p.split( plane(),p_out );
	for( int k=0;k<2;++k ){
		if( p_out[k].valid() ){
			p_out[k]=kids[k]->clip( p_out[k],leaf_kind,out );
		}
	}
	if( q!=Poly::SPLIT ) return p_out[0].valid() ? p_out[0] : p_out[1];
	if( p_out[0].valid() && p_out[1].valid() ) return p;
	if( p_out[0].valid() ) out.push_back( p_out[0] );
	if( p_out[1].valid() ) out.push_back( p_out[1] );
	return Poly();
}

Poly BSPNode::clip( const Poly &p,LeafPrim *leaf_prim,Model &out )const{
	if( leaf() ){
		return leafPrim==leaf_prim ? p : Poly();
	}
	Poly p_out[2];
	int q=p.split( plane(),p_out );
	for( int k=0;k<2;++k ){
		if( p_out[k].valid() ){
			p_out[k]=kids[k]->clip( p_out[k],leaf_prim,out );
		}
	}
	if( q!=Poly::SPLIT ) return p_out[0].valid() ? p_out[0] : p_out[1];
	if( p_out[0].valid() && p_out[1].valid() ) return p;
	if( p_out[0].valid() ) out.push_back( p_out[0] );
	if( p_out[1].valid() ) out.push_back( p_out[1] );
	return Poly();
}

Poly BSPNode::clip( Poly poly )const{

	const BSPNode *t=this,*p;

	while( poly.valid() && (p=t->parent) ){

		Poly out[2];

		poly.split( p->plane(),out );

		if( t==p->kids[0] ) poly=out[0];
		else poly=out[1];

		t=p;
	}

	return poly;
}

void BSPNode::clip( LeafPrim *prim,const Model &model,Model &out )const{
	if( leaf() ){
		return;
	}

	int leaf_kind=prim->leafKind();

	Poly p;
	Model tmp;

	int k;
	for( k=0;k<model.size();++k ){
		if( leaf_kind==SOLID ){
			p=clip( model[k].flipped(),prim,tmp );
		}else{
			p=clip( model[k].flipped(),SOLID,tmp );
		}
		if( p.valid() ) tmp.push_back( p );
	}

	tmp.flip();

	for( k=0;k<tmp.size();++k ){
		if( leaf_kind==SOLID ){
			p=clip( tmp[k],HOLLOW,out );
		}else{
			p=clip( tmp[k],prim,out );
		}
		if( p.valid() ) out.push_back( p );
	}
}

void BSPNode::clip( const Model &model,Model &out )const{
	if( leaf() ){
		return;
	}

	Poly p;
	Model tmp;

	int k;
	for( k=0;k<model.size();++k ){
		p=clip( model[k].flipped(),SOLID,tmp );
		if( p.valid() ) tmp.push_back( p );
	}

	tmp.flip();

	for( k=0;k<tmp.size();++k ){
		p=clip( tmp[k],HOLLOW,out );
		if( p.valid() ) out.push_back( p );
	}
}

//************************** UTIL **************************

void BSPNode::flip(){
	if( leaf() ){
		kind= kind==SOLID ? HOLLOW : SOLID;
		return;
	}
	for( int k=0;k<2;++k ) kids[k]->flip();
}

void BSPNode::condense(){
}

void BSPNode::makeNode( const Plane &p ){

	planePoly=clip( p );

	if( !planePoly.valid() ){
		bbError( "BSPNode::makeNode error - degenerate planePoly" );
	}

	kind=NODE;
	leafPrim=0;
	kids[0]=new BSPNode( this );
	kids[1]=new BSPNode( this );
	kids[0]->kind=HOLLOW;
}

void BSPNode::buildModel( Model &out )const{
	if( leaf() ){
		return;
	}

	Poly p;
	Model tmp;

	p=clip( planePoly,SOLID,tmp );
	if( p.valid() ) tmp.push_back( p );
	p=clip( planePoly.flipped(),SOLID,tmp );
	if( p.valid() ) tmp.push_back( p );

	tmp.flip();

	int k;
	for( k=0;k<tmp.size();++k ){
		p=clip( tmp[k],HOLLOW,out );
		if( p.valid() ) out.push_back( p );
	}

	for( k=0;k<2;++k ) kids[k]->buildModel( out );
}

float BSPNode::intersect( const Line &ln,int leaf_kind,float t0,float t1,LeafHit *hit )const{

	LeafHit t_hit;
	if( !hit ) hit=&t_hit;

	if( leaf() ){
		if( kind==leaf_kind ){
			hit->leaf=this;
			return t0;
		}
		hit->leaf=0;
		return t1;
	}

	float d0=plane().distance(ln[t0]);
	int side0=fabs(d0)<=PLANAR_EPSILON ? -1 : d0<0;

	float d1=plane().distance(ln[t1]);
	int side1=fabs(d1)<=PLANAR_EPSILON ? -1 : d1<0;

	if( side0<0 && side1<0 ){
		LeafHit h0,h1;
		float q0=kids[0]->intersect( ln,leaf_kind,t0,t1,&h0 );
		float q1=kids[1]->intersect( ln,leaf_kind,t0,t1,&h1 );
		if( q0<q1 ){
			*hit=h0;
			return q0;
		}
		*hit=h1;
		return q1;
	}

	if( side0<0 || side1<0 || side0==side1 ){
		int side=side0>=0 ? side0 : side1;
		return kids[side]->intersect( ln,leaf_kind,t0,t1,hit );
	}

	float t=d0/(d0-d1)*(t1-t0)+t0;
	float q0=kids[side0]->intersect( ln,leaf_kind,t0,t,hit );
	if( q0<t ) return q0;
	hit->plane=plane();
	return kids[side1]->intersect( ln,leaf_kind,t,t1,hit );
}

/*
BSPNode::BSPNode():kind(SOLID),parent(0),leafPrim(0){
}

BSPNode::BSPNode( int kind,BSPNode *parent ):kind(kind),parent(parent),leafPrim(0){
}

void BSPNode::enumNodes( vector<BSPNode*> &out ){
	if( leaf() ){
		return;
	}
	enumId=out.size();
	out.push_back( this );
	for( int k=0;k<2;++k ){
		kids[k]->enumNodes( out );
	}
}

void BSPNode::enumLeaves( vector<BSPNode*> &out,int leaf_kind ){
	if( leaf() ){
		if( kind!=leaf_kind ) return;
		enumId=out.size();
		out.push_back( this );
		return;
	}
	for( int k=0;k<2;++k ){
		kids[k]->enumLeaves( out,leaf_kind );
	}
}

void BSPNode::makeNode( Plane p ){
	kind=NODE;
	planePoly=clipToParents(Poly(p));
	kids[0]=new BSPNode( HOLLOW,this );
	kids[1]=new BSPNode( SOLID,this );

	if( !planePoly.valid() ){
		bbError( "makeNode error - degenerate planePoly",true );
	}
}

void split( const Model &vol,const Plane &plane,Model out[2],int side ){

	int k,n_p=0,n_f=0,n_b=0;

	for( k=0;k<vol.size();++k ){

		Poly p_out[2];

		int n=vol[k].split( plane,p_out );

		if( n==Poly::PLANAR ){
			++n_p;
			int n=p_out[0].valid() ? side : 1-side;
			out[n].push_back( vol[k] );
			out[n].back().setId( -1 );
			continue;
		}

		switch( n ){
		case Poly::FRONT:++n_f;break;
		case Poly::BACK:++n_b;break;
		case Poly::SPLIT:++n_f;++n_b;break;
		}

		for( int j=0;j<2;++j ){
			if( p_out[j].valid() ) out[j].push_back( p_out[j] );
		}
	}

	if( n_f && n_b ){

		if( n_p ){
			bbError( "Volume split error - planar poly error",true );
		}

		Poly p(plane);

		p.setId( -1 );

		for( k=0;k<vol.size();++k ){

			Poly p_out[2];

			p.split( vol[k].plane(),p_out );

			p=p_out[side];
		}

		out[side].push_back( p );
		out[1-side].push_back( p.flipped() );
	}

	return;

	for( k=0;k<2;++k ){
		if( out[k].size() && out[k].size()<4 ){
			bbError( "Volume split error - degenerate volume",true );
		}
	}
}

void BSPNode::build( const Model &polys,void *prim ){
	if( leaf() ){
		if( solid() ) return;

		int thisKind=kind;
		void *thisPrim=leafPrim;

		BSPNode *t=this;

		for( int k=0;k<polys.size();++k ){

			if( polys[k].id()<0 ){
				continue;
			}

			t->makeNode( polys[k].plane() );

			t->kids[0]->kind=thisKind;
			t->kids[0]->leafPrim=thisPrim;

			t=t->kids[1];
		}

		t->kind=SOLID;
		t->leafPrim=prim;

		return;
	}

	Model p_out[2];

	split( polys,plane(),p_out,1 );

	for( int k=0;k<2;++k ){
		if( p_out[k].size() ) kids[k]->build( p_out[k],prim );
	}
}

void BSPNode::carve( const Model &polys,void *prim ){
	if( leaf() ){
		if( hollow() ) return;

		int thisKind=kind;
		void *thisPrim=leafPrim;

		BSPNode *t=this;

		for( int k=0;k<polys.size();++k ){

			if( polys[k].id()<0 ){
				continue;
			}

			t->makeNode( polys[k].plane() );

			t->kids[1]->kind=SOLID;//thisKind;
			t->kids[1]->leafPrim=0;//prim;//thisPrim;

			t=t->kids[0];
		}

		t->kind=HOLLOW;
		t->leafPrim=prim;

		return;
	}

	Model p_out[2];

	split( polys,plane(),p_out,0 );

	for( int k=0;k<2;++k ){
		if( p_out[k].size() ) kids[k]->carve( p_out[k],prim );
	}
}

Poly BSPNode::clip( const Poly &p,int leaf_kind,Model &out )const{
	if( leaf() ){
		if( leafPrim ) return Poly();
		return kind==leaf_kind ? p : Poly();
	}
	Poly p_out[2];
	int q=p.split( plane(),p_out );
	for( int k=0;k<2;++k ){
		if( p_out[k].valid() ){
			p_out[k]=kids[k]->clip( p_out[k],leaf_kind,out );
		}
	}
	if( q!=Poly::SPLIT ) return p_out[0].valid() ? p_out[0] : p_out[1];
	if( p_out[0].valid() && p_out[1].valid() ) return p;
	if( p_out[0].valid() ) out.push_back( p_out[0] );
	if( p_out[1].valid() ) out.push_back( p_out[1] );
	return Poly();
}

Poly BSPNode::clip( const Poly &p,void *prim,Model &out ){
	if( leaf() ){
		if( leafPrim && leafPrim!=prim ) return Poly();
		leafPrim=prim;
		return p;
	}
	Poly p_out[2];
	int q=p.split( plane(),p_out );
	for( int k=0;k<2;++k ){
		if( p_out[k].valid() ){
			p_out[k]=kids[k]->clip( p_out[k],prim,out );
		}
	}
	if( q!=Poly::SPLIT ) return p_out[0].valid() ? p_out[0] : p_out[1];
	if( p_out[0].valid() && p_out[1].valid() ) return p;
	if( p_out[0].valid() ) out.push_back( p_out[0] );
	if( p_out[1].valid() ) out.push_back( p_out[1] );
	return Poly();
}

void BSPNode::clip( const Poly &poly,int leaf_kind,vector<LeafPoly> &out ){
	if( leaf() ){
		if( kind==leaf_kind ) out.push_back( LeafPoly(this,poly) );
		return;
	}
	Poly bits[2];
	poly.split( plane(),bits );
	for( int k=0;k<2;++k ){
		if( bits[k].valid() ) kids[k]->clip( bits[k],leaf_kind,out );
	}
}

void BSPNode::verify(){

	if( leaf() ){
		return;
	}

	if( !planePoly.valid() ){
		bbError( "Verify error - invalid node plane",true );
	}

	if( kids[0]->leaf() && kids[0]->kind==kids[1]->kind ){
		bbLogf( "Condensable!\n" );
	}

	for( int k=0;k<2;++k ){
		kids[k]->verify();
	}
}

void BSPNode::clip( const Model &model,int leaf_kind,Model &out,bool flip )const{
	for( int k=0;k<model.size();++k ){
		Poly p=clip( flip ? model[k].flipped() : model[k],leaf_kind,out );
		if( p.valid() ) out.push_back(p);
	}
}

void BSPNode::clip( const Model &model,void *leafPrim,Model &out,bool flip ){
	for( int k=0;k<model.size();++k ){
		Poly p=clip( flip ? model[k].flipped() : model[k],leafPrim,out );
		if( p.valid() ) out.push_back(p);
	}
}

void BSPNode::planarClip( const Model &model,Model &out )const{
	Model tmp;
	clip( model,SOLID,tmp,true );
	clip( tmp,HOLLOW,out,true );
}

void BSPNode::inClip( const Model &polys,Model &out )const{
	Model tmp;
	int k;
	for( k=0;k<polys.size();++k ){
		Poly p=clip( polys[k].flipped(),HOLLOW,tmp );
		if( p.valid() ) tmp.push_back(p);
	}
	for( k=0;k<tmp.size();++k ){
		Poly p=clip( tmp[k].flipped(),HOLLOW,out );
		if( p.valid() ) out.push_back(p);
	}
}

void BSPNode::outClip( const Model &polys,Model &out )const{
	Model tmp;
	int k;
	for( k=0;k<polys.size();++k ){
		Poly p=clip( polys[k].flipped(),SOLID,tmp );
		if( p.valid() ) tmp.push_back(p);
	}
	for( k=0;k<tmp.size();++k ){
		Poly p=clip( tmp[k].flipped(),SOLID,out );
		if( p.valid() ) out.push_back(p);
	}
}

void BSPNode::createModel( Model &out ){
	if( leaf() ){
		return;
	}

	int k;

	Poly p;
	Model tmp;

	p=clip( planePoly,SOLID,tmp );
	if( p.valid() ) tmp.push_back(p);

	p=clip( planePoly.flipped(),SOLID,tmp );
	if( p.valid() ) tmp.push_back(p);

	for( k=0;k<tmp.size();++k ){

		Poly p=clip( tmp[k].flipped(),HOLLOW,out );
		if( p.valid() ) out.push_back(p);
	}

	for( k=0;k<2;++k ){
		kids[k]->createModel( out );
	}
}

void BSPNode::createPortals( Model &out ){
	if( leaf() ){
		return;
	}

	vector<LeafPoly> tmp;

	kids[0]->clip( planePoly,HOLLOW,tmp );

	for( int k=0;k<tmp.size();++k ){

		void *prim=tmp[k].leaf->leafPrim;

		vector<LeafPoly> tmp2;

		kids[1]->clip( tmp[k].poly,HOLLOW,tmp2 );

		for( int j=0;j<tmp2.size();++j ){

			if( prim && prim==tmp2[j].leaf->leafPrim ) continue;

			out.push_back( tmp2[j].poly );
		}
	}

	for( k=0;k<2;++k ){
		kids[k]->createPortals( out );
	}
}

Poly BSPNode::clipToParents( Poly poly )const{
	const BSPNode *t=this,*p;
	while( poly.valid() && (p=t->parent) ){
		Poly out[2];
		poly.split( p->plane(),out );
		if( t==p->kids[0] ) poly=out[0];
		else poly=out[1];
		t=p;
	}
	return poly;
}


float BSPNode::intersect( const Line &ln,int leaf_kind,float t0,float t1,LeafHit *hit ){

	LeafHit t_hit;
	if( !hit ) hit=&t_hit;

	if( leaf() ){
		if( kind==leaf_kind ){
			hit->leaf=this;
			return t0;
		}
		hit->leaf=0;
		return t1;
	}

	float d0=plane().distance(ln[t0]);
	int side0=fabs(d0)<=PLANAR_EPSILON ? -1 : d0<0;

	float d1=plane().distance(ln[t1]);
	int side1=fabs(d1)<=PLANAR_EPSILON ? -1 : d1<0;

	if( side0<0 && side1<0 ){
		LeafHit h0,h1;
		float q0=kids[0]->intersect( ln,leaf_kind,t0,t1,&h0 );
		float q1=kids[1]->intersect( ln,leaf_kind,t0,t1,&h1 );
		if( q0<q1 ){
			*hit=h0;
			return q0;
		}
		*hit=h1;
		return q1;
	}

	if( side0<0 || side1<0 || side0==side1 ){
		int side=side0>=0 ? side0 : side1;
		return kids[side]->intersect( ln,leaf_kind,t0,t1,hit );
	}

	float t=d0/(d0-d1)*(t1-t0)+t0;
	float q0=kids[side0]->intersect( ln,leaf_kind,t0,t,hit );
	if( q0<t ) return q0;
	return kids[side1]->intersect( ln,leaf_kind,t,t1,hit );
}

void BSPNode::flip(){
	if( leaf() ){
		kind=kind==SOLID ? HOLLOW : SOLID;
		return;
	}

//	planePoly.flip();
//	std::swap( kids[0],kids[1] );

	for( int k=0;k<2;++k ){
		kids[k]->flip();
	}
}

void BSPNode::clearLeafPrims(){
	if( leaf() ){
		leafPrim=0;
		return;
	}

	for( int k=0;k<2;++k ){
		kids[k]->clearLeafPrims();
	}
}
*/
