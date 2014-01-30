
#ifndef MODEL_H
#define MODEL_H

#include "poly.h"

class Model : public vector<Poly>{
public:

	Model(){}
	Model( const Poly &p ){ push_back(p); }

	Poly &poly( int n ){ return (*this)[n]; }
	const Poly &poly( int n )const{ return (*this)[n]; }

	Model &flip();
	Model flipped()const;

	Model &transform( const Transform &t );
	Model transformed( const Transform &t )const;

	void update( Box &box );

	void setBrush( Brush *b );

	void glRender( Brush *b )const;
};

#endif