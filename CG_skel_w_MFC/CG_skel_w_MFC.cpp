 // CG_skel_w_MFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CG_skel_w_MFC.h"
#include "InputDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "vec.h"
#include "mat.h"
#include "InitShader.h"
#include "Scene.h"
#include "Renderer.h"
#include <string>


#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#define CAM_ADD 1
#define CAM_REND 2
#define CAM_ACTIVE 3

#define FILE_OPEN 1

#define MAIN_DEMO 1
#define MAIN_ABOUT 2
#define MAIN_PRIM 3

#define MODEL_ACTIVE 1
#define MODEL_COLOR 2
#define MODEL_BBOX 3
#define MODEL_SURFACE 4

#define FRAME_WORLD 1
#define FRAME_MODEL 2

#define NORMAL_FACE 1
#define NORMAL_VERTEX 2

#define STEP_ROTATE 1
#define STEP_SCALE 2
#define STEP_MOVE 3

#define PROJ_ORT 1
#define PROJ_PRES 2
#define PROJ_GEO 3

#define CAM_FRAME_VIEW 1
#define CAM_FRAME_WORLD 2

#define COLOR_UNI 1
#define COLOR_XUNI 2

#define LIGHT_POINT 1
#define LIGHT_PAR 2
#define LIGHT_AMB 3

#define SHADE_FLAT 1
#define SHADE_GOUR 2
#define SHADE_PHONG 3

#define LIGHT_ADD 1
#define LIGHT_ACTIVE 2
#define LIGHT_COLOR 3
#define LIGHT_POSITION 4
#define LIGHT_ORIENT 5
#define LIGHT_CLEAR 6

Scene* scene;
Renderer* renderer;

int last_x, last_y;
bool lb_down, rb_down, mb_down;

//--------------------------------------------------------------------------
// Helpers

string dialogBox(CString s) {
	printf("dialog box\n");
	CCmdDialog dlg(s);
	if (dlg.DoModal() == IDOK) {
		return dlg.GetCmd();
	}
	return "";
}

vec3 dialogBoxVec(CString s){
	CCmdXyzDialog dlg(s);
	vec3 v;
	if (dlg.DoModal() == IDOK)
		v = dlg.GetXYZ();
	return v;
}

//----------------------------------------------------------------------------
// Callbacks

void display(void)
{
	printf("DISPLAY\n");
	//Call the scene and ask it to draw itself
	//scene* new_scene = new scene(fileName);
	scene->draw(); //CHANGE
	glFlush();
}

void reshape(int width, int height)
{
	//update the renderer's buffers
	renderer->reshape(width, height);
	//glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
	//printf("KEYBOARD %c\n", key);
	switch (key) {
	case 033:
		//printf("EXIT\n");
		exit(EXIT_SUCCESS);
		break;

	case '+':
		//printf("PLUS\n");
		scene->scale(key);
		break;
	case '-':
		//printf("MINUS\n");
		scene->scale(key);
		break;

	case 'b':
		scene->bbox();
		break;

	case 'f':
		scene->focus();
		break;
	case 'i':
		scene->zoomIn();
		break;
	case 'o':
		scene->zoomOut();
		break;
	
	case 'x':
		scene->rotate('x');
		break;
	case 'y':
		scene->rotate('y');
		break;
	case 'z':
		scene->rotate('z');
		break;
	case 'X':
		scene->rotate('X');
		break;
	case 'Y':
		scene->rotate('Y');
		break;
	case 'Z':
		scene->rotate('Z');
		break;


	case 'w': // camera up
		scene->camMove('u');
		break;
	case 'a': // camera left
		scene->camMove('l');
		break;
	case 's': // camera down
		scene->camMove('d');
		break;
	case 'd':  // camera right
		scene->camMove('r');
		break;
	case 'A': // camera close
		scene->camMove('c');
		break;
	case 'D': // camera far
		scene->camMove('f');
		break;

	case '[':
		scene->dimm();
		break;
	case ']':
		scene->bloom();
		break;
	}
	
	glutPostRedisplay();
}

