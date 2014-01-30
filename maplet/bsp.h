
#ifndef BSP_H
#define BSP_H

#include "model.h"
#include "brush.h"

class LeafPrim;
struct BSPNode;

struct LeafHit{
	float time;
	Plane plane;
	const BSPNode *leaf;
};

struct BSPNode{

	enum{
		NODE,SOLID,HOLLOW
	};

	int kind;
	Poly planePoly;				//for nodes
	LeafPrim *leafPrim;
	BSPNode *parent,*kids[2];

	BSPNode();
	BSPNode( BSPNode *parent );
	~BSPNode();

	void insert( LeafPrim *prim,const Model &model );

	void clip( LeafPrim *prim,const Model &model,Model &out )const;
	void clip( const Model &model,Model &out )const;

	Poly clip( Poly p )const;
	Poly clip( const Poly &p,int leaf_kind,Model &out )const;
	Poly clip( const Poly &p,LeafPrim *leaf_prim,Model &out )const;

	void flip();
	void condense();
	void makeNode( const Plane &p );

	void buildModel( Model &out )const;

	bool node()const{ return kind==NODE; }
	bool leaf()const{ return kind!=NODE; }
	const Plane &plane()const{ return planePoly.plane(); }

	float intersect( const Line &ln,int leaf_kind=SOLID,float t0=0,float t1=1,LeafHit *hit=0 )const;
};

/*
struct BSPNode;

struct LeafPoly{
	BSPNode *leaf;
	Poly poly;

	LeafPoly():leaf(0){}
	LeafPoly( BSPNode *t,const Poly &p ):leaf(t),poly(p){}
};

struct LeafHit{
	float time;
	Plane plane;
	BSPNode *leaf;
};

struct BSPNode{
	enum{
		HOLLOW,SOLID,NODE
	};

	int kind;
	int enumId;
	BSPNode *parent;
	Poly planePoly;
	union{
		void *leafPrim;
		BSPNode *kids[2];
	};

	BSPNode();
	BSPNode( int leaf_kind,BSPNode *parent );
	~BSPNode();

	void verify();

	void clearLeafPrims();

	void flip();

	void makeNode( Plane p );

	void enumNodes( vector<BSPNode*> &nodes );
	void enumLeaves( vector<BSPNode*> &leaves,int leaf_kind );

	void carve( const Model &polys,void *prim );
	void build( const Model &polys,void *prim );

	void inClip( const Model &polys,Model &out )const;
	void outClip( const Model &polys,Model &out )const;
	void planarClip( const Model &polys,Model &out )const;
	void planarClip( const Model &polys,void *prim,Model &out )const;

	Poly clip( const Poly &poly,int leaf_kind,Model &out )const;
	Poly clip( const Poly &poly,void *leafPrim,Model &out );

	void clip( const Model &model,int leaf_kind,Model &out,bool flip )const;
	void clip( const Model &model,void *leafPrim,Model &out,bool flip );

	void clip( const Poly &poly,int leaf_kind,vector<LeafPoly> &out );

	void createModel( Model &out );
	void createPortals( Model &out );

	bool leaf()const{ return kind!=NODE; }
	bool node()const{ return kind==NODE; }
	bool solid()const{ return kind==SOLID; }
	bool hollow()const{ return kind==HOLLOW; }
	const Plane &plane()const{ return planePoly.plane(); }

	Poly clipToParents( Poly p )const;

	float intersect( const Line &ln,int leaf_kind=SOLID,float t0=0,float t1=1,LeafHit *hit=0 );
};
*/

#endif
