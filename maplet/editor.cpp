
#include "std.h"
#include "editor.h"

#include <stdio.h>
#include <stdlib.h>

static Vector up(0,1,0);

static BBComboBox *_modeCombo,*_primCombo,*_bruCombo;

static vector<Brush*> _brushes;

enum{
	MENU_BRUOPEN=1001,MENU_BRUOPENDIR=1002
};

Editor::Editor( Document *doc,BBCanvas *canvas ):_doc(doc),_canvas(canvas){

	_mode=MODE_CREATE;
	_createMode=CREATE_ORG;

	_mouse=Vector(0,0,1);
	_mouseView=Line( _camera.v,_camera.m*_mouse ).normalized();

	_lmb=_rmb=_ctrl=_shift=false;

	_grid=Plane( Vector(0,1,0),0 );

	_modPrim=0;

	_createPrim=0;
	
	_camera=Transform( Matrix::pitch(QUARTERPI),Vector(0,5,-5) );

	_solid=false;

	_prim=0;
	_cursor=_grid.intersect( _mouseView );

	refreshTools();
}

Editor::~Editor(){
}

void Editor::findLeafPrim( const Line &ln ){

	LeafHit hit;

	_prim=0;

	float t=_doc->bsp()->intersect( ln,BSPNode::SOLID,0,1,&hit );

	if( t==0 ){

		t=_doc->bsp()->intersect( ln,BSPNode::HOLLOW,0,1,&hit );
		if( t==0 || t==1 ) return;
		t=_doc->bsp()->intersect( ln,BSPNode::SOLID,t,1,&hit );
		if( t==0 || t==1 ) return;

	}else if( t==1 ){
		return;
	}

	if( _prim=hit.leaf->leafPrim ){

		_primHit=ln[t];
		_primPlane=hit.plane;

		if( _prim->planar( _primHit ) ) return;

		_prim=0;
	}

	Line t_line( ln[t],ln.o-ln[t] );

	t=_doc->bsp()->intersect( t_line,BSPNode::HOLLOW,0,1,&hit );

	if( t==1 ) return;

	_prim=hit.leaf->leafPrim;

	_primHit=t_line[t];
	_primPlane=hit.plane;
}

void Editor::findPrim(){

	Line ln( _mouseView.o,_mouseView.d * 256 );
	findLeafPrim( ln );

	if( !_prim ) return;

	while( _prim->group()!=_doc->rootGroup() ) _prim=_prim->group();
}

void Editor::drawPlane( const Plane &p ){
	glPushMatrix();
	glTranslatef( 0,-p.d,0 );
	glBegin( GL_LINES );
	for( int t=WORLD_MIN;t<=WORLD_MAX;t+=WORLD_SIZE/64 ){
		glVertex3f( WORLD_MIN,0,t );
		glVertex3f( WORLD_MAX,0,t );
		glVertex3f( t,0,WORLD_MIN );
		glVertex3f( t,0,WORLD_MAX );
	}
	glEnd();
	glPopMatrix();
}

void Editor::refresh(){

	glEnable( GL_DEPTH_TEST );

	glEnable( GL_CULL_FACE );

	glFrontFace( GL_CW );

	glPolygonOffset( 1,1 );

	glEnable( GL_POLYGON_OFFSET_FILL );

	glClearColor( 0,.5f,1,1 );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	glPointSize( 4 );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -1,1,-1,1,1,1000 );
	Transform( Matrix(1,0,0, 0,1,0, 0,0,-1),Vector(0) ).glMultMatrix();
	glMatrixMode( GL_MODELVIEW );
	(-_camera).glLoadMatrix();

	//render main model
	_doc->model().glRender( 0 );

	if( _mode==MODE_CREATE && _createMode!=CREATE_ORG ){
		_createPrim->glRender( 0 );
		glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		Brush b( Vector(0,10,0) );
		_createPrim->glRender( &b );
		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );

	}else if( _prim ){
		//render selected prim
		glPolygonMode( GL_FRONT_AND_BACK,GL_LINE );
		Brush b( Vector(10,10,0) );
		_prim->glRender( &b );
		glPolygonMode( GL_FRONT_AND_BACK,GL_FILL );
	}

	if( _modPrim ){
		_modPrim->glRender(0);
	}

	//render grid
	if( _mode==MODE_CREATE ){
		glColor3f( 0,.7f,1 );
		drawPlane( _grid );
		glColor3f( 1,1,1 );
		glBegin( GL_POINTS );
		glVertex3fv( _cursor );
		glEnd();
	}else if( _mode==MODE_MODIFY ){
		glColor3f( 0,.7f,1 );
		drawPlane( _grid );
		glColor3f( 1,1,0 );
		glBegin( GL_POINTS );
		glVertex3fv( _cursor );
		glEnd();
	}
}