void catchKey(int key, int x, int y) {
	//printf("CATCH KEY %d\n", key);
	switch (key) {
	case GLUT_KEY_LEFT:
		scene->scale('l');
		break;
	case GLUT_KEY_RIGHT:
		scene->scale('r');
		break;
	case GLUT_KEY_UP:
		scene->scale('u');
		break;
	case GLUT_KEY_DOWN:
		scene->scale('d');
		break;
	case GLUT_KEY_HOME:
		scene->scale('z');
		break;
	case GLUT_KEY_END:
		scene->scale('Z');
		break;
	}
	glutPostRedisplay();
}


void mouse(int button, int state, int x, int y)
{
	//printf("MOUSE\n");
	//button = {GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON}
	//state = {GLUT_DOWN,GLUT_UP}

	//set down flags
	switch (button) {
	case GLUT_LEFT_BUTTON:
		//printf("LEFT MOUSE\n");
		lb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_RIGHT_BUTTON:
		//printf("RIGHT MOUSE\n");
		rb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	case GLUT_MIDDLE_BUTTON:
		//printf("MIDDLE MOUSE\n");
		mb_down = (state == GLUT_UP) ? 0 : 1;
		break;
	}
	glutPostRedisplay();
}

void motion(int x, int y)
{
	//printf("MOTION\n");
	// calc difference in mouse movement
	int dx = x - last_x;
	int dy = y - last_y;

	// update last x,y
	last_x = x;
	last_y = y;

	scene->move(dx, dy);
	glutPostRedisplay();
}

void fileMenu(int id)
{
	switch (id)
	{
	case FILE_OPEN:
		CFileDialog dlg(TRUE, _T(".obj"), NULL, NULL, _T("*.obj|*.*"));
		if (dlg.DoModal() == IDOK)
		{
			std::string s((LPCTSTR)dlg.GetPathName());
			scene->loadOBJModel((LPCTSTR)dlg.GetPathName());
		}
		break;
	}
}

void mainMenu(int id)
{
	switch (id)
	{
	case MAIN_DEMO:
		scene->drawDemo();
		break;
	case MAIN_ABOUT:
		AfxMessageBox(_T("Computer Graphics"));
		break;
	case MAIN_PRIM:
		scene->addPrim();
		break;
	}
}

void camMenu(int id) {
	vec3 eye, at, up;
	string cmd = "";
	int curr_cam;
	switch (id)
	{
	case CAM_ADD:
		eye = dialogBoxVec("Eye");
		at = dialogBoxVec("At");
		up = dialogBoxVec("Up");
		//camDialog(&eye, &at, &up);
		scene->addCam(cmd, eye, at, up);
		break;
	case CAM_REND:
		scene->render();
		break;
	case CAM_ACTIVE:
		curr_cam = stoi(dialogBox("Active Camera Number"));
		while (curr_cam < 0 || curr_cam >= scene->cameras.size()) {
			AfxMessageBox(_T("Out of range"));
			curr_cam = stoi(dialogBox("Active Camera Number"));
		}
		scene->activeCamera = curr_cam;
		break;
	}
}

void normalMenu(int id) {
	switch (id)
	{
	case NORMAL_VERTEX:
		scene->showNormalsV();
		break;
	case NORMAL_FACE:
		scene->showNormalsF();
		break;
	}
}

void modelMenu(int id) {
	int curr_model;
	vec3 color;
	GLfloat emissive, diffuse, specular, alpha;
	switch (id)
	{
	case MODEL_ACTIVE:
		curr_model = stoi(dialogBox("Active Model Number"));
		while (curr_model < 0 || curr_model >= scene->models.size()) {
			AfxMessageBox(_T("Out of range"));
			curr_model = stoi(dialogBox("Active Model Number"));
		}
		scene->activeCamera = curr_model;
		scene->activeModel = curr_model;
		break;
	case MODEL_BBOX:
		scene->bbox();
		break;
	case MODEL_SURFACE:
		emissive = stof(dialogBox("Model Emissive Coefficient"));
		diffuse = stof(dialogBox("Model Diffuse Coefficient"));
		specular = stof(dialogBox("Model Specular Coefficient"));
		alpha = stof(dialogBox("Alpha"));
		scene->setSurface(emissive, diffuse, specular, alpha);
		break;
	}
}

