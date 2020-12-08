#pragma once
#include "scene.h"
#include "vec.h"
#include "mat.h"
#include <string>

using namespace std;


class MeshModel : public Model
{
protected :
	MeshModel() {}
	bool normal = false;
	void _add_line(vec3 v1, vec3 v2);
	GLfloat k = 1;
	
public:
	bool bbox = false;
	bool show_normalsV = false;
	bool show_normalsF = false;
	bool uniform = true;
	char frame = 'm';
	vec3 color = vec3(0.5,0.5,0.5);
	mat4 _world_transform;
	mat4 _normal_transform;
	mat4 _normal_world_transform;
	
	mat4 m_transform;
	mat4 m_translate;
	mat4 m_rotate;

	vec4 fraction = vec4(0.5,0.5,0.5,5); // surface coefficient. Ka, Kd, Ks, alpha

	GLfloat min_x;
	GLfloat min_y;
	GLfloat min_z;
	GLfloat max_z;
	GLfloat max_x;
	GLfloat max_y;



	MeshModel(string fileName);
	~MeshModel(void);
	void loadFile(string fileName);
	void draw();
	vector<vec3> vertex_positions;
	vector<vec3> vertex_normal;
	vector<vec3> vertices;
	vector<vec3> vertex_bbox;

	vector<vec3> v_normal;
	vector<vec3> f_normal;
	
};

class PrimMeshModel : public MeshModel
{
public:
	PrimMeshModel() : MeshModel("cube.obj"){}
};