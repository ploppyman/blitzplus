
#ifndef VIS_H
#define VIS_H

#include "bsp.h"

class Vis{

	struct Portal{
		Poly poly;
		int dst_node;

		Portal( const Poly &p,int dst ):poly(p),dst_node(dst){}
		Portal( const Portal &t ):poly(t.poly),dst_node(t.dst_node){}
	};

	typedef vector<Portal> Portals;

	struct VisNode{
		Model model;
		vector<Portal> portals;
	};

	vector<BSPNode*> _nodes;
	vector<BSPNode*> _leaves;
	vector<VisNode>  _visNodes;
	vector<int>		 _leafNodes;	//leaf->visNode relationship

	Model _portalPolys;

	char *_vis;

	int src_node;

	bool getVis( int src,int dst );

	void setVis( int src,int dst,bool vis );

	int  mergeNodes( int src,int dst );

	void portalize();

	void vis( int node,const Poly &src_poly,const Poly &cur_poly );

	void vis( int node,const Poly &src_poly );

	void vis( int node );

	Poly clip( BSPNode *node,const Poly &poly );

public:

	void vis( BSPNode *tree,const Model &model );

	const Model &portalPolys()const{ return _portalPolys; }

	void glRender( int eye_leaf,const Vector &color=1 );
};

#endif