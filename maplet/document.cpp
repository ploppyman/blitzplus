
#include "std.h"
#include "document.h"

Document::Document():_modified(false){
	_group=new GroupPrim();
	_bsp=new BSPNode();
}

Document::Document( GroupPrim *group ):_group(group),_modified(false){
	_bsp=new BSPNode();
}

Document::~Document(){
	delete _group;
	delete _bsp;
}

void Document::clear(){

	delete _bsp;
	_bsp=new BSPNode();

	_model.clear();

	_group->clear();

	_modified=true;
}

void Document::add( Prim *prim ){

	_group->add( prim );

	prim->insert( _bsp,_model );

	_modified=true;
}

void Document::erase( Prim *prim ){

	_group->erase( prim );

	update();

	_modified=true;
}

void Document::update(){

	delete _bsp;
	_bsp=new BSPNode();

	_model.clear();

	_group->insert( _bsp,_model );

	_modified=true;
}

void Document::groupAll(){
}

static vector<Brush*> _brus;
static map<Brush*,int> _brumap;

static void enumBrushes( Prim *prim ){

	if( GroupPrim *t=dynamic_cast<GroupPrim*>(prim) ){

		const list<Prim*> &prims=t->prims();

		list<Prim*>::const_iterator it;
		for( it=prims.begin();it!=prims.end();++it ){
			enumBrushes( *it );
		}

	}else if( LeafPrim *t=dynamic_cast<LeafPrim*>(prim) ){

		Brush *b=t->brush();

		if( _brumap.find(b)==_brumap.end() ){

			_brumap[b]=_brus.size();
			_brus.push_back( b );
		}
	}
}

static Prim *readPrim( BBStream *str ){

	int tag;
	Transform tform;
	str->read( (char*)&tag,4 );
	str->read( (char*)&tform,48 );

	Prim *prim;

	if( tag=='GROU' ){

		int size;
		str->read( (char*)&size,4 );

		GroupPrim *t=new GroupPrim();

		while( (size--)>0 ) t->add( readPrim(str) );

		prim=t;

	}else{

		int kind,bru;
		str->read( (char*)&kind,4 );
		str->read( (char*)&bru,4 );

		LeafPrim *t;
		switch( tag ){
		case 'CUBE':t=new CubePrim();break;
		default:bbError( "Corrupt file!" );
		}

		//set brush!
		t->setBrush( _brus[bru] );
		t->setLeafKind( kind );

		prim=t;
	}
	prim->setLocalTform( tform );
	return prim;
}

static void writePrim( BBStream *str,Prim *prim ){

	if( GroupPrim *t=dynamic_cast<GroupPrim*>(prim) ){

		const list<Prim*> &prims=t->prims();

		int tag='GROU',size=prims.size();

		str->write( (char*)&tag,4 );
		str->write( (char*)&prim->localTform(),48 );
		str->write( (char*)&size,4 );

		list<Prim*>::const_iterator it;
		for( it=prims.begin();it!=prims.end();++it ){
			writePrim( str,*it );
		}

	}else if( LeafPrim *t=dynamic_cast<LeafPrim*>(prim) ){

		int tag=t->tag();
		int kind=t->leafKind();
		int bru=_brumap[t->brush()];

		str->write( (char*)&tag,4 );
		str->write( (char*)&prim->localTform(),48 );
		str->write( (char*)&kind,4 );
		str->write( (char*)&bru,4 );
	}
}

bool Document::save( BBString *file ){

	BBStream *str=bbWriteFile( file );
	if( !str ) return false;

	const vector<Brush*> &brus=Brush::loaded();

	_brus.clear();
	_brumap.clear();

	enumBrushes( _group );

	int size=_brus.size();
	str->write( (char*)&size,4 );

	for( int k=0;k<size;++k ){
		Brush *b=_brus[k];
		bbWriteString( str,b->file() );
	}

	writePrim( str,_group );

	str->release();

	_modified=false;
	return true;
}

Document *Document::open( BBString *file ){

	BBStream *str=bbReadFile( file );
	if( !str ) return false;

	int size;
	str->read( (char*)&size,4 );

	_brus.clear();
	for( int k=0;k<size;++k ){
		BBString *file=bbReadString( str );
		Brush *b=Brush::load( file );
		_brus.push_back( b );
		file->release();
	}

	Prim *p=readPrim( str );

	GroupPrim *t=dynamic_cast<GroupPrim*>(p);

	if( !t ){
		t=new GroupPrim();
		t->add(p);
	}

	str->release();

	Document *doc=new Document(t);
	doc->update();
	return doc;
}
