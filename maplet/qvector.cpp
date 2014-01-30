
#include "qvector.h"

#include <stdlib.h>

static const int MAX_BLOCK=2048;//256;

static int *mem[MAX_BLOCK],_avail;

int qtl::qavail(){
	return _avail;
}

void *qtl::qmalloc( int sz ){
	sz=(sz+sizeof(int)+7)&~7;
	int blk=sz/8;
	if( blk>=MAX_BLOCK || !mem[blk] ){
		int *p=(int*)malloc(sz);
		*p++=blk;
		return p;
	}
	_avail-=blk*8;
	int *p=mem[blk];
	mem[blk]=*(int**)p;
	*p++=blk;
	return p;
}

void qtl::qfree( void *q ){
	if( !q ) return;
	int *p=(int*)q-1;
	int blk=*p;
	if( blk>=MAX_BLOCK ){
		free(p);
	}else{
		_avail+=blk*8;
		*(int**)p=mem[blk];
		mem[blk]=p;
	}
}
