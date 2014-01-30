
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "prim.h"

class Document{

	GroupPrim*	_group;
	BSPNode*	_bsp;
	Model		_model;
	bool		_modified;

public:
	Document();
	Document( GroupPrim *group );
	~Document();

	void		clear();
	void		add( Prim *prim );
	void		erase( Prim *prim );
	void		update();
	void		groupAll();

	BSPNode*	bsp()const{ return _bsp; }
	const		Model &model()const{ return _model; }
	GroupPrim*	rootGroup()const{ return _group; }
	bool		modified()const{ return _modified; }

	bool		save( BBString *file );
	static		Document *open( BBString *file );
};

#endif