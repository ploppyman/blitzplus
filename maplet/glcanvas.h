
#ifndef GLCANVAS_H
#define GLCANVAS_H

class GLCanvas : public GXGadget{
public:
	GLCanvas( int x,int y,int w,int h,GXGroup *group,int style );

	void makeCurrent();
	void swapBuffers();
};

#endif