void Editor::refreshTools(){
	int sel=_bruCombo->items()->selected();
	_bruCombo->clear();
	const vector<Brush*> &brushes=Brush::loaded();
	for( int k=0;k<brushes.size();++k ){
		if( brushes[k]->file()->size() ){
			BBString *t=brushes[k]->file()->extractFile( false );
			_bruCombo->add( t,-1 );
			t->release();
		}else{
			_bruCombo->add( BBTMPSTR("<null>"),-1 );
		}
	}
	if( sel!=-1 ) _bruCombo->select( sel );
}

bool Editor::handleKeyEvent( const BBEvent *ev ){
	bool ref=false;

	if( ev->kind==BBEvent::KEY_DOWN ){

		switch( ev->data ){
		case 29:
			_ctrl=true;
		case 42:
			_shift=true;
			break;
		}

	}else if( ev->kind==BBEvent::KEY_UP ){

		switch( ev->data ){
		case 29:
			_ctrl=false;
		case 42:
			_shift=false;
			break;
		}

	}else if( ev->kind==BBEvent::KEY_CHAR ){

		switch( ev->data ){
		case ' ':
			_solid=!_solid;
			if( _createPrim ) _createPrim->setSolid( _solid );
			ref=true;
			break;
		case 'g':
			_doc->groupAll();
			ref=true;
			break;
		case 'c':
			delete _createPrim;
			_createPrim=0;
			_mode=MODE_CREATE;
			_createMode=CREATE_ORG;
			ref=true;
			break;
		case 'm':
			_mode=MODE_MODIFY;
			_modMode=MODIFY_MOVE;
			_modPrim=0;
			ref=true;
			break;
		case 's':
			_mode=MODE_MODIFY;
			_modMode=MODIFY_SCALE;
			_modPrim=0;
			ref=true;
			break;
		case 'r':
			_mode=MODE_MODIFY;
			_modMode=MODIFY_ROTATE;
			_modPrim=0;
			ref=true;
			break;
		case 27:
			_createMode=CREATE_ORG;
			ref=true;
			break;
		case 8:
			if( _prim ){
				_doc->erase( _prim );
				delete _prim;
				_prim=0;
				ref=true;
			}
			break;
		}
	}
	return ref;
}

bool Editor::handleMenuEvent( const BBEvent *ev ){

	if( ev->data==MENU_BRUOPEN ){

		BBString *file=bbRequestFile( BBTMPSTR("Open Brush"),BBTMPSTR("bmp,jpg,png"),false,BBTMPSTR("") );

		if( !file->size() ){
			file->release();
			return false;
		}

		Brush *b=Brush::load( file );

		refreshTools();

		_bruCombo->select( b->id() );

		file->release();
	}

	return false;
}

