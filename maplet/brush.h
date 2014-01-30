
#ifndef BRUSH_H
#define BRUSH_H

#include "geom.h"

class Brush{
	int _id,_tex;
	BBString *_file;
	Vector _color;

public:
	Brush( const Vector &color=1 );

	int id()const{ return _id; }
	int tex()const{ return _tex; }
	BBString *file()const{ return _file; }
	const Vector &color()const{ return _color; }

	static Brush *null();
	static Brush *load( BBString *file );
	static const vector<Brush*> &loaded();
};

#endif