void stepMenu(int id) {
	switch (id) {
	case STEP_ROTATE:
		scene->step_rotate = stof(dialogBox("Rotate Step"));
		break;
	case STEP_SCALE:
		scene->step_scale = stof(dialogBox("Scale Step"));
		break;
	case STEP_MOVE:
		scene->step_move = stof(dialogBox("Move Step"));
		break;
	}
}

void frameMenu(int id) {
	switch (id) {
	case FRAME_MODEL:
		scene->modelFrame('m');
		break;
	case FRAME_WORLD:
		scene->modelFrame('w');
		break;
	}
}

void camFrameMenu(int id) {
	switch (id) {
	case CAM_FRAME_VIEW:
		scene->camFrame('v');
		break;
	case CAM_FRAME_WORLD:
		scene->camFrame('w');
		break;
	}
}

void colorMenu(int id) {
	vec3 color = dialogBoxVec("Color");
	switch (id) {
	case COLOR_UNI:
		scene->color(color);
		break;
	case COLOR_XUNI:
		scene->color(color, false);
		break;
	}
}

void lightShadeMenu(int id) {
	switch (id) {
	case SHADE_FLAT:
		scene->shade("flat");
		break;
	case SHADE_GOUR:
		scene->shade("gour");
		break;
	case SHADE_PHONG:
		scene->shade("phong");
		break;
	}
}

void lightTypeMenu(int id) {
	switch (id) {
	case LIGHT_POINT:
		scene->setLightType("point");
		break;
	case LIGHT_PAR:
		scene->setLightType("parallel");
		break;
	case LIGHT_AMB:
		scene->setLightType("ambient");
		break;
	}
}

void lightMenu(int id) {
	int curr_light;
	vec3 ans;
	switch (id) {
	case LIGHT_ADD:
		scene->addLight();
		break;
	case LIGHT_CLEAR:
		scene->deactivateLight();
		break;
	case LIGHT_ACTIVE:
		curr_light = stoi(dialogBox("Active Light Number"));
		while (curr_light < 0 || curr_light >= scene->lights.size()){
			AfxMessageBox(_T("Out of range"));
			curr_light = stoi(dialogBox("Active Light Number"));
		}
		scene->activeLight = curr_light;
		break;
	case LIGHT_COLOR:
		ans = dialogBoxVec("Set Light Color");
		scene->colorLight(ans);
		break;
	case LIGHT_POSITION:
		ans = dialogBoxVec("Set Light Position");
		scene->positionLight(ans);
		break;
	case LIGHT_ORIENT:
		scene->orientLight();
		break;
	}
}

void projMenu(int id) {
	GLfloat left = 2, right = -2, bottom = 2, top = -2, zNear = 2, zFar = -2, fovy = 1, aspect = 2;
	switch (id) {
	case PROJ_ORT:
		left = stof(dialogBox("Left"));
		right = stof(dialogBox("Right"));
		bottom = stof(dialogBox("Bottom"));
		top = stof(dialogBox("Top"));
		zNear = stof(dialogBox("zNear"));
		zFar = stof(dialogBox("zFar"));
		scene->ortho(left, right, bottom, top, zNear, zFar);
		break;
	case PROJ_PRES:
		left = stof(dialogBox("Left"));
		right = stof(dialogBox("Right"));
		bottom = stof(dialogBox("Bottom"));
		top = stof(dialogBox("Top"));
		zNear = stof(dialogBox("zNear"));
		zFar = stof(dialogBox("zFar"));
		scene->frustum(left, right, bottom, top, zNear, zFar);
		break;
	case PROJ_GEO:
		fovy = stof(dialogBox("Fovy"));
		aspect = stof(dialogBox("Aspect Ration"));
		zNear = stof(dialogBox("zNear"));
		zFar = stof(dialogBox("zFar"));
		scene->perspective(fovy, aspect, zNear, zFar);
		break;
	}
	
}

