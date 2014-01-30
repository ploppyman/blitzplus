
#include "std.h"
#include "vis.h"
#include "volume.h"

int  Vis::mergeNodes( int src_node,int dst_node ){

	Portals &src=_visNodes[src_node].portals;
	Portals &dst=_visNodes[dst_node].portals;

	if( !dst.size() ) return src_node;
	if( !src.size() ) return dst_node;

	for( int k=0;k<dst.size();++k ){
		src.push_back(dst[k]);
	}
	dst.clear();
	return src_node;
}

void Vis::portalize(){

	int k;

	_leafNodes.clear();
	_leafNodes.resize( _leaves.size() );

	_visNodes.clear();

	//create a node for each prim...
	map<void*,int> prim_map;
	for( k=0;k<_leaves.size();++k ){
		if( void *p=_leaves[k]->leafPrim ){
			map<void*,int>::iterator it=prim_map.find(p);
			if( it!=prim_map.end() ){
				_leafNodes[k]=it->second;
				continue;
			}
			prim_map.insert( make_pair(p,_visNodes.size()) );
		}
		_leafNodes[k]=_visNodes.size();
		_visNodes.push_back( VisNode() );
	}

	_portalPolys.clear();

	for( k=0;k<_nodes.size();++k ){

		vector<LeafPoly> tmp;

		_nodes[k]->kids[0]->clip( _nodes[k]->planePoly,BSPNode::HOLLOW,tmp );

		for( int j=0;j<tmp.size();++j ){

			int src_leaf=tmp[j].leaf->enumId;
			int src_node=_leafNodes[src_leaf];
			void *src_prim=tmp[j].leaf->leafPrim;

			vector<LeafPoly> tmp2;

			_nodes[k]->kids[1]->clip( tmp[j].poly,BSPNode::HOLLOW,tmp2 );

			for( int n=0;n<tmp2.size();++n ){

//				p=tmp[j].leaf->clipToParents(p);
//				if( !p.valid() ) p=tmp2[n].poly.flipped();

				int dst_leaf=tmp2[n].leaf->enumId;
				int dst_node=_leafNodes[dst_leaf];
				void *dst_prim=tmp2[n].leaf->leafPrim;

				if( src_node==dst_node ) continue;

				Poly p=tmp2[n].poly;

				_visNodes[src_node].portals.push_back(Portal(p,dst_node));
				_visNodes[dst_node].portals.push_back(Portal(p.flipped(),src_node));

				_portalPolys.push_back(p);
				_portalPolys.push_back(p.flipped());
			}
		}
	}
}

void Vis::setVis( int src,int dst,bool vis ){

	int n=dst*_visNodes.size()+src;

	if( vis ) _vis[n/8]|=1<<(n&7);
	else _vis[n/8]&=!(1<<(n&7));
}

bool Vis::getVis( int src,int dst ){

	int n=dst*_visNodes.size()+src;
	return !!(_vis[n/8]&(1<<(n&7)));
}

void Vis::vis( int node,const Poly &src_poly,const Poly &cur_poly ){

	setVis( node,src_node,true );
	setVis( src_node,node,true );

	Volume vis_vol,tmp_vol;

	Volume::antiPenumbra( src_poly,cur_poly,vis_vol );

	const Portals &portals=_visNodes[node].portals;

	for( int k=0;k<portals.size();++k ){

		const Poly &dst_poly=portals[k].poly;

		if( src_poly.planar(dst_poly) || dst_poly.planar(src_poly) ) continue;

		Poly src=src_poly.clip( dst_poly );
		if( !src.valid() ) continue;

		Poly dst=dst_poly.clip( src_poly,1 );
		if( !dst.valid() ) continue;

		dst=vis_vol.clip( dst );
		if( !dst.valid() ) continue;

		tmp_vol.clear();
		Volume::antiPenumbra( cur_poly,portals[k].poly,tmp_vol );

		src=tmp_vol.clip( src,1 );
		if( !src.valid() ) continue;

		vis( portals[k].dst_node,src,dst );
	}
}

void Vis::vis( int node,const Poly &src_poly ){

	setVis( node,src_node,true );
	setVis( src_node,node,true );

	const Portals &portals=_visNodes[node].portals;

	for( int k=0;k<portals.size();++k ){

		if( portals[k].dst_node==src_node ) continue;

		const Poly &dst_poly=portals[k].poly;

		if( src_poly.planar(dst_poly) || dst_poly.planar(src_poly) ) continue;

		vis( portals[k].dst_node,src_poly,dst_poly );
	}
}

void Vis::vis( int node ){

	src_node=node;

	setVis( node,node,true );

	const Portals &portals=_visNodes[node].portals;

	for( int k=0;k<portals.size();++k ){

		int node=portals[k].dst_node;

		vis( node,portals[k].poly );
	}
}

void Vis::vis( BSPNode *tree,const Model &model ){

	_nodes.clear();
	_leaves.clear();

	tree->enumNodes( _nodes );
	tree->enumLeaves( _leaves,BSPNode::HOLLOW );

	portalize();

	int sz=(_visNodes.size()*_visNodes.size()+7)&~7;

	_vis=new char[sz];
	memset( _vis,0,sz );

	int k;
	for( k=0;k<_visNodes.size();++k ){
		vis(k);
	}

	for( k=0;k<model.size();++k ){

		vector<LeafPoly> tmp;

		tree->clip( model[k],BSPNode::HOLLOW,tmp );

		for( int j=0;j<tmp.size();++j ){

			int leaf=tmp[j].leaf->enumId;
			int node=_leafNodes[leaf];

			_visNodes[node].model.push_back( tmp[j].poly );
		}
	}
}

void Vis::glRender( int eye_leaf,const Vector &color ){

	//find eye node...
	int eye_node=eye_leaf>=0 ? _leafNodes[eye_leaf] : -1;

	for( int k=0;k<_visNodes.size();++k ){
		if( eye_node==-1 || getVis(eye_node,k) ){
			_visNodes[k].model.glRender(color);
		}
	}
}
