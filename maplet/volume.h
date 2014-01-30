
#ifndef VOLUME_H
#define VOLUME_H

#include "poly.h"

class Volume : public vector<Plane>{
public:
	Poly clip( Poly p,int side=0 );

	const Plane &plane( int n )const{ return (*this)[n]; }

	static void antiPenumbra( Poly src,Poly dst,Volume &out );
};


#endif