void initMenu()
{
	const int menuFile = glutCreateMenu(fileMenu);
	glutAddMenuEntry("Open..", FILE_OPEN);

	const int projFile = glutCreateMenu(projMenu);
	glutAddMenuEntry("Orthogonal", PROJ_ORT);
	glutAddMenuEntry("Prespective", PROJ_PRES);
	glutAddMenuEntry("Prespective by Aspect Ratio", PROJ_GEO);

	const int camFrameFile = glutCreateMenu(camFrameMenu);
	glutAddMenuEntry("View", CAM_FRAME_VIEW);
	glutAddMenuEntry("World", CAM_FRAME_WORLD);

	const int camFile = glutCreateMenu(camMenu);
	glutAddMenuEntry("Add", CAM_ADD);
	glutAddMenuEntry("Set Active", CAM_ACTIVE);
	glutAddMenuEntry("Render", CAM_REND);
	glutAddSubMenu("Projection", projFile);
	glutAddSubMenu("Frame", camFrameFile);

	const int frameFile = glutCreateMenu(frameMenu);
	glutAddMenuEntry("Model", FRAME_MODEL);
	glutAddMenuEntry("World", FRAME_WORLD);

	const int normalFile = glutCreateMenu(normalMenu);
	glutAddMenuEntry("Face", NORMAL_FACE);
	glutAddMenuEntry("Vertex", NORMAL_VERTEX);

	const int colorFile = glutCreateMenu(colorMenu);
	glutAddMenuEntry("Uniform", COLOR_UNI);
	glutAddMenuEntry("Non-Uniform", COLOR_XUNI);

	const int modelFile = glutCreateMenu(modelMenu);
	glutAddMenuEntry("Set Active", MODEL_ACTIVE);
	glutAddSubMenu("Color", colorFile);
	glutAddSubMenu("Normals", normalFile);
	glutAddSubMenu("Frame", frameFile);
	glutAddMenuEntry("Bounding Box", MODEL_BBOX);
	glutAddMenuEntry("Surface Coefficient", MODEL_SURFACE);

	const int lightTypeFile = glutCreateMenu(lightTypeMenu);
	glutAddMenuEntry("Point", LIGHT_POINT);
	glutAddMenuEntry("Parallel", LIGHT_PAR);
	glutAddMenuEntry("Ambient", LIGHT_AMB);

	const int lightShadeFile = glutCreateMenu(lightShadeMenu);
	glutAddMenuEntry("Flat", SHADE_FLAT);
	glutAddMenuEntry("Gouraud", SHADE_GOUR);
	glutAddMenuEntry("Phong", SHADE_PHONG);

	const int lightFile = glutCreateMenu(lightMenu);
	glutAddMenuEntry("Add", LIGHT_ADD);
	glutAddMenuEntry("Deactivate", LIGHT_CLEAR);
	glutAddMenuEntry("Set Active", LIGHT_ACTIVE);
	glutAddMenuEntry("Color", LIGHT_COLOR);
	glutAddMenuEntry("Position", LIGHT_POSITION);
	glutAddMenuEntry("Orient", LIGHT_ORIENT);
	glutAddSubMenu("Type", lightTypeFile);
	glutAddSubMenu("Shade", lightShadeFile);

	const int stepFile = glutCreateMenu(stepMenu);
	glutAddMenuEntry("Rotate", STEP_ROTATE);
	glutAddMenuEntry("Scale", STEP_SCALE);
	glutAddMenuEntry("Move", STEP_MOVE);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("File", menuFile);
	glutAddMenuEntry("Add Primitve", MAIN_PRIM);
	glutAddSubMenu("Model", modelFile);
	glutAddSubMenu("Camera", camFile);	
	glutAddSubMenu("Light", lightFile);
	glutAddSubMenu("Set Step Size", stepFile);
	glutAddMenuEntry("Demo", MAIN_DEMO);
	glutAddMenuEntry("About", MAIN_ABOUT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
//----------------------------------------------------------------------------



int my_main(int argc, char** argv)
{
	//----------------------------------------------------------------------------
	// Initialize window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("CG");
	glewExperimental = GL_TRUE;
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		/*		...*/
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));


	renderer = new Renderer(512, 512);
	scene = new Scene(renderer);
	//----------------------------------------------------------------------------
	// Initialize Callbacks

	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(catchKey);
	initMenu();


	glutMainLoop();
	delete scene;
	delete renderer;
	return 0;
}

CWinApp theApp;

using namespace std;

int main(int argc, char** argv)
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		my_main(argc, argv);
	}

	return nRetCode;
}
