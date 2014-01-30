
#include "std.h"
#include "geom.h"
#include "poly.h"
#include "editor.h"

static const int TOOLS_WIDTH=160;

BBWindow *main_win;
static BBTabber *main_tabs;

static BBGLContext *gl_context;

static BBCanvas *ed_canvas;
static BBGraphics *ed_graphics;
static BBPanel *ed_tools;

struct OpenDoc{
	BBString *name;
	Editor *editor;
	Document *document;

	OpenDoc( BBString *t,Editor *ed,Document *doc ):name(t->copy()),editor(ed),document(doc){
	}

	~OpenDoc(){
		delete editor;
		delete document;
		name->release(); 
	}
};

static OpenDoc *cur_doc;

static vector<OpenDoc*> open_docs;

static bool menuOpen();
static bool menuSave();
static bool menuSaveAs();
static bool menuClose();

enum{
	MENU_NULL,
	MENU_NEW,MENU_OPEN,MENU_CLOSE,MENU_CLOSEALL,
	MENU_SAVE,MENU_SAVEAS,MENU_SAVEALL,
	MENU_EXIT
};

static void createGadgets(){

	main_win=bbCreateWindow( BBTMPSTR("My Window"),0,0,800,600,0,15 );
	main_win->setText( BBTMPSTR("Maplet 2.0") );

	main_tabs=bbCreateTabber( 0,0,main_win->clientWidth()-TOOLS_WIDTH,main_win->clientHeight(),main_win,0 );

	ed_canvas=bbCreateCanvas( 0,0,main_tabs->clientWidth(),main_tabs->clientHeight(),main_tabs,0 );
	ed_canvas->setLayout( 1,1,1,1 );

	ed_graphics=ed_canvas->graphics();

	gl_context=(BBGLContext*)ed_graphics->query( BBQID_BBGLCONTEXT );

	gl_context->makeCurrent();

	glViewport( 0,0,ed_canvas->clientWidth(),ed_canvas->clientHeight() );

	BBMenu *menu=main_win->menu();

	BBMenu *file=bbCreateMenu( BBTMPSTR("File"),0,menu );
		bbCreateMenu( BBTMPSTR("New"),MENU_NEW,file);
		bbCreateMenu( BBTMPSTR("Open..."),MENU_OPEN,file);
		bbCreateMenu( BBTMPSTR("Close"),MENU_CLOSE,file);
		bbCreateMenu( BBTMPSTR("Close All"),MENU_CLOSEALL,file);
		bbCreateMenu( BBString::null(),0,file);
		bbCreateMenu( BBTMPSTR("Save..."),MENU_SAVE,file);
		bbCreateMenu( BBTMPSTR("Save As..."),MENU_SAVEAS,file);
		bbCreateMenu( BBTMPSTR("Save All"),MENU_SAVEALL,file);
		bbCreateMenu( BBString::null(),0,file);
		bbCreateMenu( BBTMPSTR("Exit"),MENU_EXIT,file);

	Editor::createMenus( main_win->menu() );
	main_win->updateMenu();

	ed_tools=bbCreatePanel( main_win->clientWidth()-TOOLS_WIDTH,0,TOOLS_WIDTH,main_win->clientHeight(),main_win,0 );
	ed_tools->setLayout( 0,1,1,1 );

	Editor::createTools( ed_tools );
}

static void refresh(){

	cur_doc->editor->refresh();

	ed_canvas->flip( false );
}

static void selectDoc( OpenDoc *doc ){

	if( cur_doc==doc ) return;

	for( int k=0;k<open_docs.size();++k ){

		if( open_docs[k]!=doc ) continue;

		main_tabs->select( k );

		cur_doc=open_docs[k];

		refresh();

		return;
	}

	bbError( "SelectDoc failed" );
}

static bool openDoc( BBString *name ){

	BBString *tab;
	Document *doc;

	if( name->size() ){

		doc=Document::open( name );

		if( !doc ) return false;

		tab=name->extractFile( false );

	}else{

		doc=new Document();

		tab=new BBString( "<untitled>" );

	}

	main_tabs->add( tab,-1 );

	tab->release();

	open_docs.push_back( new OpenDoc( name,new Editor(doc,ed_canvas),doc ) );

	selectDoc( open_docs.back() );

	return true;
}

static void closeDoc( OpenDoc *doc ){

	for( int k=0;k<open_docs.size();++k ){

		if( open_docs[k]!=doc ) continue;

		open_docs.erase( open_docs.begin()+k );

		main_tabs->remove( k );

		delete doc;

		if( open_docs.size() ){

			selectDoc( open_docs[main_tabs->items()->selected()] );

		}else{

			openDoc( BBString::null() );
		}

		return;
	}

	bbError( "Failed to close document" );
}


