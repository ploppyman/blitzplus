
#ifndef EDITOR_H
#define EDITOR_H

#include "bsp.h"
#include "geom.h"
#include "document.h"
//#include "vis.h"

static const float WORLD_MIN=-128,WORLD_MAX=128,WORLD_SIZE=WORLD_MAX-WORLD_MIN;

class Editor{

	enum{
		MODE_CREATE,	//creating
		MODE_MODIFY		//modifying
	};

	enum{
		CREATE_ORG,		//selecting origin
		CREATE_RECT,	//selecting base rect
		CREATE_HEIGHT	//selecting box height
	};

	enum{
		MODIFY_MOVE,
		MODIFY_SCALE,
		MODIFY_ROTATE
	};

	Document *_doc;
	BBCanvas *_canvas;

	//external inputs
	int _mode;
	Vector _mouse;
	Line _mouseView;
	bool _lmb,_rmb,_ctrl,_shift;
	Transform _camera;	//where the camera is

	//view parameters
	Vector _cursor;
	Plane _grid;		//default grid
	Prim *_prim;		//current prim under cursor
	Vector _primHit;	//where it hits
	Plane _primPlane;	//plane it hits

	//when creating
	int _createMode;
	Vector _createOrg;
	LeafPrim *_createPrim;
	bool _solid;

	//when modifying
	int _modMode;
	Vector _modOrg;
	Plane _modPlane;
	Vector _modPivot;
	Prim *_modPrim;		//primitive being modified
	Transform _modTform;//original prim transform

	void findPrim();
	void findLeafPrim( const Line &ln );
	void drawPlane( const Plane &p );

public:
	Editor( Document *doc,BBCanvas *canvas );
	~Editor();

	void refresh();

	bool handleKeyEvent( const BBEvent *ev );
	bool handleMenuEvent( const BBEvent *ev );
	bool handleMouseEvent( const BBEvent *ev );

	Document *document()const{ return _doc; }

	static void createMenus( BBMenu *parent );
	static void createTools( BBPanel *panel );

	static void refreshTools();
};

#endif