bool Editor::handleMouseEvent( const BBEvent *ev ){
	bool ref=false;

	if( ev->kind==BBEvent::MOUSE_DOWN ){

		if( ev->data==1 ){

			_lmb=true;

			if( _mode==MODE_CREATE ){

				if( _createMode==CREATE_ORG ){

					Brush *br=Brush::loaded()[_bruCombo->items()->selected()];

					_createPrim=new CubePrim();
					_createPrim->setSolid( _solid );
					_createPrim->setBrush( br );

					_createOrg=_cursor;
					_createPrim->setGlobalTform( Transform(Matrix(0),_createOrg) );

					_createMode=CREATE_RECT;
					ref=true;

				}else if( _createMode==CREATE_HEIGHT ){

					Brush *br=Brush::loaded()[_bruCombo->items()->selected()];

					_createPrim->setBrush( br );

					_doc->add( _createPrim );

					_createPrim=0;
					_createMode=CREATE_ORG;

					ref=true;
				}
			}else if( _mode==MODE_MODIFY ){

				if( _prim ){
					Box box;
					box.a=INFINITE;
					box.b=-INFINITE;
					_prim->update( box );

					_modPrim=_prim;
					_modOrg=_primHit;
					_modPlane=_primPlane;
					_modTform=_prim->globalTform();
					_modPivot=box.centre();
				}
				ref=true;
			}

		}else if( ev->data==2 ){
			_rmb=true;
		}
	}else if( ev->kind==BBEvent::MOUSE_UP ){

		if( ev->data==1 ){

			_lmb=false;

			if( _mode==MODE_CREATE ){

				if( _createMode==CREATE_RECT ){

					_createMode=CREATE_HEIGHT;
				}

			}else if( _mode==MODE_MODIFY ){

				_modPrim=0;

				_doc->update();

				ref=true;
			}

		}else if( ev->data==2 ){
			_rmb=false;
		}

	}else if( ev->kind==BBEvent::MOUSE_MOVE ){

		int _mx=ev->x;
		int _my=ev->y;

		_mouse=Vector( _mx*2.0f/_canvas->clientWidth()-1,1-_my*2.0f/_canvas->clientHeight(),1 );

		Line mv=_mouseView;

		_mouseView=Line( _camera.v,_camera.m*_mouse ).normalized();

		if( _ctrl ){

			float yaw=mv.d.yaw()-_mouseView.d.yaw();
			float pitch=mv.d.pitch()-_mouseView.d.pitch();

			yaw+=_camera.m.k.yaw();
			pitch+=_camera.m.k.pitch();

			_camera.m=Matrix::yaw(yaw)*Matrix::pitch(pitch);

			_mouseView=mv;

		}else if( _rmb ){

			if( _shift ){

				_camera.v+=_cursor-Line( _cursor,up ).nearest( _mouseView );

			}else{

				_camera.v+=_cursor-Plane( up,-_cursor.y ).intersect( _mouseView );

			}

		}else if( _shift ){

			if( _mode==MODE_CREATE && _createMode==CREATE_ORG ){

				_cursor=Line( _cursor,up ).nearest( _mouseView );
				_grid.d=-_cursor.y;

			}else{
				_camera.v+=_cursor-Line( _cursor,up ).nearest( _mouseView );
			}

		}else if( _mode==MODE_CREATE ){

			if( _createMode==CREATE_ORG ){

				findPrim();

				_cursor=_grid.intersect( _mouseView );
				_cursor.x=floor((_cursor.x*4)+.5f)/4;
				_cursor.z=floor((_cursor.z*4)+.5f)/4;

			}else{

				if( _createMode==CREATE_RECT ){

					_cursor=_grid.intersect( _mouseView );
					_cursor.x=floor((_cursor.x*4)+.5f)/4;
					_cursor.z=floor((_cursor.z*4)+.5f)/4;

				}else if( _createMode==CREATE_HEIGHT ){

					_cursor=Line( _cursor,up ).nearest( _mouseView );
				}

				Box box( _createOrg );
				box.update( _cursor );

				Transform t(
					Vector( box.width()/2,0,0 ),
					Vector( 0,box.height()/2,0 ),
					Vector( 0,0,box.depth()/2 ),
					box.centre() );

				_createPrim->setGlobalTform( t );
			}

		}else if( _mode==MODE_MODIFY ){

			if( _modPrim ){

				if( _modMode==MODIFY_MOVE ){

					_cursor=Plane( up,-_cursor.y ).intersect( _mouseView );

					Vector dv=_cursor-_modOrg;
					
					Transform t( _modTform.m,_modTform.v+dv );

					_modPrim->setGlobalTform( t );

//					_doc->update();

				}else if( _modMode==MODIFY_SCALE ){

					_cursor=Line( _modOrg,_modPlane.n ).nearest( _mouseView );

					Vector dv=(-_modTform*_cursor)/(-_modTform*_modOrg);

					Matrix sc( Vector(dv.x,0,0),Vector(0,dv.y,0),Vector(0,0,dv.z) );

					_modPrim->setGlobalTform( _modTform * Transform(sc) );

//					_doc->update();

				}else if( _modMode==MODIFY_ROTATE ){

					_cursor=Plane( up,-_cursor.y ).intersect( _mouseView );

					Vector tv=_modTform.v;
					Matrix tm=_modTform.m;
					Matrix tr=Matrix( tm.i.normalized(),tm.j.normalized(),tm.k.normalized() );
					Matrix ts=Matrix( Vector(tm.i.length(),0,0),Vector(0,tm.j.length(),0),Vector( 0,0,tm.k.length()) );

//					char buff[256];
//					sprintf( buff,"%f %f %f",ts.i.x,ts.j.y,ts.k.z );
//					main_win->setStatusText( BBTMPSTR( buff ) );

					Vector v1=_cursor-_modTform.v;
					Vector v2=_modOrg-_modTform.v;

					float y1=atan2( v1.x,v1.z );
					float y2=atan2( v2.x,v2.z );

					Matrix my( Matrix::yaw( y2-y1 ) );

					_modPrim->setGlobalTform( Transform( my*tr*ts,tv ) );

//					_doc->update();
				}

			}else{

				findPrim();

				if( _prim ){

					_cursor=_primHit;

				}else{

					_cursor=_grid.intersect( _mouseView );
					_cursor.x=floor((_cursor.x*4)+.5f)/4;
					_cursor.z=floor((_cursor.z*4)+.5f)/4;
				}
			}
		}

		ref=true;
	}

	return ref;
}

void Editor::createMenus( BBMenu *parent ){

	BBMenu *bru=bbCreateMenu( BBTMPSTR("Brush"),0,parent );
		bbCreateMenu( BBTMPSTR("Open..."),MENU_BRUOPEN,bru );
		bbCreateMenu( BBTMPSTR("Open Dir..."),MENU_BRUOPENDIR,bru );

}

void Editor::createTools( BBPanel *panel ){

	_modeCombo=bbCreateComboBox( 8,8,panel->clientWidth()-16,24,panel,0 );

	_modeCombo->add( BBTMPSTR("Carve"),-1 );

	_modeCombo->select(0);

	_primCombo=bbCreateComboBox( 8,40,panel->clientWidth()-16,24,panel,0 );

	_primCombo->add( BBTMPSTR("Cube"),-1 );
	_primCombo->add( BBTMPSTR("Sphere"),-1 );

	_primCombo->select(0);

	_bruCombo=bbCreateComboBox( 8,72,panel->clientWidth()-16,24,panel,0 );

	Brush::null();

	refreshTools();

	_bruCombo->select(0);
}