static void menuNew(){

	openDoc( BBString::null() );
}

static bool menuOpen(){

	bool done=false;

	BBString *file=bbRequestFile( BBTMPSTR("Open document"),BBTMPSTR("maplet2"),false,BBTMPSTR("") );

	if( file->size() ){

		done=openDoc( file );

		if( !done ){

			bbNotify( BBTMPSTR("Error opening document"),true );
		}
	}

	file->release();

	return done;
}

static bool menuClose(){

	if( cur_doc->document->modified() ){

		switch( bbProceed( BBTMPSTR("Document has been modified - save before closing?"),true) ){
		case 1://YES
			if( !menuSave() ) return false;
		case 0://NO
			break;
		case -1://CANCEL
			return false;
		}
	}

	closeDoc( cur_doc );

	return true;
}

static bool menuCloseAll(){

	while( open_docs.size() ){

		OpenDoc *t=*open_docs.begin();

		if( t->document->modified() ){

			selectDoc( t );

			if( !menuClose() ) return false;

		}else{

			if( open_docs.size()==1 ) return true;

			closeDoc( t );
		}
	}

	return true;
}

static bool menuSaveAs(){

	bool done=false;

	BBString *file=bbRequestFile( BBTMPSTR("Save document"),BBTMPSTR("maplet2"),true,BBTMPSTR("") );

	if( file->size() ){

		if( cur_doc->document->save( file ) ){

			cur_doc->name->release();

			cur_doc->name=file->copy();

			BBString *tab=file->extractFile(false);

			main_tabs->modify( main_tabs->items()->selected(),tab,-1 );

			tab->release();

			done=true;

		}else{

			bbNotify( BBTMPSTR("Error saving document"),true );
		}
	}

	file->release();

	return done;
}

static bool menuSave(){

	if( !cur_doc->name->size() ) return menuSaveAs();

	bool done=false;

	if( cur_doc->document->save( cur_doc->name ) ){

		done=true;

	}else{

		bbNotify( BBTMPSTR("Error saving document"),true );
	}

	return done;
}

static bool menuSaveAll(){

	for( int k=0;k<open_docs.size();++k ){

		OpenDoc *t=open_docs[k];

		if( !t->document->modified() ) continue;

		if( t->name->size() ){
			
			if( !t->document->save( t->name ) ) return false;

			continue;
		}

		selectDoc( t );

		if( !menuSave() ) return false;
	}

	return true;
}

static void bbMain(){

	const char *cfg="OpenGL";

	BBModule::setDriver( "Canvas",cfg );
	BBModule::setDriver( "Graphics",cfg );

	BBModule::stopDrivers();
	BBModule::startDrivers();

	createGadgets();

	menuNew();

	for(;;){

		BBEvent ev;
		bbNextEvent( &ev,-1,true );

		bool ref=false;

		switch( ev.kind ){
		case BBEvent::KEY_DOWN:
		case BBEvent::KEY_UP:
		case BBEvent::KEY_CHAR:
			if( ev.source==ed_canvas ) ref=cur_doc->editor->handleKeyEvent(&ev);
			break;
		case BBEvent::WINDOW_CLOSE:
			bbEnd();
			break;
		case BBEvent::MENU_ACTION:
			switch( ev.data ){
			case MENU_NEW:
				menuNew();
				break;
			case MENU_OPEN:
				menuOpen();
				break;
			case MENU_CLOSE:
				menuClose();
				break;
			case MENU_CLOSEALL:
				menuCloseAll();
				break;
			case MENU_SAVE:
				menuSave();
				break;
			case MENU_SAVEAS:
				menuSaveAs();
				break;
			case MENU_SAVEALL:
				menuSaveAll();
				break;
			case MENU_EXIT:
				if( menuCloseAll() ) bbEnd();
				break;
			default:
				ref=cur_doc->editor->handleMenuEvent(&ev);
			}
			break;
		case BBEvent::GADGET_ACTION:
			if( ev.source==main_tabs ){
				cur_doc=open_docs[ ev.data ];
				refresh();
				break;
			}
			break;
		case BBEvent::MOUSE_DOWN:
		case BBEvent::MOUSE_UP:
		case BBEvent::MOUSE_MOVE:
			if( ev.source==ed_canvas ) ref=cur_doc->editor->handleMouseEvent(&ev);
			break;
		}

		if( ref ) refresh();

		bbActivateGadget( ed_canvas );
	}
}


int WINAPI WinMain( HINSTANCE inst,HINSTANCE prev,LPSTR cmd,int show ){

	bbRun( bbMain,"",0 );

	return 0;
}
