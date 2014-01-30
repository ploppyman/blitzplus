
#include "std.h"
#include "brush.h"

struct BBStringCmp{
	bool operator()( BBString *a,BBString *b )const{
		return a->compare(b)<0;
	}
};

static Brush *_null;
static vector<Brush*> _loaded;
static map<BBString*,Brush*,BBStringCmp> _loadedmap;

Brush::Brush( const Vector &color ):_id(-1),_tex(0),_file(BBString::null()),_color(color){
}

Brush *Brush::null(){
	if( !_null ){
		_null=new Brush();
		_null->_id=_loaded.size();
		_loaded.push_back(_null);
	}
	return _null;
}

Brush *Brush::load( BBString *file ){

	if( !file->size() ) return null();

	GLuint id;

	map<BBString*,Brush*,BBStringCmp>::iterator it=_loadedmap.find(file);

	if( it!=_loadedmap.end() ) return it->second;

	BBGraphics *g=bbLoadGraphics( file );
	if( !g ) return null();

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

	Brush *brush=new Brush();

	brush->_id=_loaded.size();
	brush->_tex=id;
	brush->_file->release();
	brush->_file=file->copy();

	_loaded.push_back( brush );

	return brush;
}

const vector<Brush*> &Brush::loaded(){
	return _loaded;
}
