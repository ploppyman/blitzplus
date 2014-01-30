
#ifndef PRIM_H
#define PRIM_H

#include "bsp.h"
#include "model.h"

class GroupPrim;

class Prim{
	Transform _tform;
	GroupPrim *_group;
	mutable Transform _gtform;
	mutable bool _valid;

protected:

	friend class GroupPrim;

	void	setGroup( GroupPrim *group );
	virtual void invalidate();

public:
	Prim();
	virtual ~Prim();

	void	setLocalTform( const Transform &tform );
	void	setGlobalTform( const Transform &tform );

	const	Transform &localTform()const{ return _tform; }
	const	Transform &globalTform()const;

	GroupPrim *group()const{ return _group; }

	virtual void update( Box &box )=0;
	virtual	void glRender( Brush *b )=0;
	virtual void insert( BSPNode *bsp,Model &out )=0;
	virtual bool planar( const Vector &v )=0;
};

class GroupPrim : public Prim{
protected:

	list<Prim*> _prims;

	void	invalidate();

public:
	void	update( Box &box );
	void	glRender( Brush *b );
	void	insert( BSPNode *bsp,Model &out );
	bool	planar( const Vector &v );

	virtual void clear();
	virtual void add( Prim *prim );
	virtual void erase( Prim *prim );

	const list<Prim*> &prims()const{ return _prims; }
};

class LeafPrim : public Prim{
	int		_kind;
	Brush*	_brush;

protected:

	virtual const Model &baseModel()=0;

public:
	LeafPrim();
	~LeafPrim();

	virtual int tag()=0;

	void	update( Box &box );
	void	glRender( Brush *b );
	void	insert( BSPNode *bsp,Model &out );
	bool	planar( const Vector &v );

	void	setSolid( bool solid );
	void	setHollow( bool hollow );
	void	setLeafKind( int kind );
	void	setBrush( Brush *brush );

	int		leafKind()const{ return _kind; }
	Brush*	brush()const{ return _brush; }

};

class CubePrim : public LeafPrim{
	int		tag();
	const	Model &baseModel();
};

class SpherePrim : public LeafPrim{
	int		tag();
	const	Model &baseModel();
};

#endif