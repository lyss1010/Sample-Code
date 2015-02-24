////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"
#include <vector>
#include "skeleton.h"
#include "skin.h"
#include "morph.h"
#include "AnimationPlayer.h"
#define WINDOWTITLE	"Spinning Cube"
////////////////////////////////////////////////////////////////////////////////
AnimationPlayer animationPlayer;
Animation animation;
Skeleton skeleton;
Skin skin;
Morph morph;
static Tester *TESTER;
GLUI *glui;
void gluiFunction();
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);

	gluiFunction();
	glutMainLoop();
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton ); 
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	//light0
	glEnable(GL_LIGHT0);
	GLfloat light_position[] = { -5.0, 1.0, -1.0, 1.0 };
	GLfloat light_diffuse[] = { 0.5, 0.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	//light1
	glEnable(GL_LIGHT1);
	GLfloat light_position1[] = { 5.0, 1.0, -1.0, 1.0 };
	GLfloat light_diffuse1[] = { 0.0, 1.0, 0.5, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

	GLfloat mat_d[] = { 0.1, 0.5, 0.8, 1.0 };
	GLfloat mat_s[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat low_sh[] = { 5.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_s);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT, GL_SHININESS, low_sh);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	// Initialize components
	skeleton.load("wasp.skel");
	skin.load("wasp.skin");
	animation.load("wasp_walk.anim");
	animationPlayer.SetClip(animation); 
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
	skeleton.update(glui);
	skin.update(skeleton.joints);
	animationPlayer.Update();
	skeleton.setPose(animationPlayer.GetPose());
	/**/
	
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
	Cube.Reset();
}


void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();
  // skeleton.draw();
   skin.draw();
   glColor3f(1, 1, 1);
   glBegin(GL_QUADS);
   glVertex3f(-500, 0, -500);
   glVertex3f(-500, 0, 500);
   glVertex3f(500, 0, 500);
   glVertex3f(500, 0, -500);
   glEnd();
	// Finish drawing scene
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case 'l':
			morph.load("head1.morph",skin.vertexs);
			break;
		case 'p':
			morph.load("head2.morph", skin.vertexs);
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}



////////////////////////////////////////////////////////////////////////////////
void gluiFunction(){
	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/
	//glui = GLUI_Master.create_glui("GLUI");
	glui = GLUI_Master.create_glui_subwindow(TESTER->WindowHandle,
		GLUI_SUBWINDOW_RIGHT);
	glui->set_main_gfx_window(TESTER->WindowHandle);
	
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idle);
}

