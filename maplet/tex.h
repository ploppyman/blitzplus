
#ifndef TEX_H
#define TEX_H

class Tex{
public:
	static BBString *name( int id );
	static int find( BBString *file );
	static int load( BBString *file );
};

#endif