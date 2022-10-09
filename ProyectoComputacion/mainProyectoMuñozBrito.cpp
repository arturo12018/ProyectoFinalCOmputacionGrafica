/*
Semestre 2022-1
Proyecto Teoria

Brito Serrano Miguel Ángel-Grupo 11
Muñoz García Arturo-Grupo 07

*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"


#include <irrKlang/irrKlang.h>


const float toRadians = 3.14159265f / 180.0f;


float movOffsetHeli2;
float anguloHeliZ;
float senoidal;
float rotllhelice;
float movCocheHeliX;
float movCocheHeliY;
bool movOffsetHeli;
bool movOffsetCoche;
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float rotqaHeliceOffset;
float rotahelice;

//Proyecto
float rotacionArbol;
float movXFerb;
float movZFerb;
float anguloYFerb;
float movXPhineas;
float movZPhineas;
float anguloYPhineas;

float movYFerb;
float anguloXFerb;
bool idaFerb;
float anguloZFerb;

float santaX;
float santaY;
float santaZ;
bool subidaSanta=true;
bool activaSanta = false;
float anguloYSanta = 0.0f;
float anguloXSanta;
float anguloZSanta;

//Animacion de Avatra
//Pie izquierdo
float rotacionPieIzq = 0.0f;
float posicionPieIzqX = 0.0f;
float posicionPieIzqY = 0.25f;
float timerPasosAvaatr = 0.0f;

//Pie derecho
float rotacionPieDer = 0.0f;
float posicionPieDerX = 0.0f;
float posicionPieDerY = 0.25f;

//Brazo Derecho
float rotacionBrazoDer = 0.0f;
float posicionBrazoDerX = 0.0f;
float posicionBrazoDerY = 0.25f;

//Brazo Izquierdo
float rotacionBrazoIz = 0.0f;
float posicionBrazoIzX = 0.0f;
float posicionBrazoIzY = 0.25f;



float tiempo1;
float tiempo2;

float rotaCamraRobot;

float movYCandase;
float movZCandase;
float anguloYCandase;
float contadorVueltasCandase;

float alturaArbol;
bool subidaArbolNavidad;
bool activaArbol=false;


//Avatar
float rotacionAvatraDerecha;
float rotacionAvatarIzquierda;
float posAvatarX;
float posAvatarZ;
int tipoCamara=0; //si es 0 camara global, si es 1 en 3ra persona y  si es 2 es camara global

//iluminaicon
bool dia = true;
bool lucesPorTeclado = false;
bool encendidasApagadas = true;
bool enciendeLucesShow = false;
bool showActivacion = false;
float timerShow = 0.0f;
float posLuz3X = 0.0f;
float posLuz3Y = 0.0f;
float posLuz3Z = 0.0f;
float posLuz4X = 0.0f;
float posLuz4Y = 0.0f;
float posLuz4Z = 0.0f;
float posLuz5X = 0.0f;
float posLuz5Y = 0.0f;
float posLuz5Z = 0.0f;
float posLuz6X = 0.0f;
float posLuz6Y = 0.0f;
float posLuz6Z = 0.0f;

//camara
float camaraAereaX;
float camaraAereaZ;
float camaraLibreX;
float camaraLibreY;
float camaraLibreZ;


//Kframe
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
Texture Madera;
Texture alfombra;

//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model Dado_M;
Model BlackhawkHelice_M;


//----Proyecto
Model Arbol;
Model Candace;
Model Santa_M;
Model Ferb_M;
Model Perry;
Model Phienas;
Model ArbolNavidad_M;

//Avatar
Model CabezaRobot_M;
Model BrazoDerechoRobot_M;
Model BrazoIzquierdoRobot_M;
Model CuerpoRobot_M;
Model PieDerechoRobot_M;
Model PieIzquierdoRobot_M;


Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();//audio

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);
void recorrido(bool* keys);//declaracion de l afuncion de recorrido
void camvioSkybox();
void animacionCandase();
void GiroArbol(bool* keys);
void iluminacion(bool* keys);
void animaiconSanta(bool* keys);
void soundTrack();
void iluminacionShow(bool* keys);
void showIluminacion(bool* keys);
void avatarPasos();
void avaatarParado();

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj7);

	

	
}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}



void CrearPrismaPoligonal()
{
	unsigned int prisma_indices[] = {
		// front 
		0, 1, 2,
		2, 3, 0,
		2, 3, 4,

		//right bottom
		5,6,7,
		7,8,5,

		//right up
		9,10,11,
		11,12,9,

		//left bottom 
		13,14,15,
		15,16,13,

		//left up
		17,18,19,
		19,20,17,

		//button
		21,22,23,
		23,24,21,

		//back
		25,26,27,
		27,28,25,
		27,28,29



		
		
	};
	GLfloat prisma_vertices[] = {

		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  0.0f,	0.01f,  0.01f,		0.0f,	0.0f,	-1.0f,	//0
		4.5f, -0.0f,  0.0f,		0.3f,	0.01f,		0.0f,	0.0f,	-1.0f,	//1
		4.5f,  7.5f,  0.0f,		0.3f,	0.5f,		0.0f,	0.0f,	-1.0f,	//2
		-0.0f,  7.5f,  0.0f,	0.01f,	0.5,		0.0f,	0.0f,	-1.0f,	//3
		2.25f,  9.5f,  0.0f,	0.15f,	0.7f,		0.0f,	0.0f,	-1.0f,	//4

		//right bottom
		//x		y		z		S		T			NX		NY		NZ
		4.5f, -0.0f,  0.0f,		0.31f,	0.01f,		0.0f,	0.0f,	-1.0f,	//5
		4.5f, -0.0f,  -1.0f,	0.4f,	0.01f,		0.0f,	0.0f,	-1.0f,	//6
		4.5f,  7.5f,  -1.0f,	0.4f,	0.5f,		0.0f,	0.0f,	-1.0f,	//7
		4.5f,  7.5f,  0.0f,		0.31f,	0.5f,		0.0f,	0.0f,	-1.0f,	//8

		//right up
		//x		y		z		S		T			NX		NY		NZ
		4.5f,  7.5f,  -1.0f,	0.21f,	0.51f,		0.0f,	0.0f,	-1.0f,	//9
		2.25f,  9.5f,  -1.0f,	0.4f,	0.7f,		0.0f,	0.0f,	-1.0f,	//10
		2.25f,  9.5f,  0.0f,	0.21f,	0.7f,		0.0f,	0.0f,	-1.0f,	//11
		4.5f,  7.5f,  0.0f,		0.4f,	0.51f,		0.0f,	0.0f,	-1.0f,	//12

		//left bottom
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  0.0f,	0.71f,  0.01f,		0.0f,	0.0f,	-1.0f,	//13
		-0.0f, -0.0f,  -1.0f,	0.8f,  0.01f,		0.0f,	0.0f,	-1.0f,	//14
		-0.0f,  7.5f,  -1.0f,	0.8f,	0.5f,		0.0f,	0.0f,	-1.0f,	//15
		-0.0f,  7.5f,  0.0f,	0.71,	0.5f,		0.0f,	0.0f,	-1.0f,	//16
		
		//left up
		//x		y		z		S		T			NX		NY		NZ
		-0.0f,  7.5f,  0.0f,	0.1f,	0.6f,		0.0f,	0.0f,	-1.0f,	//17
		2.25f,  9.5f,  0.0f,	0.1f,	0.8f,		0.0f,	0.0f,	-1.0f,	//18
		2.25f,  9.5f,  -1.0f,	0.01f,	0.8f,		0.0f,	0.0f,	-1.0f,	//19
		-0.0f,  7.5f,  -1.0f,	0.01f,	0.6f,		0.0f,	0.0f,	-1.0f,	//20

		//botton
		-0.0f, -0.0f,  0.0f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//21
		4.5f, -0.0f,  0.0f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//22
		4.5f, -0.0f,  -1.0f,	0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//23
		-0.0f, -0.0f,  -1.0f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//24

		//back
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  -1.0f,	0.7f,  0.01f,		0.0f,	0.0f,	-1.0f,	//25
		4.5f, -0.0f,  -1.0f,	0.41f,	0.01f,		0.0f,	0.0f,	-1.0f,	//26
		4.5f,  7.5f,  -1.0f,	0.41f,	0.5f,		0.0f,	0.0f,	-1.0f,	//27
		-0.0f,  7.5f,  -1.0f,	0.7f,	0.5f,		0.0f,	0.0f,	-1.0f,	//28
		2.25f,  9.5f,  -1.0f,	0.55f,	0.7f,		0.0f,	0.0f,	-1.0f,	//29
	
	
		
	
	};

	Mesh* prisma = new Mesh();
	prisma->CreateMesh(prisma_vertices, prisma_indices, 240, 48);
	meshList.push_back(prisma);

}

void CreaPrismaTrain()
{
	unsigned int prismaTrian_indices[] = {
		// front 
		0, 1, 2,
		
		//Back
		3,4,5,

		//right
		6,7,8,
		8,9,6,

		//left
		10,11,12,
		12,13,10,

		//botton
		14,15,16,
		16,17,14,


		







	};
	GLfloat prismaTrian_vertices[] = {

		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  0.0f,	0.371f,  0.24f,		0.0f,	0.0f,	-1.0f,	//0
		4.0f, -0.0f,  0.0f,		0.661f,	0.24f,		0.0f,	0.0f,	-1.0f,	//1
		2.0f,  3.0f,  0.0f,		0.532f,	0.0f,		0.0f,	0.0f,	-1.0f,	//2

		// back
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  -5.0f,	0.38f,  0.723f,		0.0f,	0.0f,	-1.0f,	//3
		4.0f, -0.0f,  -5.0f,	0.66f,	0.723f,		0.0f,	0.0f,	-1.0f,	//4
		2.0f,  3.0f, -5.0f,		0.525f,	1.0f,		0.0f,	0.0f,	-1.0f,	//5

		//right
		4.0f, -0.0f,  0.0f,		0.661f,	0.25f,		0.0f,	0.0f,	-1.0f,	//6
		4.0f, -0.0f,  -5.0f,	0.661f,	0.723f,		0.0f,	0.0f,	-1.0f,	//7
		2.0f,  3.0f, -5.0f,		0.95f,	0.723f,		0.0f,	0.0f,	-1.0f,	//8
		2.0f,  3.0f,  0.0f,		0.95f,	0.25f,		0.0f,	0.0f,	-1.0f,	//9

		//left
		-0.0f, -0.0f,  0.0f,	0.37f,  0.25f,		0.0f,	0.0f,	-1.0f,	//10
		-0.0f, -0.0f,  -5.0f,	0.37f,  0.723f,		0.0f,	0.0f,	-1.0f,	//11
		2.0f,  3.0f, -5.0f,		0.1f,	0.723f,		0.0f,	0.0f,	-1.0f,	//12
		2.0f,  3.0f,  0.0f,		0.1f,	0.25f,		0.0f,	0.0f,	-1.0f,	//13

		//botton
		-0.0f, -0.0f,  0.0f,	0.371f,  0.25f,		0.0f,	0.0f,	-1.0f,	//14
		-0.0f, -0.0f,  -5.0f,	0.371f,  0.723f,		0.0f,	0.0f,	-1.0f,	//15
		4.0f, -0.0f,  -5.0f,	0.66f,	0.723f,		0.0f,	0.0f,	-1.0f,//16
		4.0f, -0.0f,  0.0f,		0.66f,	0.25f,		0.0f,	0.0f,	-1.0f,	//17

	};

	Mesh* prismaTrian = new Mesh();
	prismaTrian->CreateMesh(prismaTrian_vertices, prismaTrian_indices, 144, 24);
	meshList.push_back(prismaTrian);

}

void CrearToroide()
{
	unsigned int toroide_indices[] = {
		// front 
		0, 1, 2,
		2, 3, 0,
		2, 3, 4,







	};
	GLfloat toroide_vertices[] = {

		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.0f, -0.0f,  0.0f,	0.01f,  0.01f,		0.0f,	0.0f,	-1.0f,	//0
		4.5f, -0.0f,  0.0f,		0.3f,	0.01f,		0.0f,	0.0f,	-1.0f,	//1
		4.5f,  7.5f,  0.0f,		0.3f,	0.5f,		0.0f,	0.0f,	-1.0f,	//2
		-0.0f,  7.5f,  0.0f,	0.01f,	0.5,		0.0f,	0.0f,	-1.0f,	//3
		2.25f,  9.5f,  0.0f,	0.15f,	0.7f,		0.0f,	0.0f,	-1.0f,	//4

	};

	Mesh* prismaTrian = new Mesh();
	prismaTrian->CreateMesh(toroide_vertices, toroide_indices, 144, 24);
	meshList.push_back(prismaTrian);
	
}




void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//Hace que haga un recorrido cuadrado
void animacionPhineas() {
	if (movZPhineas < 20.0f && movXPhineas == 0.0f) {
		if (movZPhineas >= 19.7f && movZPhineas < 20.0f) {
			if (anguloYPhineas < 90) {
				anguloYPhineas += 0.5f * deltaTime * 50;
				movZPhineas = 19.9f;
			}
			else {
				anguloYPhineas = 90;
				movZPhineas = 20.0f;
			}
		}
		else {
			movZPhineas += 0.5f * deltaTime ;
		}

	}

	if (movXPhineas < 10 && movZPhineas == 20.0f) {
		if (movXPhineas >= 9.7 && movXPhineas < 10) {
			if (anguloYPhineas >= 90 && anguloYPhineas < 180) {
				anguloYPhineas += movOffset * deltaTime * 50;
				movXPhineas = 9.9;
			}
			else {
				anguloYPhineas = 180;
				movXPhineas = 10.0f;
			}
		}
		else {
			movXPhineas += movOffset * deltaTime ;
		}

	}

	if (movZPhineas > 0 && movXPhineas == 10.0f) {
		if (movZPhineas <= 0.3 && movZPhineas > 0) {
			if (anguloYPhineas >= 180.0f && anguloYPhineas < 270.0f) {
				anguloYPhineas += movOffset * deltaTime * 50;
				movZPhineas = 0.1;
			}
			else {
				anguloYPhineas = 270;
				movZPhineas = 0;
			}
		}
		else {
			movZPhineas -= movOffset * deltaTime ;
		}

	}

	if (movZPhineas == 0.0f && movXPhineas > 0.0f) {
		if (movXPhineas <= 0.3 && movXPhineas > 0) {
			if (anguloYPhineas >= 270.0f && anguloYPhineas < 360.0f) {
				anguloYPhineas += movOffset * deltaTime * 50;
				movXPhineas = 0.1;
			}
			else {
				anguloYPhineas = 0;
				movXPhineas = 0;
			}
		}
		else {
			movXPhineas -= movOffset * deltaTime ;
		}

	}
}

//Animacion Ferb se lanza
void animacionFerb(){

	if (movZFerb < 20 && idaFerb) {
		
		if (movZFerb >= 19.8 && movZFerb < 20  ) {
			movZFerb = 20;
		}
		else {
			movZFerb += movOffset * deltaTime;
		}
	}

	if (movZFerb >= 20 && movZFerb <26  && idaFerb) { 
		
		if (anguloXFerb < 90) {
			movZFerb += movOffset * deltaTime;
			anguloXFerb += movOffset * deltaTime * 30;
			movYFerb += movOffset * deltaTime;

			if (anguloXFerb >= 88.5 && anguloXFerb < 90) {
				anguloXFerb = 90.0f;
			}
		}
			
			else if (movYFerb>0 && anguloXFerb==90)
			{
				movYFerb -= movOffset * deltaTime;
				movZFerb += movOffset * deltaTime;
				if (movYFerb <= 0.1 && movYFerb > 0) {
					movYFerb = 0.0f;
					movZFerb = 26.0f;
					SoundEngine->play2D("audio/golpeFerb.mp3", false);
				}
			}
				
		}
	
	
		if (movZFerb==26.0f && movYFerb==0.0f && anguloXFerb > 0 && idaFerb) {
			if(anguloXFerb > 0.1)
				anguloXFerb -= movOffset * deltaTime * 30;
			else {
				anguloXFerb = 0.0f;
			}
		}


		if (movZFerb == 26.0f && movYFerb == 0.0f &&  anguloYFerb < 180 ) {
			
			//printf("%f\n", anguloYFerb);--si sale error es el angulo restar o aumentar
			if (anguloYFerb >= 178.0f && anguloYFerb < 180.0f) {
				anguloYFerb = 180.0f;
				idaFerb = false;
			}
			else {

				anguloYFerb += movOffset * deltaTime * 30;
				

			}
		}
		
		
		if (idaFerb == false && movZFerb>0.0f ) {
			
			if (movZFerb<=0.3f && movZFerb>0) {
				movZFerb = 0.0f;
			}
			else {
				movZFerb -= movOffset * deltaTime;
			}

		}


		if (idaFerb == false && movZFerb==0.0f ) {
			if (anguloYFerb > 0.2f) {
				anguloYFerb -= movOffset * deltaTime*30;
			}
			else {
				idaFerb = true;
				anguloYFerb = 0.0f;
			}
			
		}
		
		
	}
	
	








//Animacion de la clase


void animacionCohe() {
	
	

	
	glm::vec3 carropos = glm::vec3(movCoche, -1.3f, 0.7f);

	
	
	if (movCoche < 50.0f && movOffsetCoche) {


		//spotLights[1].SetFlash(carropos + glm::vec3(-1.0f, 0.0f, -1.3f), glm::vec3(1.0, 0.0f, 0.0f));//(posicion,dondeilumina)
		//spotLights[2].SetFlash(carropos + glm::vec3(-1.0f, 0.0f, -0.3f), glm::vec3(1.0, 0.0f, 0.0f));//(posicion,dondeilumina)

		movCoche += movOffset * deltaTime ;
	rotllanta += rotllantaOffset * deltaTime ;
		if (movCoche >= 49.0f && movCoche < 50.0f)
			movOffsetCoche = false;
	}
	else if (movCoche > -45.0f && movOffsetCoche == false) {
		//printf("Regreso");
		movCoche -= movOffset * deltaTime ;
		rotllanta -= rotllantaOffset * deltaTime;

		//spotLights[1].SetFlash(carropos + glm::vec3(-1.0f, 0.0f, -1.3f), glm::vec3(-1.0, 0.0f, 0.0f));//(posicion,dondeilumina)
		//spotLights[2].SetFlash(carropos + glm::vec3(-1.0f, 0.0f, -0.3f), glm::vec3(-1.0, 0.0f, 0.0f));//(posicion,dondeilumina)

		if (movCoche <= -44.0f && movCoche > -45.0f)
			movOffsetCoche = true;
	}
}



///////////////////////////////KEYFRAMES/////////////////////


bool animacion = false;



//NEW// Keyframes
float posXavion = 2.0, posYavion = 2.0, posZavion = 0, movXPerry=0.0f, moYPerry = 0.0f, moZPerry = 0.0f, rotacionXPerry=0.0f, rotacionYPerry = 0.0f, rotacionZPerry = 0.0f;
float	movAvion_x = 0.0f, movAvion_y = 0.0f, movXPerry_Inc = 0.0f, moYPerry_Inc = 0.0f, moZPerry_Inc = 0.0f, rotacionXPerry_Inc = 0.0f, rotacionYPerry_Inc = 0.0f, rotacionZPerry_Inc = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 30
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;

	float movXPerry;
	float moYPerry;
	float moZPerry;
	float rotacionXPerry;
	float rotacionYPerry;
	float rotacionZPerry;

	float movXPerry_Inc;
	float moYPerry_Inc;
	float moZPerry_Inc;
	float rotacionXPerry_Inc;
	float rotacionYPerry_Inc;
	float rotacionZPerry_Inc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 13;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion = giroAvion;
	KeyFrame[FrameIndex].movXPerry = movXPerry;
	KeyFrame[FrameIndex].moYPerry = moYPerry;
	KeyFrame[FrameIndex].moZPerry = moZPerry;
	KeyFrame[FrameIndex].rotacionXPerry = rotacionXPerry;
	KeyFrame[FrameIndex].rotacionYPerry = rotacionYPerry;
	KeyFrame[FrameIndex].rotacionZPerry = rotacionZPerry;

	FrameIndex++;
}

void resetElements(void)
{




	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
	movXPerry = KeyFrame[0].movXPerry;
	moYPerry = KeyFrame[0].moYPerry;
	moZPerry = KeyFrame[0].moZPerry;
	rotacionXPerry = KeyFrame[0].rotacionXPerry;
	rotacionYPerry = KeyFrame[0].rotacionYPerry;
	rotacionZPerry = KeyFrame[0].rotacionZPerry;


}

void interpolation(void)
{


	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;
	KeyFrame[playIndex].movXPerry_Inc = (KeyFrame[playIndex + 1].movXPerry- KeyFrame[playIndex].movXPerry) / i_max_steps;
	KeyFrame[playIndex].moYPerry_Inc = (KeyFrame[playIndex + 1].moYPerry- KeyFrame[playIndex].moYPerry) / i_max_steps;
	KeyFrame[playIndex].moZPerry_Inc = (KeyFrame[playIndex + 1].moZPerry- KeyFrame[playIndex].moZPerry) / i_max_steps;
	KeyFrame[playIndex].rotacionXPerry_Inc = (KeyFrame[playIndex + 1].rotacionXPerry- KeyFrame[playIndex].rotacionXPerry) / i_max_steps;
	KeyFrame[playIndex].rotacionYPerry_Inc = (KeyFrame[playIndex + 1].rotacionYPerry- KeyFrame[playIndex].rotacionYPerry) / i_max_steps;
	KeyFrame[playIndex].rotacionZPerry_Inc = (KeyFrame[playIndex + 1].rotacionZPerry- KeyFrame[playIndex].rotacionZPerry) / i_max_steps;

}


void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aqu�\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se qued� aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			movXPerry += KeyFrame[playIndex].movXPerry_Inc;
			moYPerry += KeyFrame[playIndex].moYPerry_Inc;
			moZPerry += KeyFrame[playIndex].moZPerry_Inc;
			rotacionXPerry += KeyFrame[playIndex].rotacionXPerry_Inc;
			rotacionYPerry += KeyFrame[playIndex].rotacionYPerry_Inc;
			rotacionZPerry += KeyFrame[playIndex].rotacionZPerry_Inc;
	

			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/




int main()
{
	

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CrearPrismaPoligonal();
	CreaPrismaTrain();
	CrearToroide();
	CreateShaders();
	


	camera = Camera(glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 7.0f, 0.5f);

	

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	dadoTexture = Texture("Textures/dado.tga");
	dadoTexture.LoadTextureA();
	Madera= Texture("Textures/madera.tga");
	Madera.LoadTextureA();
	alfombra = Texture("Textures/alfombraRoja.tga");
	alfombra.LoadTextureA();
	
	//pisoTexture = Texture("Textures/piso.tga");
	//pisoTexture.LoadTextureA();
	pisoTexture = Texture("Textures/Skybox/pasto.jpg");
	pisoTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/helicopteroCuerpo.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");
	BlackhawkHelice_M = Model();
	BlackhawkHelice_M.LoadModel("Models/helicopteroHelice.obj");



	//------Proyecto
	Arbol = Model();
	Arbol.LoadModel("Models/arbol.obj");
	Candace = Model();
	Candace.LoadModel("Models/candace.obj");

	Santa_M = Model();
	Santa_M.LoadModel("Models/santa.obj");
	Ferb_M = Model();
	Ferb_M.LoadModel("Models/ferbo.obj");
	Perry = Model();
	Perry.LoadModel("Models/perry.obj");
	Phienas = Model();
	Phienas.LoadModel("Models/phineas.obj");
	ArbolNavidad_M = Model();
	ArbolNavidad_M.LoadModel("Models/arbolNavidad.obj");


	CabezaRobot_M = Model();
	CabezaRobot_M.LoadModel("Models/Cabeza_Robot.obj");
	BrazoDerechoRobot_M = Model();
	BrazoDerechoRobot_M.LoadModel("Models/BrazoDerecho_Robot.obj");
	BrazoIzquierdoRobot_M = Model();
	BrazoIzquierdoRobot_M.LoadModel("Models/BrazoIzquierdo_Robot.obj");
	CuerpoRobot_M = Model();
	CuerpoRobot_M.LoadModel("Models/BrazoCuerpo_Robot.obj");
	PieDerechoRobot_M = Model();
	PieDerechoRobot_M.LoadModel("Models/PieDercho_Robot.obj");
	PieIzquierdoRobot_M = Model();
	PieIzquierdoRobot_M.LoadModel("Models/PieIzquierdo_Robot.obj");





	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	/*
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija

	spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		30.0f);
	spotLightCount++;


	//Iluminacion para el show de luces

	spotLights[3] = SpotLight(0.0f,0.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	
	spotLights[4] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	

	spotLights[5] = SpotLight(1.0f, 0.0f, 0.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	spotLights[6] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	



	//luz de faro
	 //luz de helicóptero

	
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	
	
	std::vector<std::string> skyboxFaces;

	//patio phienas y ferb dia
	skyboxFaces.push_back("Textures/Skybox/PatioIzq.jpg");
	skyboxFaces.push_back("Textures/Skybox/patioDer.jpg");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.jpg");
	skyboxFaces.push_back("Textures/Skybox/patioAtras.jpg");
	skyboxFaces.push_back("Textures/Skybox/patioAdel.jpg");

	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	//KEYFRAMES DECLARADOS INICIALES



	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0.0f;
	KeyFrame[0].rotacionXPerry = 0.0f;
	KeyFrame[0].moYPerry = 0.0f;
	KeyFrame[0].moZPerry = 0.0f;
	KeyFrame[0].rotacionYPerry = 0.0f;
	KeyFrame[0].movXPerry = 0.0f;



	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 2.0f;
	KeyFrame[1].giroAvion = 0;
	KeyFrame[1].rotacionXPerry = 90.0f;
	KeyFrame[1].moYPerry = 2.5f;
	KeyFrame[1].moZPerry = 2.5f;
	KeyFrame[1].rotacionYPerry = 0.0f;
	KeyFrame[1].movXPerry = 0.0f;


	KeyFrame[2].movAvion_x = 2.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;
	KeyFrame[2].rotacionXPerry = 180.0f;
	KeyFrame[2].moYPerry = 5.0f;
	KeyFrame[2].moZPerry = 5.0f;
	KeyFrame[2].rotacionYPerry = 0.0f;
	KeyFrame[2].movXPerry = 0.0f;


	KeyFrame[3].movAvion_x = 3.0f;
	KeyFrame[3].movAvion_y = -2.0f;
	KeyFrame[3].giroAvion = 0;
	KeyFrame[3].rotacionXPerry = 270.0f;
	KeyFrame[3].moYPerry = 2.5f;
	KeyFrame[3].moZPerry = 7.5f;
	KeyFrame[3].rotacionYPerry = 0.0f;
	KeyFrame[3].movXPerry = 0.0f;

	KeyFrame[4].movAvion_x = 4.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 0.0f;
	KeyFrame[4].rotacionXPerry = 360.0f;
	KeyFrame[4].moYPerry = 0.0f;
	KeyFrame[4].moZPerry = 10.0f;
	KeyFrame[4].rotacionYPerry = 0.0f;
	KeyFrame[4].movXPerry = 0.0f;

	KeyFrame[5].movAvion_x = 5.0f;
	KeyFrame[5].movAvion_y = 2.0f;
	KeyFrame[5].giroAvion = 0.0f;
	KeyFrame[5].rotacionXPerry = 360.0f;
	KeyFrame[5].moYPerry = 0.0f;
	KeyFrame[5].moZPerry = 10.0f;
	KeyFrame[5].rotacionYPerry = -90.0f;
	KeyFrame[5].movXPerry = 0.0f;
	


	KeyFrame[6].movAvion_x = 6.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].giroAvion = 180.0f;
	KeyFrame[6].rotacionXPerry = 360.0f;
	KeyFrame[6].moYPerry = 0.0f;
	KeyFrame[6].moZPerry = 10.0f;
	KeyFrame[6].rotacionYPerry = -90.0f;
	KeyFrame[6].movXPerry = -5.0f;


	KeyFrame[7].movAvion_x = 5.0f;
	KeyFrame[7].movAvion_y = -2.0f;
	KeyFrame[7].giroAvion = 180.0f;
	KeyFrame[7].rotacionXPerry = 360.0f;
	KeyFrame[7].moYPerry = 0.0f;
	KeyFrame[7].moZPerry = 0.0f;
	KeyFrame[7].rotacionYPerry = -180.0f;
	KeyFrame[7].movXPerry = -5.0f;

	KeyFrame[8].movAvion_x = 4.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].giroAvion = 180.0f;
	KeyFrame[8].rotacionXPerry = 380.0f;
	KeyFrame[8].moYPerry = 3.0f;
	KeyFrame[8].moZPerry = -1.0f;
	KeyFrame[8].rotacionYPerry = -180.0f;
	KeyFrame[8].movXPerry = -5.0f;

	KeyFrame[9].movAvion_x = 3.0f;
	KeyFrame[9].movAvion_y = 2.0f;
	KeyFrame[9].giroAvion = 180.0f;
	KeyFrame[9].rotacionXPerry = 360.0f;
	KeyFrame[9].moYPerry = 0.0f;
	KeyFrame[9].moZPerry = -1.0f;
	KeyFrame[9].rotacionYPerry = -180.0f;
	KeyFrame[9].movXPerry = -5.0f;

	KeyFrame[10].movAvion_x = 2.0f;
	KeyFrame[10].movAvion_y = 0.0f;
	KeyFrame[10].giroAvion = 180.0f;
	KeyFrame[10].rotacionXPerry = 360.0f;
	KeyFrame[10].moYPerry = 0.0f;
	KeyFrame[10].moZPerry = 0.0f;
	KeyFrame[10].rotacionYPerry = -360.0f;
	KeyFrame[10].movXPerry = 0.0f;


	KeyFrame[11].movAvion_x = 1.0f;
	KeyFrame[11].movAvion_y = -2.0f;
	KeyFrame[11].giroAvion = 180.0f;
	KeyFrame[11].rotacionXPerry = 360.0f;
	KeyFrame[11].moYPerry = 0.0f;
	KeyFrame[11].moZPerry = 0.0f;
	KeyFrame[11].rotacionYPerry = -360.0f;
	KeyFrame[11].movXPerry = 0.0f;

	KeyFrame[12].movAvion_x = 1.0f;
	KeyFrame[12].movAvion_y = 0.0f;
	KeyFrame[12].giroAvion = 0.0f;
	KeyFrame[12].rotacionXPerry = 360.0f;
	KeyFrame[12].moYPerry = 0.0f;
	KeyFrame[12].moZPerry = 0.0f;
	KeyFrame[12].rotacionYPerry = -360.0f;
	KeyFrame[12].movXPerry = 0.0f;


	//Agregar Kefyrame[5] para que el avi�n regrese al inicio



	movOffsetCoche = true;
	movCoche = 0.0f;
	movOffsetHeli2 = 0.5f;
	movOffset = 0.5f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;
	movCocheHeliX = 0.0f;
	movCocheHeliY = 0.0f;
	movOffsetHeli = true;
	rotllhelice = 0.0f;
	senoidal = 0.0f;
	anguloHeliZ = -90.0f;
	rotqaHeliceOffset = 10.0f;
	anguloZFerb = 0.0f;


	//pryecto
	rotacionArbol = 0.0f;
	movXFerb = 0.0f;
	movYFerb = 0.0f;
	movZFerb = 0.0f;
	anguloYFerb = 0.0f;
	movXPhineas = 0.0f;
	movZPhineas = 0.0f;
	anguloYPhineas = 0.0f;
	anguloXFerb = 0.0f;
	idaFerb = true;
	tiempo1=0.0f;
	tiempo2=0.0f;

	alturaArbol = 0.0f;
	subidaArbolNavidad = true;

	movYCandase=0.0f; 
	movZCandase=0.0f;
	anguloYCandase=0.0f;
	contadorVueltasCandase = 0.0f;

	santaX = 0.0f;
	santaY = 0.0f;
	santaZ = 0.0f;
	anguloYSanta = 0.0f;
	anguloXSanta = 0.0f;
	anguloZSanta = 0.0f;

	//camara
	camaraAereaX=0.0f;
	camaraAereaZ=0.0f;
	camaraLibreX=0.0f;
	camaraLibreY=1.5f;
	camaraLibreZ=0.0f;
	


	//Avatar
	rotacionAvatraDerecha = 0.0f;
	rotacionAvatarIzquierda = 0.0f;

	posAvatarX = 0.0f;
	posAvatarZ = 3.0f;

	rotaCamraRobot = 0.0f;

	soundTrack();
	

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		

		tiempo1 += deltaTime;
		if (tiempo1 > 30) {
			camvioSkybox();
			tiempo1 = 0;
		}

		tiempo2 += deltaTime;
		if (tiempo2 > 87) {
			soundTrack();
			tiempo2 = 0;
		}
		//printf("%f\n", tiempo2);

		iluminacion(mainWindow.getsKeys());

		rotahelice += 30 * 10 * deltaTime;
		

		

		GiroArbol(mainWindow.getsKeys());//animaicon arbol
		animaiconSanta(mainWindow.getsKeys());//animacion sanat
		iluminacionShow(mainWindow.getsKeys());//prender y apagar iluminacion de show
		showIluminacion(mainWindow.getsKeys());

		animacionPhineas();//animacion de Phineas
		animacionCohe();//animacion coche
		animacionFerb();//animacion ferb
		animacionCandase();//animacion candase


		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime, tipoCamara);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange(), tipoCamara);
		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		recorrido(mainWindow.getsKeys());//Funcion del recorrido
		animate();
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		//spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		
		



		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoTexture.UseTexture();
		//plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		



		//modelaux = glm::mat4(1.0);
		model = glm::mat4(1.0);
		modelaux = model = glm::translate(model, glm::vec3(movCoche, -1.3f, 0.7f));
		//modelaux = model;
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		/*model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();



		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.43, -0.1f, -0.27f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.07f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Llanta_M.RenderModel();
		//Dado_M.RenderModel();


		glm::mat4 modelauxhelicoptero(1.0);//matriz auxiliar para jerarquia de helicoptero
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, giroAvion * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelauxhelicoptero = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Blackhawk_M.RenderModel();
		//spotLights[3].SetPos(posblackhawk);

		//Helice Helicoptero
		model = modelauxhelicoptero;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.2f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, mainWindow.getrotahelice() * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotahelice * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//agregar material al helic�ptero
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BlackhawkHelice_M.RenderModel();



		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.5f, -0.1f));
		model = glm::scale(model, glm::vec3(4.1f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Camino_M.RenderModel();

		
		for (int x = -45; x <= 50; x += 4) {
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(x, -2.0f, -1.0f));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Madera.UseTexture();
			meshList[8]->RenderMesh();
			}
			
		
		
		

		//---------------------------Proyecto-------------------------------------------------------------


			
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -2.0f, 5.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Arbol.RenderModel();





		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, movYCandase+1.0f, movZCandase +10.0f));
		model = glm::scale(model, glm::vec3(2.9f, 2.9f, 2.9f));
		model = glm::rotate(model, anguloYCandase * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Candace.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(santaX-10.0f, santaY-2.0f, santaZ+5.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, anguloYSanta * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, anguloXSanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, anguloZSanta * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Santa_M.RenderModel();


		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(movXFerb + 7.0f, movYFerb -2.0f, movZFerb));
		model = glm::scale(model, glm::vec3(0.23f, 0.23f, 0.23f));
		model = glm::rotate(model, anguloYFerb * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, anguloXFerb * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, anguloZFerb * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Ferb_M.RenderModel();

		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movXPerry -5.0f, moYPerry -2.0f, moZPerry +15.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, rotacionXPerry * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotacionYPerry * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotacionZPerry * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Perry.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movXPhineas + 10.0f, -2.0f, movZPhineas + 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, anguloYPhineas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Phienas.RenderModel();


		glm::mat4 modelauxArbolNav(1.0);//matriz auxiliar para jerarquia de helicoptero


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, alturaArbol-2.0f, 5.0f));
		modelauxArbolNav = model;
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		model = glm::rotate(model, rotacionArbol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		ArbolNavidad_M.RenderModel();


		//Base arbol
		model = modelauxArbolNav;
		model = glm::translate(model, glm::vec3(4.1f, 0.0f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		alfombra.UseTexture();
		meshList[9]->RenderMesh();


		//Avatar--Robot

		//Cabeza cuerpo
		glm::mat4 modelauxRobot(1.0);//matriz auxiliar para jerarquia de helicoptero
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posAvatarX +5.0f, -3.2f, posAvatarZ +0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, rotaCamraRobot + 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		modelauxRobot = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CabezaRobot_M.RenderModel();



		//Brazo derecho
		model = modelauxRobot;
		model = glm::translate(model, glm::vec3(posicionBrazoDerX, posicionBrazoDerY, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, rotacionBrazoDer * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BrazoDerechoRobot_M.RenderModel();


		

		//Brazo Izquierdp
		model = modelauxRobot;
		model = glm::translate(model, glm::vec3(posicionBrazoIzX, posicionBrazoIzY, -0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, rotacionBrazoIz * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BrazoIzquierdoRobot_M.RenderModel();

	
		//Cuerpo
		model = modelauxRobot;
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CuerpoRobot_M.RenderModel();


		//Pie derecho
		model = modelauxRobot;
		model = glm::translate(model, glm::vec3(posicionPieDerX, posicionPieDerY, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, rotacionPieDer * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PieDerechoRobot_M.RenderModel();


		//Pie Izquierdo
		model = modelauxRobot;
		model = glm::translate(model, glm::vec3(posicionPieIzqX, posicionPieIzqY, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::rotate(model, rotacionPieIzq * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PieIzquierdoRobot_M.RenderModel();


		//Agabe 
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.7f, -2.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 0 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Tagave.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
		
	}

	SoundEngine->drop();
	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animaci�n'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}




	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}


}


void recorrido(bool* keys){
	//Camara libre por defecto 

	//Camara Avatar
	if (keys[GLFW_KEY_Z]) {
		camera = Camera(glm::vec3(posAvatarX-10.0f, 5.0f, posAvatarZ), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 7.0f, 0.5f);
		tipoCamara=1;
		SoundEngine->play2D("audio/bleep.mp3", false);
	}

	//Camara Aerea
	else if (keys[GLFW_KEY_X]) {
		camera = Camera(glm::vec3(camaraAereaX, 22.0f, camaraAereaZ), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 7.0f, 0.5f);
		tipoCamara = 2;
		SoundEngine->play2D("audio/bleep.mp3", false);
	}


	//Camara libre
	else if (keys[GLFW_KEY_C] ) {
		camera = Camera(glm::vec3(camaraLibreX, camaraLibreY, camaraLibreZ), glm::vec3(0.0f, 1.5f, 0.0f), -60.0f, 0.0f, 7.0f, 0.5f);
		tipoCamara = 0;
		SoundEngine->play2D("audio/bleep.mp3", false);
	}

	if (tipoCamara == 0) {
		camaraLibreX = camera.getCameraPosition().x;
		camaraLibreY = camera.getCameraPosition().y;
		camaraLibreZ = camera.getCameraPosition().z;
	}

	//va almacenando la ubicacion de la camara aerea
	if (tipoCamara == 2) {
		camaraAereaX = camera.getCameraPosition().x;
		camaraAereaZ= camera.getCameraPosition().z;
	}

	//Animacion moviemineto avatar
	if (tipoCamara==1) {
		posAvatarX = camera.getCameraPosition().x+10.0f;
		posAvatarZ = camera.getCameraPosition().z;
		
		
		//Giro cuerpo
		if (keys[GLFW_KEY_D] || keys[GLFW_KEY_A] || keys[GLFW_KEY_S] || keys[GLFW_KEY_W]) {
			if (keys[GLFW_KEY_W]) {
				avatarPasos();
			}
			if (keys[GLFW_KEY_D]) {
				rotaCamraRobot = 80.0f;
				avatarPasos();

			}
			if (keys[GLFW_KEY_A]) {
				rotaCamraRobot = -80.0f;
				avatarPasos();
			}
			if (keys[GLFW_KEY_S]) {
				rotaCamraRobot = -160.0f;
				avatarPasos();
			}
			if (keys[GLFW_KEY_A] && keys[GLFW_KEY_W]) {
				rotaCamraRobot = 240.0f;
				//avatarPasos();
			}
			if (keys[GLFW_KEY_D] && keys[GLFW_KEY_W]) {
				rotaCamraRobot = -240.0f;
				//avatarPasos();
			}
		}
		else {
			rotaCamraRobot = 0.0f;
			
			avaatarParado();
		}
		

	}
	
}

//Cambia skybox
void camvioSkybox(){


	if (dia==false) {
		std::vector<std::string> skyboxFaces;
		skyboxFaces.push_back("Textures/Skybox/PatioIzq.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioDer.jpg");
		skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
		skyboxFaces.push_back("Textures/Skybox/cielo.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioAtras.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioAdel.jpg");
		skybox = Skybox(skyboxFaces);
		SoundEngine->play2D("audio/Gallo.mp3", false);
		dia = true;
		return;
	}
	else if(dia) {
		std::vector<std::string> skyboxFaces;
		skyboxFaces.push_back("Textures/Skybox/PatioIzq_noche.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioDer_noche.jpg");
		skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
		skyboxFaces.push_back("Textures/Skybox/cielo_noche.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioAtras_noche.jpg");
		skyboxFaces.push_back("Textures/Skybox/patioAdel_noche.jpg");
		skybox = Skybox(skyboxFaces);
		SoundEngine->play2D("audio/grillos.mp3", false);
		dia = false;

		
	}
	return;

	
	
}

void iluminacion(bool* keys){
	if (keys[GLFW_KEY_T] && encendidasApagadas) {
		encendidasApagadas = false;
		
	}
	else if (keys[GLFW_KEY_T] && encendidasApagadas == false) {
		encendidasApagadas = true;
		
	}

	else if (keys[GLFW_KEY_R] && lucesPorTeclado == false) {
		lucesPorTeclado = true;
		

		

	}
	else if (keys[GLFW_KEY_R] && lucesPorTeclado) {
		lucesPorTeclado = false;
	}


	if (lucesPorTeclado == false) {
		if (dia) {
			//Luz Camara
			glm::vec3 lowerLight = glm::vec3(posAvatarX + 5.0f, -3.2f, posAvatarZ + 0.0f);
			spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, 0.0f));

			//Luz phineas
			glm::vec3 phineasPosLuz = glm::vec3(movXPhineas, 4.0f, movZPhineas);
			spotLights[1].SetFlash(phineasPosLuz + glm::vec3(10.0, 0.0f, 0.2f), glm::vec3(0.0, 0.0f, 0.0f));//(posicion,dondeilumina)

			//Luz Ferb
			glm::vec3 ferbPosLuz = glm::vec3(movXFerb + 7.0f, 4.0f, movZFerb);
			spotLights[2].SetFlash(ferbPosLuz + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0f, 0.0f));//(posicion,dondeilumina)
			
		}

		else {


			if ((keys[GLFW_KEY_D] || keys[GLFW_KEY_A]) && tipoCamara == 1) {
				if (keys[GLFW_KEY_D]) {
					glm::vec3 lowerLight = glm::vec3(posAvatarX + 5.0, 4.0f, posAvatarZ - 15.0f);
					spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, 1.0f));
				}
				if (keys[GLFW_KEY_A]) {
					glm::vec3 lowerLight = glm::vec3(posAvatarX + 10.0f, 4.0f, posAvatarZ + 15.0f);
					spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, -1.0f));
				}
			}
			else {
				glm::vec3 lowerLight = glm::vec3(posAvatarX - 5.0f, 4.0f, posAvatarZ);
				spotLights[0].SetFlash(lowerLight, glm::vec3(1.0, 0.0f, 0.0f));

			}

			//Luz Phieneas
			glm::vec3 phineasPosLuz = glm::vec3(movXPhineas, 4.0f, movZPhineas);
			spotLights[1].SetFlash(phineasPosLuz + glm::vec3(10.0, 0.0f, 0.2f), glm::vec3(0.0, -1.0f, 0.0f));//(posicion,dondeilumina)


			//Luz Ferb
			glm::vec3 ferbPosLuz = glm::vec3(movXFerb + 7.0f, 4.0f, movZFerb);
			spotLights[2].SetFlash(ferbPosLuz + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, -1.0f, 0.0f));//(posicion,dondeilumina)

		}

	}


	if (lucesPorTeclado ) {

		

		if (encendidasApagadas) {
			if ((keys[GLFW_KEY_D] || keys[GLFW_KEY_A]) && tipoCamara == 1) {
				if (keys[GLFW_KEY_D]) {
					glm::vec3 lowerLight = glm::vec3(posAvatarX + 5.0, 4.0f, posAvatarZ - 15.0f);
					spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, 1.0f));
				}
				if (keys[GLFW_KEY_A]) {
					glm::vec3 lowerLight = glm::vec3(posAvatarX + 10.0f, 4.0f, posAvatarZ + 15.0f);
					spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, -1.0f));
				}
			}
			else {
				glm::vec3 lowerLight = glm::vec3(posAvatarX - 5.0f, 4.0f, posAvatarZ);
				spotLights[0].SetFlash(lowerLight, glm::vec3(1.0, 0.0f, 0.0f));

			}

			//Luz Phieneas
			glm::vec3 phineasPosLuz = glm::vec3(movXPhineas, 4.0f, movZPhineas);
			spotLights[1].SetFlash(phineasPosLuz + glm::vec3(10.0, 0.0f, 0.2f), glm::vec3(0.0, -1.0f, 0.0f));//(posicion,dondeilumina)


			//Luz Ferb
			glm::vec3 ferbPosLuz = glm::vec3(movXFerb + 7.0f, 4.0f, movZFerb);
			spotLights[2].SetFlash(ferbPosLuz + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, -1.0f, 0.0f));//(posicion,dondeilumina)
		}


		if (encendidasApagadas == false) {
			//Luz Camara
			glm::vec3 lowerLight = glm::vec3(posAvatarX + 5.0f, -3.2f, posAvatarZ + 0.0f);
			spotLights[0].SetFlash(lowerLight, glm::vec3(0.0, 0.0f, 0.0f));

			//Luz phineas
			glm::vec3 phineasPosLuz = glm::vec3(movXPhineas, 4.0f, movZPhineas);
			spotLights[1].SetFlash(phineasPosLuz + glm::vec3(10.0, 0.0f, 0.2f), glm::vec3(0.0, 0.0f, 0.0f));//(posicion,dondeilumina)

			//Luz Ferb
			glm::vec3 ferbPosLuz = glm::vec3(movXFerb + 7.0f, 4.0f, movZFerb);
			spotLights[2].SetFlash(ferbPosLuz + glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0f, 0.0f));//(posicion,dondeilumina)
		}

		}
	
	}


	
	

	



void animacionCandase() {

	if (contadorVueltasCandase<3 && movZCandase<36){
		
		if (contadorVueltasCandase == 1.0f && movYCandase<2.0f) {
			if (movYCandase >= 2.7 && movYCandase < 3.0f) {
				movYCandase = 3.0f;
			}
			else {
				movYCandase += 0.5 * deltaTime;
			}
			
		}
		if (contadorVueltasCandase == 2.0f && movYCandase >0.0f) {
			if (movYCandase >= 0.3 && movYCandase < 0.0f) {
				movYCandase = 0.0f;
			}
			else {
				movYCandase -= 0.5 * deltaTime;
			}
			
		}
		
		if (anguloYCandase >= 357 && anguloYCandase < 360) {
			anguloYCandase = 0.0f;
			contadorVueltasCandase++;
			if (contadorVueltasCandase == 3) {
				movZCandase = 36;
			}
		}
		else {
			anguloYCandase += 30 * 0.5 * deltaTime;
			movZCandase += 0.5 * deltaTime;
		}

	}

	
	if (contadorVueltasCandase == 3) {
			if (anguloYCandase >= 178 && anguloYCandase<180) {
				anguloYCandase = 180;
				contadorVueltasCandase++;
			}
			else {
				anguloYCandase += 30 * 0.5 * deltaTime;

			}
		}
	if (contadorVueltasCandase == 4 && movZCandase >0.0f) {
		if (movZCandase <= 0.3 && movZCandase > 0.0f) {
			movZCandase = 0.0f;
			contadorVueltasCandase++;
		}
		else {
			movZCandase -= 0.5 * deltaTime;
		}
		
	}
	if (contadorVueltasCandase == 5 && movZCandase == 0.0f) {
		if (anguloYCandase <= 2 && anguloYCandase > 0.0f) {
			anguloYCandase = 0;
			contadorVueltasCandase=0.0f;
		}
		else {
			anguloYCandase -= 30 * 0.5 * deltaTime;

		}
	}
	
	
	}

void GiroArbol(bool* keys) {
	
	if (keys[GLFW_KEY_E] && activaArbol==false) {
		activaArbol = true;
	}
		

	if (activaArbol) {
		if (alturaArbol <= 10.0f && subidaArbolNavidad) {

			if (alturaArbol >= 9.0f && alturaArbol < 10.0f) {
				subidaArbolNavidad = false;
			}
			else
			{
				rotacionArbol+= deltaTime*15;
				alturaArbol += deltaTime;
			}
		}
		else if (alturaArbol >= 0.0f && subidaArbolNavidad == false) {

			if (alturaArbol <= 1.0f && alturaArbol > 0.0f) {
				subidaArbolNavidad = true;
			}
			else
			{
				rotacionArbol-= deltaTime*15;
				alturaArbol -= deltaTime;
			}
		}
	}

}

void animaiconSanta(bool* keys){
	if (keys[GLFW_KEY_Q] && activaSanta == false) {
		activaSanta = true;
		
	}
	if (activaSanta) {
		if (santaY < 10 && subidaSanta) {
			if (santaY >= 9.7 && santaY < 10) {
				santaY = 10;
				SoundEngine->play2D("audio/jojoSanta.mp3", false);
			}
			else {
				santaY += deltaTime;
			}
			
		}
		if (santaY == 10 && anguloXSanta< 360 && subidaSanta){
			if (anguloXSanta>=359.7 && anguloXSanta<360) {
				anguloXSanta = 360;
				subidaSanta = false;
				SoundEngine->play2D("audio/jojoSanta.mp3", false);
			}
			else {
				anguloXSanta += deltaTime * 15;
				santaZ += deltaTime;
			}
			
		}


		if (anguloXSanta == 360 && santaY > 0 && subidaSanta == false) {
			if (santaY <=0.2  && santaY >0) {
				santaY = 0;
				anguloYSanta = 180;
				SoundEngine->play2D("audio/jojoSanta.mp3", false);
			}
			else {
				santaY -= deltaTime;
				anguloYSanta += deltaTime * 18;
			}
		}


		if (anguloYSanta <= 180 && santaY == 0 && subidaSanta == false && santaZ>0) {
			if (santaZ <= 0.2 && santaZ > 0) {
				santaZ = 0;
				anguloYSanta = 0;
				anguloXSanta = 0;
				subidaSanta = true;
				SoundEngine->play2D("audio/jojoSanta.mp3", false);
			}
			else {
				santaZ -= deltaTime;
				anguloYSanta -= deltaTime * 8;
			}
		}
	


		
	

	}
}
//funcion de suntrack
void soundTrack() {
	SoundEngine->play2D("audio/soundTrack.mp3", false);

}


//iluminacion para show
void iluminacionShow(bool* keys){
	if (keys[GLFW_KEY_Y] && enciendeLucesShow == false) {
		enciendeLucesShow = true;
	}
	else if (keys[GLFW_KEY_Y] && enciendeLucesShow) {
		enciendeLucesShow = false;
	}

	if ( enciendeLucesShow) {
		spotLights[3].SetFlash(glm::vec3(-5.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		spotLights[4].SetFlash(glm::vec3(-5.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
	}
	else if (enciendeLucesShow == false) {
		spotLights[3].SetFlash(glm::vec3(0.0f, 10.0f, 15.0f), glm::vec3(0.0, 0.0f, 0.0f));
		spotLights[4].SetFlash(glm::vec3(0.0f, 10.0f, 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		spotLights[5].SetFlash(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
		spotLights[6].SetFlash(glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
	}
	
}

//show de luces 
void showIluminacion(bool* keys){
	if (keys[GLFW_KEY_U] && showActivacion == false) {
		showActivacion = true;
		
		posLuz3X = 0.0f;
		posLuz3Y = 0.0f;
		posLuz3Z = 0.0f;
		posLuz4X = 0.0f;
		posLuz4Y = 0.0f;
		posLuz4Z = 0.0f;
		posLuz5X = 0.0f;
		posLuz5Y = 0.0f;
		posLuz5Z = 0.0f;
		posLuz6X = 0.0f;
		posLuz6Y = 0.0f;
		posLuz6Z = 0.0f;
		posLuz6X = -20.0f;
		timerShow = 0.0f;
	}
	if (showActivacion) {
		timerShow += deltaTime * 5;
		if (timerShow < 35.0f) {
			posLuz6X+= deltaTime * 5;
			spotLights[6].SetFlash(glm::vec3(posLuz6X, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 70.0f && timerShow>35.0f) {
			posLuz6X -= deltaTime * 5;
			spotLights[6].SetFlash(glm::vec3(posLuz6X, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 73.0f && timerShow>70.0f) {
			posLuz6X = -20.0f;
			spotLights[6].SetFlash(glm::vec3(posLuz6X, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 76.0f && timerShow>73.0f) {
			spotLights[3].SetFlash(glm::vec3(-3.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(-3.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 79.0f && timerShow>76.0f) {
			spotLights[3].SetFlash(glm::vec3(-3.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(-3.0f, 10.0f, 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 81.0f && timerShow>79.0f) {
			spotLights[3].SetFlash(glm::vec3(-3.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(-3.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 84.0f && timerShow>81.0f) {
			spotLights[3].SetFlash(glm::vec3(-3.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(-3.0f, 10.0f, 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 87.0f && timerShow>84.0f) {
			spotLights[3].SetFlash(glm::vec3(-3.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(-3.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(5.0f, 10.0f, 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(5.0f, 10.0f, 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			posLuz6X = 0.0f;
		}
		if (timerShow < 100.0f && timerShow>87.0f) {
			 posLuz3X -= deltaTime * 5;
			 posLuz3Z -= deltaTime * 5;
			 posLuz4X -= deltaTime * 5;
			 posLuz4Z += deltaTime * 5;
			 posLuz5X += deltaTime * 5;
			 posLuz5Z -= deltaTime * 5;
			 posLuz6X += deltaTime * 5;
			 posLuz6Z += deltaTime * 5;
			spotLights[3].SetFlash(glm::vec3(posLuz3X -3.0f, 10.0f, posLuz3Z +5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X -3.0f, 10.0f, posLuz4Z +10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X +5.0f, 10.0f, posLuz5Z +5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X +5.0f, 10.0f, posLuz6Z +10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 108.0f && timerShow>100.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 105.0f && timerShow>100.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 110.0f && timerShow>105.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 115.0f && timerShow>110.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 120.0f && timerShow>115.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}


		if (timerShow < 133.0f && timerShow>120.0f) {
			posLuz3X += deltaTime * 5;
			posLuz3Z += deltaTime * 5;
			posLuz4X += deltaTime * 5;
			posLuz4Z -= deltaTime * 5;
			posLuz5X -= deltaTime * 5;
			posLuz5Z += deltaTime * 5;
			posLuz6X -= deltaTime * 5;
			posLuz6Z -= deltaTime * 5;
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));

		}
		if (timerShow < 138.0f && timerShow>133.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 143.0f && timerShow>138.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 148.0f && timerShow>143.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 148.0f && timerShow>143.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 150.0f && timerShow>148.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}

		if (timerShow < 157.0f && timerShow>150.0f) {
			posLuz4X -= deltaTime * 5;
			posLuz6X += deltaTime * 5;
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 164.0f && timerShow>157.0f) {
			posLuz4Z -= deltaTime * 5;
			posLuz6Z -= deltaTime * 5;
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 168.0f && timerShow>164.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 170.0f && timerShow>168.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 177.0f && timerShow>170.0f) {
			posLuz3Z += deltaTime * 5;
			posLuz5Z += deltaTime * 5;
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 184.0f && timerShow>177.0f) {
			posLuz3X += deltaTime * 5;
			posLuz5X -= deltaTime * 5;
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 192.0f && timerShow>184.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 202.0f && timerShow>192.0f) {
			posLuz3Z -= deltaTime * 5;
			posLuz5Z -= deltaTime * 5;
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 208.0f && timerShow>202.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow < 215.0f && timerShow>208.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, 0.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, 0.0f, 0.0f));
		}
		if (timerShow < 225.0f && timerShow>215.0f) {
			spotLights[3].SetFlash(glm::vec3(posLuz3X - 3.0f, 10.0f, posLuz3Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[4].SetFlash(glm::vec3(posLuz4X - 3.0f, 10.0f, posLuz4Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[5].SetFlash(glm::vec3(posLuz5X + 5.0f, 10.0f, posLuz5Z + 5.0f), glm::vec3(0.0, -1.0f, 0.0f));
			spotLights[6].SetFlash(glm::vec3(posLuz6X + 5.0f, 10.0f, posLuz6Z + 10.0f), glm::vec3(0.0, -1.0f, 0.0f));
		}
		if (timerShow>225.0f) {
			showActivacion = false;
			timerShow = 0.0f;
		}
	}

	
}

//Animacin de las extremidades avatar
void avatarPasos() {

	timerPasosAvaatr += deltaTime * 6;

	if (timerPasosAvaatr < 1) {
		//Pie izquierdo
		posicionPieIzqY = 0.25f;
		posicionPieIzqX = 0.0f;
		rotacionPieIzq = 0;
		
		//Pie derecho
		 rotacionPieDer = 0.0f;
		 posicionPieDerX = 0.0f;
		 posicionPieDerY = 0.25f;


		 //Brazo Izquierdo
		 rotacionBrazoIz = 0.0f;
		 posicionBrazoIzX = 0.0f;
		 posicionBrazoIzY = 0.25f;


		 //Brazo Derecho
		 rotacionBrazoDer = 0.0f;
		 posicionBrazoDerX = 0.0f;
		 posicionBrazoDerY = 0.25f;
 


		
	}
	if (timerPasosAvaatr < 2 && timerPasosAvaatr>1) {
		posicionPieIzqY = 0.41f;
		posicionPieIzqX = 0.43f;
		rotacionPieIzq = 45;

		posicionPieDerX = -0.415f;
		posicionPieDerY = 0.44f;
		rotacionPieDer = -45;

		//Brazo Izquierdo
		rotacionBrazoIz = -45;
		posicionBrazoIzX = -0.662f;
		posicionBrazoIzY = 0.52f;

		//Brazo Derecho
		rotacionBrazoDer = 45;
		posicionBrazoDerX = 0.662f;
		posicionBrazoDerY = 0.51f;

		

		
	}
	if (timerPasosAvaatr < 3 && timerPasosAvaatr>2) {
		posicionPieIzqY = 0.25f;
		posicionPieIzqX = 0.0f;
		rotacionPieIzq = 0;

		posicionPieDerY = 0.25f;
		posicionPieDerX = 0.0f;
		rotacionPieDer = 0;

		//Brazo Izquierdo
		rotacionBrazoIz = 0.0f;
		posicionBrazoIzX = 0.0f;
		posicionBrazoIzY = 0.25f;

		//Brazo Derecho
		rotacionBrazoDer = 0.0f;
		posicionBrazoDerX = 0.0f;
		posicionBrazoDerY = 0.25f;
	}
	if (timerPasosAvaatr < 4 && timerPasosAvaatr>3) {

		posicionPieIzqX = -0.415f;
		posicionPieIzqY = 0.44f;
		rotacionPieIzq = -45;


		posicionPieDerY = 0.41f;
		posicionPieDerX = 0.43f;
		rotacionPieDer = 45;


		//Brazo Izquierdo
		rotacionBrazoIz = 45;
		posicionBrazoIzX = 0.662f;
		posicionBrazoIzY = 0.51f;

		//Brazo Derecho
		rotacionBrazoDer = -45;
		posicionBrazoDerX = -0.662f;
		posicionBrazoDerY = 0.52f;
	}


	if (timerPasosAvaatr > 4) {
		timerPasosAvaatr = 0.0f;
	}

}

//Avatar cuando no camina
void avaatarParado()
{
	timerPasosAvaatr = 0.0f;
	

	//Pie izquierdo
	posicionPieIzqY = 0.25f;
	posicionPieIzqX = 0.0f;
	rotacionPieIzq = 0;

	//Pie derecho
	rotacionPieDer = 0.0f;
	posicionPieDerX = 0.0f;
	posicionPieDerY = 0.25f;


	//Brazo Izquierdo
	rotacionBrazoIz = 0.0f;
	posicionBrazoIzX = 0.0f;
	posicionBrazoIzY = 0.25f;


	//Brazo Derecho
	rotacionBrazoDer = 0.0f;
	posicionBrazoDerX = 0.0f;
	posicionBrazoDerY = 0.25f;
}

	
