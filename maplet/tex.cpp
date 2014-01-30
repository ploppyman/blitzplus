
#include "std.h"
#include "tex.h"

struct BBStringCmp{
	bool operator()( BBString *a,BBString *b )const{
		return a->compare(b)<0;
	}
};

static map<int,BBString*> tex_unmap;
static map<BBString*,int,BBStringCmp> tex_map;

BBString *Tex::name( int id ){
	map<int,BBString*>::iterator it=tex_unmap.find(id);
	return it==tex_unmap.end() ? 0 : it->second;
}

int Tex::find( BBString *file ){
	map<BBString*,int,BBStringCmp>::iterator it=tex_map.find( file );
	return it==tex_map.end() ? 0 : it->second;
}

int Tex::load( BBString *file ){

	GLuint id;

	if( id=find( file ) ) return id;

	BBGraphics *g=bbLoadGraphics( file );
	if( !g ) return 0;

	glGenTextures( 1,&id );

	int w=g->width(),h=g->height();

	BBGraphics *p=new BBPixmap( w,h,BB_RGB888 );
	p->blit( 0,0,w,h,g,0,0,0 );

	glBindTexture( GL_TEXTURE_2D,id );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR );

	for( int n=0;;++n ){

		void *src;
		int src_pitch,src_fmt;
		p->lock( &src,&src_pitch,&src_fmt );
		glTexImage2D( GL_TEXTURE_2D,n,3,p->width(),p->height(),0,GL_BGR_EXT,GL_UNSIGNED_BYTE,src );
		p->unlock();

		BBGraphics *t=bbMipHalve( p );
		if( !t ) break;
		p->release();
		p=t;
	}

	p->release();
	g->release();

	tex_map.insert( make_pair(file->copy(),id) );
	tex_unmap.insert( make_pair(id,file->copy()) );

	return id;
}
