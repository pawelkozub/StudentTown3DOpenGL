#include "stdafx.h"

using namespace std;

// stała do obsługi menu podręcznego

enum
{
	EXIT // wyjście
};

struct Point3Ds {
	double x;
	double y;
	double z;
};

GLuint texture[15];

double PredkoscKamery = 0;
double scale = 3;
GLfloat denisty_fog = 0.04;
GLfloat velocity_lighting = 4;
bool stereoscopic = false;
float zeroo = 0.1;
float k_eye = 0.2;

// pionowy kąt pola widzenia

GLdouble fovy = 60;
float czajnikrot = 0;
// wpółrzędne położenia obserwatora

GLdouble eyex = 0;
GLdouble eyey = 0;
GLdouble eyez = 3;
GLdouble ObrotX = 3.14 / 2.0;
GLdouble ObrotY = 0;

bool KeyUP = false;
bool KeyDOWN = false;
bool KeyRIGHT = false;
bool KeyLEFT = false;

// współrzędne punktu w którego kierunku jest zwrócony obserwator,

GLdouble centerx = 0;
GLdouble centery = 0;
GLdouble centerz = -100;

int figure = 0;
float kat = 0;
GLuint idlisty;

void twins(int m, int n, int door, int peak, int club);
void twins_door(int ix, int iy, int iz);
void UstawKamere();
// funkcja generująca scenę 3D

#pragma region views

void StereoProjection(float _left, float _right, float _bottom, float _top, float _near, float _far, float _zero_plane, float _dist, float _eye)
{
	//    Perform the perspective projection for one eye's subfield.
	//    The projection is in the direction of the negative z-axis.
	//            _left=-6.0;
	//            _right=6.0;
	//            _bottom=-4.8;
	//             _top=4.8;
	//    [default: -6.0, 6.0, -4.8, 4.8]
	//    left, right, bottom, top = the coordinate range, in the plane of zero parallax setting,
	//         which will be displayed on the screen.
	//         The ratio between (right-left) and (top-bottom) should equal the aspect
	//    ratio of the display.


	//                  _near=6.0;
	//                  _far=-20.0;
	//    [default: 6.0, -6.0]
	//    near, far = the z-coordinate values of the clipping planes.

	//                  _zero_plane=0.0;
	//    [default: 0.0]
	//    zero_plane = the z-coordinate of the plane of zero parallax setting.

	//    [default: 14.5]
	//                     _dist=10.5;
	//   dist = the distance from the center of projection to the plane of zero parallax.

	//    [default: -0.3]
	//                 _eye=-0.3;
	//    eye = half the eye separation; positive for the right eye subfield,
	//    negative for the left eye subfield.

	float   _dx = _right - _left;
	float   _dy = _top - _bottom;

	float   _xmid = (_right + _left) / 2.0;
	float   _ymid = (_top + _bottom) / 2.0;

	float   _clip_near = _dist + _zero_plane - _near;
	float   _clip_far = _dist + _zero_plane - _far;

	float  _n_over_d = _clip_near / _dist;

	float   _topw = _n_over_d * _dy / 2.0;
	float   _bottomw = -_topw;
	float   _rightw = _n_over_d * (_dx / 2.0 - _eye);
	float   _leftw = _n_over_d *(-_dx / 2.0 - _eye);

	// Create a fustrum, and shift it off axis
	// glTranslate() applies to PROJECTION matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(_leftw, _rightw, _bottomw, _topw, _clip_near, _clip_far);
	glTranslatef(-_xmid - _eye, -_ymid, -_zero_plane - _dist);
}

void stereoscopic_left_eye()
{
	glViewport(0, 0, GetSystemMetrics(SM_CXSCREEN) - 5, GetSystemMetrics(SM_CYSCREEN) - 5);

	//Dla lewego oka :
	glDrawBuffer(GL_BACK_LEFT);
	StereoProjection(-6, 6, -4.8, 4.8, 12, -100, zeroo, 13, k_eye*-1);
	glColorMask(true, false, false, false);
}

void stereoscopic_right_eye()
{
	glDrawBuffer(GL_BACK_RIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);
	StereoProjection(-6, 6, -4.8, 4.8, 12, -100, zeroo, 13, k_eye);
	glColorMask(false, true, true, false);
	UstawKamere();
	//Rysowanie sceny
	if (figure == 0) glCallList(idlisty + 0);
	switch (figure)
	{
	case 1:
		glCallList(idlisty + 0);
		break;

	case 2:
		glCallList(idlisty + 1);
		break;

	case 3:
		glCallList(idlisty + 2);
		break;
	}
	glColorMask(true, true, true, true);
}

void lighting() {
	glEnable(GL_LIGHTING); //w??czenia ?wiat?a
	GLfloat ambientlight[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientlight);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);

	GLfloat DiffuseLight1[] = { 1.0,1.0,1.0,1.0 };
	GLfloat LightPosition1[] = { 0.0, 0.0, 200.0, 1.0 };
	GLfloat AmbientLight[] = { 0.1, 0.1, 0.1, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight1);

	glPushMatrix();
	glRotatef(kat, 1, 0, 0);
	kat += velocity_lighting;
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition1);
	glPopMatrix();

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);

	GLfloat specular[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);


	GLfloat specref[] = { 1.0,1.0,1.0,1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glEnable(GL_LIGHT0);
}

#pragma endregion views


#pragma region texture

AUX_RGBImageRec *LoadBMP(char* filename)
{
	FILE* file = NULL;

	if (!filename)
	{
		return NULL;
	}

	file = fopen(filename, "r");

	if (file)
	{
		fclose(file);
		return auxDIBImageLoad(filename);
	}

	return NULL;
}

int LoadGLTextures()
{
	int status = false;

	AUX_RGBImageRec *TextureImage[15];
	memset(TextureImage, 0, sizeof(void *) * 1);

	if (TextureImage[0] = LoadBMP("remont.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[0]);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[1] = LoadBMP("studio.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[1]);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[2] = LoadBMP("roof.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[2]);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[3] = LoadBMP("fence.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[3]);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[4] = LoadBMP("road.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[4]);
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[5] = LoadBMP("bruk.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[5]);
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[5]->sizeX, TextureImage[5]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[5]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[6] = LoadBMP("basketballcourt.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[6]);
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[6]->sizeX, TextureImage[6]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[6]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[7] = LoadBMP("asfalt.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[7]);
		glBindTexture(GL_TEXTURE_2D, texture[7]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[7]->sizeX, TextureImage[7]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[7]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[8] = LoadBMP("parking.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[8]);
		glBindTexture(GL_TEXTURE_2D, texture[8]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[8]->sizeX, TextureImage[8]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[8]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[9] = LoadBMP("boisko_nozna2.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[9]);
		glBindTexture(GL_TEXTURE_2D, texture[9]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[9]->sizeX, TextureImage[9]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[9]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[10] = LoadBMP("tenis.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[10]);
		glBindTexture(GL_TEXTURE_2D, texture[10]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[10]->sizeX, TextureImage[10]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[10]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[11] = LoadBMP("boisko_kosz.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[11]);
		glBindTexture(GL_TEXTURE_2D, texture[11]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[11]->sizeX, TextureImage[11]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[11]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	if (TextureImage[12] = LoadBMP("tennisfeld.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[12]);
		glBindTexture(GL_TEXTURE_2D, texture[12]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[12]->sizeX, TextureImage[12]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[12]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	if (TextureImage[13] = LoadBMP("sand.bmp"))
	{
		status = true;

		glGenTextures(1, &texture[13]);
		glBindTexture(GL_TEXTURE_2D, texture[13]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[13]->sizeX, TextureImage[13]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[13]->data);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	if (TextureImage[0])
	{
		if (TextureImage[0]->data)
		{
			free(TextureImage[0]->data);
		}
		free(TextureImage[0]);
	}
	return status;
}

void texture_front(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(m, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(m, n); glVertex3f(1.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(-1.0f, 2.0f, 1.0f);
	glEnd();
}

void texture_back(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(m, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2f(m, n); glVertex3f(-1.0f, 2.0f, -1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(1.0f, 2.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glEnd();
}

void texture_left(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, -1.0f);
	glTexCoord2f(m, 0.0f); glVertex3f(-1.0f, 0.0f, 1.0f);
	glTexCoord2f(m, n); glVertex3f(-1.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(-1.0f, 2.0f, -1.0f);
	glEnd();
}

void texture_right(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(m, 0.0f); glVertex3f(1.0f, 0.0f, -1.0f);
	glTexCoord2f(m, n); glVertex3f(1.0f, 2.0f, -1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(1.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();
}

void texture_top(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 2.0f, -1.0f);
	glTexCoord2f(m, 0.0f); glVertex3f(-1.0f, 2.0f, 1.0f);
	glTexCoord2f(m, n); glVertex3f(1.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(1.0f, 2.0f, -1.0f);
	glEnd();
}

void texture_bottom(int nr_texture, int m = 1, int n = 1)
{
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(m, n); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, n); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(m, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
}

void texture_free(int nr_texture, Point3Ds p, int m = 1, int n = 1)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0,0,0);
	glTexCoord2f(m, 0.0f); glVertex3f(p.x, p.y,0);
	glTexCoord2f(m, n); glVertex3f(p.x, p.y, p.z);
	glTexCoord2f(0.0f, n); glVertex3f(0, 0, p.z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void texture_surface(GLfloat points2[], GLuint nr_texture, GLfloat m = 1, GLfloat n = 1)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[nr_texture]);

	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 4, 0, 1, 12, 3, points2);
	glMap2f(GL_MAP2_COLOR_4, 0.0, 1.0, 3, 4, 0, 1, 12, 3, points2);

	//teksturowanie

	glEnable(GL_MAP2_TEXTURE_COORD_2);

	// ewaluator dwuwymiarowy dla czterech punktów kontronych powierzchni
	GLfloat texture_coord[4 * 2] = { 0.0, 0.0, m, 0.0, 0.0, n, m, n };
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0.0, 1.0, 2, 2, 0.0, 1.0, 4, 2, texture_coord);

	glMapGrid2f(25, 0, 1, 25, 0, 1);
	glPointSize(2);
	glEvalMesh2(GL_FILL, 0, 25, 0, 25);

	glDisable(GL_MAP2_TEXTURE_COORD_2);
	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_TEXTURE_2D);

}

#pragma endregion texture

#pragma region OBJ

void loadObjFile(const char* filename)
{
	FILE* file = NULL;

	if (!filename)
	{
		return;
	}

	file = fopen(filename, "r");
	char type = 0;


	int vertexCount = 0;
	int facesCount = 0;

	char buffer[1000] = {};

	while (fgets(buffer, 1000, file))
	{
		if (buffer[0] == 'v')
		{
			vertexCount++;
		}
		if (buffer[0] == 'f')
		{
			facesCount++;
		}
	}

	double **vertex = new double*[vertexCount];
	for (int i = 0; i < vertexCount; ++i)
		vertex[i] = new double[3];

	int **faces = new int*[facesCount];
	for (int i = 0; i < facesCount; ++i)
		faces[i] = new int[3];


	if (file)
	{
		fclose(file);
	}

	ifstream ifs;
	ifs.open(filename, ifstream::in);

	string out;
	int v = 0;
	int f = 0;
	while (!ifs.eof())
	{
		ifs >> out;
		if (out == "v")
		{
			ifs >> vertex[v][0] >> vertex[v][1] >> vertex[v][2];
			v++;
		}

		if (out == "f")
		{
			ifs >> faces[f][0] >> faces[f][1] >> faces[f][2];
			f++;
		}
	}
	ifs.close();


	//glColor3f(0, 0.6, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (int i = 0; i < facesCount; i++)
	{
		glBegin(GL_TRIANGLES);

		int a = faces[i][0] - 1;
		int b = faces[i][1] - 1;
		int c = faces[i][2] - 1;

		double a1 = vertex[a][0];
		double a2 = vertex[a][1];
		double a3 = vertex[a][2];

		double b1 = vertex[b][0];
		double b2 = vertex[b][1];
		double b3 = vertex[b][2];

		double c1 = vertex[c][0];
		double c2 = vertex[c][1];
		double c3 = vertex[c][2];


		glVertex3f(a1, a2, a3);
		glVertex3f(b1, b2, b3);
		glVertex3f(c1, c2, c3);

		glEnd();
	}
}

void Object_OBJ()
{
	glPushMatrix();
	glColor3f(0.54, 0.27, 0.07);
	loadObjFile("stem.obj");
	glColor3f(0, 0.39, 0);
	loadObjFile("leaves.obj");
	glPopMatrix();
}

#pragma endregion OBJ

#pragma region other_things

void row_windows(int ix, int iy, int iz)
{
	glColor3f(0.78, 0.78, 0.78);//poziom
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 0.0 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.0 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.27, 0.27, 0.27);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0, 0, 1);//okna
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(0.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(1.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(1.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.27, 0.27, 0.27);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(1.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(2.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(2.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(1.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0, 0, 1);//okna
	glBegin(GL_QUADS);
	glVertex3f(2.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(2.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glNormal3f(0, 0, 1);
	glEnd();
}

void last_row_windows(int n, int iy, int iz)
{
	glColor3f(0.27, 0.27, 0.27);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (n * 3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (n * 3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.78, 0.78, 0.78);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (n * 3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (n * 3.25), 0.5 + (iy*1.5), iz * 10);
	glEnd();
}

void row_windows_club(int ix, int iy, int iz)
{
	glColor3f(0.78, 0.78, 0.78);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 0 + (iy*1.5), iz * 5);
	glVertex3f(3.25 + (ix*3.25), 0 + (iy*1.5), iz * 5);
	glVertex3f(3.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 5);
	glVertex3f(0 + (ix*3.25), 1.5 + (iy*1.5), iz * 5);
	glEnd();
}

void last_row_windows_club(int n, int iy, int iz)
{

	glColor3f(0.78, 0.78, 0.78);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (n * 3.25), 0 + (iy*1.5), iz * 5);
	glVertex3f(0.25 + (n * 3.25), 0 + (iy*1.5), iz * 5);
	glVertex3f(0.25 + (n * 3.25), 1.5 + (iy*1.5), iz * 5);
	glVertex3f(0 + (n * 3.25), 1.5 + (iy*1.5), iz * 5);
	glEnd();
}

void fence(int n, int x = 1, int y = 1)
{
	/*glColor3f(0,0,0);
	glBegin(GL_QUADS);	

	for (int i = 0; i < 5; i++)
	{
		glVertex3f(0 + (i*0.2) + n, 0, 0);
		glVertex3f(0.1 + (i*0.2) + n, 0, 0);
		glVertex3f(0.1 + (i*0.2) + n, 1, 0);
		glVertex3f(0 + (i*0.2) + n, 1, 0);
	}
	glEnd();*/

		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0 + n, 0, 0);
	glTexCoord2f(x, 0.0f); glVertex3f(1 + n, 0, 0);
	glTexCoord2f(x, y); glVertex3f(1 + n, 1, 0);
	glTexCoord2f(0.0f, y); glVertex3f(0 + n, 1, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void high_fence(int m, int n, int x = 1, int y = 1)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, 0);
	glTexCoord2f(x, 0.0f); glVertex3f(m, 0, 0);
	glTexCoord2f(x, y); glVertex3f(m, n, 0);
	glTexCoord2f(0.0f, y); glVertex3f(0, n, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void road(int n, int scale)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0 + n, 0.5, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(scale + (n*scale), 0.5, 0);
	glTexCoord2f(1, 1); glVertex3f(scale + (n*scale), 0.5, scale);
	glTexCoord2f(0.0f, 1); glVertex3f(0 + n, 0.5, scale);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void sidewalk(double n, double scale)
{
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0 + n*scale, 0.5, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(scale + (n*scale), 0.5, 0);
	glTexCoord2f(1, 1); glVertex3f(scale + (n*scale), 0.5, scale);
	glTexCoord2f(0.0f, 1); glVertex3f(0 + n*scale, 0.5, scale);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

#pragma endregion other_things

#pragma region twins

void club(int m = 2, int n = 5)
{
	for (int iz = 0; iz < 2; iz++)
	{
		for (int iy = 0; iy < m; iy++)
		{
			for (int ix = 0; ix < n; ix++)
			{
				row_windows_club(ix, iy, iz);
			}
			last_row_windows_club(n, iy, iz);
		}

		glColor3f(0.78, 0.78, 0.78); //poddach
		glBegin(GL_QUADS);
		glVertex3f(0, 0.0 + (m * 1.5), iz * 5);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.0 + (m * 1.5), iz * 5);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.5 + (m * 1.5), iz * 5);
		glVertex3f(0, 0.5 + (m * 1.5), iz * 5);

		glVertex3f(0 + (n * 3.25), 0.0 + (m * 1.5), iz * 5);
		glVertex3f(0.25 + (n * 3.25), 0.0 + (m * 1.5), iz * 5);
		glVertex3f(0.25 + (n * 3.25), 0.5 + (m * 1.5), iz * 5);
		glVertex3f(0 + (n * 3.25), 0.5 + (m * 1.5), iz * 5);
		glEnd();
	}

	//poprzeczny
	for (int i = 0; i < 2; i++) {
		glColor3f(0.78, 0.78, 0.78);
		glBegin(GL_QUADS);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 0);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 5);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 5);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 0);
		glEnd();
	}

	glColor3f(0, 0, 0);//dach
	glBegin(GL_QUADS);
	glVertex3f((3.25*n) + 0.25, (m*1.5 + 0.5), 0);
	glVertex3f((3.25*n) + 0.25, (m*1.5 + 0.5), 5);
	glVertex3f(0, (m*1.5 + 0.5), 5);
	glVertex3f(0, (m*1.5 + 0.5), 0);

	glEnd();
}

void build_club(int ix, int iy, int iz)
{
	for (int i = 0; i < 2; i++)
	{
		//korytarz
		glColor3f(0.27, 0.27, 0.27);
		glBegin(GL_QUADS);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 0, -5);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 0, 0);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 1.5, 0);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 1.5, -5);
		glEnd();
	}

	//dach
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(1 + (ix*3.25), 1.5, 0);
	glVertex3f(2.5 + (ix*3.25), 1.5, 0);
	glVertex3f(2.5 + (ix*3.25), 1.5, -5);
	glVertex3f(1 + (ix*3.25), 1.5, -5);
	glEnd();

	glTranslated(((ix - 3)*3.25), 0, -10);
	club();
	glTranslated(-((ix - 3)*3.25), 0, 10);



}

void twins_door(int ix, int iy, int iz) 
{
	glColor3f(0.27, 0.27, 0.27);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(1 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(1 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.78, 0.78, 0.78);//poziom
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(1 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(1 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0, 0, 1);//okna
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(1 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(2.5 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(2.5 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(1 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.27, 0.27, 0.27);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(2.5 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(2.5 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();

	glColor3f(0.78, 0.78, 0.78);//poziom
	glBegin(GL_QUADS);
	glVertex3f(2.5 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(2.5 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glEnd();
}

void twins_peak(int ix, int iy, int iz)
{
	for (int i = 0; i < 2; i++)
	{
		//daszek
		glColor3f(0.78, 0.78, 0.78);
		glBegin(GL_QUADS);
		glVertex3f((ix*3.25), 1.5 + (i*0.25), 10);
		glVertex3f(((ix + 1)*3.25), 1.5 + (i*0.25), 10);
		glVertex3f(((ix + 1)*3.25), 1.5 + (i*0.25), 13);
		glVertex3f((ix*3.25), 1.5 + (i*0.25), 13);

		glVertex3f(((ix + 1)*3.25), 1.5, 13);
		glVertex3f(((ix + 1)*3.25), 1.75, 13);
		glVertex3f((ix*3.25), 1.75, 13);
		glVertex3f((ix*3.25), 1.5, 13);

		glVertex3f((ix*3.25) + (i * 3.25), 1.5, 13);
		glVertex3f((ix*3.25) + (i * 3.25), 1.75, 13);
		glVertex3f((ix*3.25) + (i * 3.25), 1.75, 10);
		glVertex3f((ix*3.25) + (i * 3.25), 1.5, 10);
		glEnd();

		//slup
		for (int j = 0; j < 2; j++)
		{
			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f((ix*3.25) + 0.1 - (i*0.25) + (j * 3.25), 0, 12);
			glVertex3f((ix*3.25) + 0.1 - (i*0.25) + (j * 3.25), 0, 11.75);
			glVertex3f((ix*3.25) + 0.1 - (i*0.25) + (j * 3.25), 2, 11.75);
			glVertex3f((ix*3.25) + 0.1 - (i*0.25) + (j * 3.25), 2, 12);

			glVertex3f((ix*3.25) + 0.1 + (j * 3.25), 0, 12 + (i*-0.25));
			glVertex3f((ix*3.25) + 0.1 + (j * 3.25), 2, 12 + (i*-0.25));
			glVertex3f((ix*3.25) + 0.1 - 0.25 + (j * 3.25), 2, 12 + (i*-0.25));
			glVertex3f((ix*3.25) + 0.1 - 0.25 + (j * 3.25), 0, 12 + (i*-0.25));

			glVertex3f((ix*3.25) + 0.1 + (j * 3.25), 2, 12);
			glVertex3f((ix*3.25) + 0.1 + (j * 3.25), 2, 11.75);
			glVertex3f((ix*3.25) + 0.1 - 0.25 + (j * 3.25), 2, 11.75);
			glVertex3f((ix*3.25) + 0.1 - 0.25 + (j * 3.25), 2, 12);
			glEnd();
		}

	}

}

void twins_build_tunnel(int ix)
{
	for (int i = 0; i < 2; i++)
	{
		glColor3f(0.78, 0.78, 0.78); //sciana
		glBegin(GL_QUADS);
		glVertex3f(0.25 + (ix*3.25) + (i * 3), 0.0, 0);
		glVertex3f(0.25 + (ix*3.25) + (i * 3), 0.0, 10);
		glVertex3f(0.25 + (ix*3.25) + (i * 3), 1.5, 10);
		glVertex3f(0.25 + (ix*3.25) + (i * 3), 1.5, 0);
		glEnd();

		last_row_windows(ix, 0, i);
	}


	glColor3f(0.78, 0.78, 0.78); //sufit
	glBegin(GL_QUADS);
	glVertex3f(0 + (ix*3.25), 1.5, 0);
	glVertex3f(0 + (ix*3.25), 1.5, 10);
	glVertex3f(0 + (ix*3.25) + (3.25), 1.5, 10);
	glVertex3f(0 + (ix*3.25) + (3.25), 1.5, 0);
	glEnd();
}

void twins_tunnel(int tunnel, int n) 
{
	for (int iz = 0; iz < 2; iz++)
	{
		for (int ix = 0; ix < n; ix++)
		{
			if (tunnel == 2) {
				if (ix == 2 || ix == n - 3)
				{
					twins_build_tunnel(ix);
					continue;
				}
			}
			if (tunnel == 1)
				if (ix == n - 1)
				{
					twins_build_tunnel(ix);
					continue;
				}
			if (tunnel == 3)
				if (ix == 0 || ix == 2 || ix == n - 3)
				{
					twins_build_tunnel(ix);
					continue;
				}

			if (ix == 5 && iz == 1) {
				twins_door(ix, 0, iz);
				continue;
			}

			row_windows(ix, 0, iz);
		}
		last_row_windows(n, 0, iz);
	}

	for (int i = 0; i < 2; i++) {
		glColor3f(0.78, 0.78, 0.78);//poprzeczny
		glBegin(GL_QUADS);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 0);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 10);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 1.5, 10);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 1.5, 0);
		glEnd();
	}
}

void twins(int m, int n, int door, int peak, int club)
{
	for (int iz = 0; iz < 2; iz++)
	{
		for (int iy = 0; iy < m; iy++)
		{
			for (int ix = 0; ix < n; ix++)
			{
				if (iz == 1 && ix == 5 && iy == 0 && door) {
					twins_door(ix, iy, iz);
					if (peak) twins_peak(ix, iy, iz);
					continue;
				}

				if (iz == 0 && ix == 5 && iy == 0 && club) {
					twins_door(ix, iy, iz);
					build_club(ix, iy, iz);
					continue;
				}
				row_windows(ix, iy, iz);
			}
			last_row_windows(n, iy, iz);
		}

		glColor3f(0.78, 0.78, 0.78); //poddach
		glBegin(GL_QUADS);
		glVertex3f(0, 0.0 + (m * 1.5), iz * 10);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.5 + (m * 1.5), iz * 10);
		glVertex3f(0, 0.5 + (m * 1.5), iz * 10);
		glEnd();

		glColor3f(0.78, 0.78, 0.78);//poddach
		glBegin(GL_QUADS);
		glVertex3f(0 + (n * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(0.25 + (n * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(0.25 + (n * 3.25), 0.5 + (m * 1.5), iz * 10);
		glVertex3f(0 + (n * 3.25), 0.5 + (m * 1.5), iz * 10);
		glEnd();
	}

	//poprzeczny
	for (int i = 0; i < 2; i++) {
		glColor3f(0.78, 0.78, 0.78);
		glBegin(GL_QUADS);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 0);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, 0, 10);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 10);
		glVertex3f((i) ? (i*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 0);
		glEnd();
	}


	glColor3f(0, 0, 0);//dach
	glBegin(GL_QUADS);
	glVertex3f((3.25*n) + 0.25, (m*1.5 + 0.5), 0);
	glVertex3f((3.25*n) + 0.25, (m*1.5 + 0.5), 10);
	glVertex3f(0, (m*1.5 + 0.5), 10);
	glVertex3f(0, (m*1.5 + 0.5), 0);

	glEnd();
}

void Builds_twins(int tunnel = 0, int peak = 0, int club = 0, int m = 5, int n = 11)
{
	glPushMatrix();
	if (tunnel) {
		twins_tunnel(tunnel, n);
		glTranslatef(0, 1.5, 0);
		twins(m - 1, n, 0, peak, club);
	}
	else
	{
		twins(m, n, 1, peak, club);
	}
	glPopMatrix();
}

#pragma endregion twins

#pragma region tower

void tower(int door, int m, int n)
{
	for (int iz = 0; iz < 2; iz++)
	{
		for (int iy = 0; iy < m; iy++)
		{
			for (int ix = 0; ix < n; ix++)
			{
				row_windows(ix, iy, iz);
			}

			last_row_windows(n, iy, iz);
		}

		glColor3f(0.78, 0.78, 0.78);//poddach
		glBegin(GL_QUADS);
		glVertex3f(0, 0.0 + (m * 1.5), iz * 10);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(3.25 + ((n - 1) * 3.25), 0.5 + (m * 1.5), iz * 10);
		glVertex3f(0, 0.5 + (m * 1.5), iz * 10);
		glEnd();

		glColor3f(0.78, 0.78, 0.78);//poddach
		glBegin(GL_QUADS);
		glVertex3f(0 + (n * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(0.25 + (n * 3.25), 0.0 + (m * 1.5), iz * 10);
		glVertex3f(0.25 + (n * 3.25), 0.5 + (m * 1.5), iz * 10);
		glVertex3f(0 + (n * 3.25), 0.5 + (m * 1.5), iz * 10);
		glEnd();
	}

	for (int ix = 0; ix < 2; ix++)
	{
		for (int iz = 0; iz < 2; iz++)
		{
			glColor3f(0.78, 0.78, 0.78);
			glBegin(GL_QUADS);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 0, 0 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 0, 4 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 4 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, (m * 1.5 + 0.5), 0 + (iz * 6));
			glEnd();

			glColor3f(0.78, 0.78, 0.78);
			glBegin(GL_QUADS);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 0, 4 + (iz * 2));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0, 4 + (iz * 2));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, (m*1.5 + 0.5), 4 + (iz * 2));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, (m*1.5 + 0.5), 4 + (iz * 2));
			glEnd();
		}
	}

	for (int ix = 0; ix < 2; ix++)
	{
		for (int iy = 0; iy < m + 1; iy++)
		{
			if (iy == 0 && door == ix)
			{
				glColor3f(0, 0, 1);//drzwi
				glBegin(GL_QUADS);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0 + (iy*1.5), 4);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0 + (iy*1.5), 6);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (iy*1.5), 6);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (iy*1.5), 4);
				glEnd();

				//daszek
				for (int i = 0; i < 2; i++)
				{
					glColor3f(0.27, 0.27, 0.27);
					glBegin(GL_QUADS);
					glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (i*0.25), 4);
					glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.5 + (i*0.25), 4);
					glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.5 + (i*0.25), 6);
					glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (i*0.25), 6);
					glEnd();
				}
				glColor3f(0.27, 0.27, 0.27);
				glBegin(GL_QUADS);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.5, 4);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.75, 4);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.75, 6);
				glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, 1.5, 6);
				glEnd();

				continue;
			}
			//boku
			glColor3f(0.78, 0.78, 0.78);//poziom
			glBegin(GL_QUADS);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.0 + (iy*1.5), 4);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.0 + (iy*1.5), 6);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.5 + (iy*1.5), 6);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.5 + (iy*1.5), 4);
			glEnd();

			if (iy == m) continue;

			glColor3f(0, 0, 1);//okna
			glBegin(GL_QUADS);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.5 + (iy*1.5), 4);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 0.5 + (iy*1.5), 6);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (iy*1.5), 6);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5 + (iy*1.5), 4);
			glEnd();
		}
	}

	glColor3f(0, 0, 0);//dach
	glBegin(GL_QUADS);
	glVertex3f((3.25*n) + 0.25 - 2, (m*1.5 + 0.5), 0);
	glVertex3f((3.25*n) + 0.25 - 2, (m*1.5 + 0.5), 10);
	glVertex3f(2, (m*1.5 + 0.5), 10);
	glVertex3f(2, (m*1.5 + 0.5), 0);

	for (int ix = 0; ix < 2; ix++)
	{
		for (int iz = 0; iz < 2; iz++)
		{
			glColor3f(0, 0, 0);//dach
			glBegin(GL_QUADS);
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, (m*1.5 + 0.5), 0 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 : 0, (m*1.5 + 0.5), 4 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, (m*1.5 + 0.5), 4 + (iz * 6));
			glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, (m*1.5 + 0.5), 0 + (iz * 6));
		}
	}

	glEnd();
}

void Builds_tower(int door = 1, int m = 20, int n = 6)
{
	glPushMatrix();
	tower(door, m, n);
	glPopMatrix();
}

#pragma endregion tower

#pragma region other_buildings

void Market()
{
	for (int ix = 0; ix < 2; ix++)
	{
		for (int iz = 0; iz < 15; iz++)
		{
			glColor3f(1, 1, 0.2);
			glBegin(GL_QUADS);
			glVertex3f(15 * ix, 1.5, 0 + iz);
			glVertex3f(15 * ix, 1.5, 1 + iz);
			glVertex3f(15 * ix, 2, 1 + iz);
			glVertex3f(15 * ix, 2, 0 + iz);
			glEnd();

			if ((iz == 7 || iz == 8) && ix == 0) {
				glColor3f(0, 0, 1);
				glBegin(GL_QUADS);
				glNormal3f(0, 0, 1);
				glVertex3f(15 * ix, 0, 0 + iz);
				glVertex3f(15 * ix, 0, 1 + iz);
				glVertex3f(15 * ix, 1.5, 1 + iz);
				glVertex3f(15 * ix, 1.5, 0 + iz);
				glEnd();
				continue;
			}

			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(15 * ix, 0, 0 + iz);
			glVertex3f(15 * ix, 0, 1 + iz);
			glVertex3f(15 * ix, 0.5, 1 + iz);
			glVertex3f(15 * ix, 0.5, 0 + iz);
			glEnd();

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex3f(15 * ix, 0, 0 + iz);
			glVertex3f(15 * ix, 0, 1 + iz);
			glVertex3f(15 * ix, 0.75, 1 + iz);
			glVertex3f(15 * ix, 0.75, 0 + iz);
			glEnd();

			if (ix == 0)
			{
				glColor3f(0, 0, 1);
				glBegin(GL_QUADS);
				glVertex3f(15 * ix, 0.75, 0 + iz);
				glVertex3f(15 * ix, 0.75, 1 + iz);
				glVertex3f(15 * ix, 1.5, 1 + iz);
				glVertex3f(15 * ix, 1.5, 0 + iz);
				glNormal3f(1, 0, 0);
				glEnd();
			}
			else
			{
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex3f(15 * ix, 0.75, 0 + iz);
				glVertex3f(15 * ix, 0.75, 1 + iz);
				glVertex3f(15 * ix, 1.5, 1 + iz);
				glVertex3f(15 * ix, 1.5, 0 + iz);
				glEnd();
			}
		}
	}

	for (int iz = 0; iz < 2; iz++)
	{
		for (int ix = 0; ix < 15; ix++)
		{
			glColor3f(1, 1, 0.2);
			glBegin(GL_QUADS);
			glVertex3f(ix, 1.5, 15 + (iz*-15));
			glVertex3f(ix + 1, 1.5, 15 + (iz*-15));
			glVertex3f(ix + 1, 2, 15 + (iz*-15));
			glVertex3f(ix, 2, 15 + (iz*-15));

			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(ix, 0, 15 + (iz*-15));
			glVertex3f(ix + 1, 0, 15 + (iz*-15));
			glVertex3f(ix + 1, 0.5, 15 + (iz*-15));
			glVertex3f(ix, 0.5, 15 + (iz*-15));
			glEnd();

			glColor3f(1, 1, 1);
			glBegin(GL_QUADS);
			glVertex3f(ix, 0, 15 + (iz*-15));
			glVertex3f(ix + 1, 0, 15 + (iz*-15));
			glVertex3f(ix + 1, 0.75, 15 + (iz*-15));
			glVertex3f(ix, 0.75, 15 + (iz*-15));
			glEnd();

			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex3f(ix, 0.75, 15 + (iz*-15));
			glVertex3f(ix + 1, 0.75, 15 + (iz*-15));
			glVertex3f(ix + 1, 1.5, 15 + (iz*-15));
			glVertex3f(ix, 1.5, 15 + (iz*-15));
			glNormal3f(0, 0, 1);
			glEnd();
		}
	}


	//dach
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 2, 0);
	glVertex3f(0, 2, 15);
	glVertex3f(15, 2, 15);
	glVertex3f(15, 2, 0);
	glEnd();
}

void Studio()
{
	glScalef(10, 3, 10);
	glEnable(GL_TEXTURE_2D);
	texture_front(0);
	texture_back(0);
	texture_left(0);
	texture_right(0);
	texture_top(1);
	glDisable(GL_TEXTURE_2D);

}

void Management_MS()
{
	for (int ix = 0; ix < 2; ix++)
	{
		for (int iz = 0; iz < 7; iz++)
		{
			glColor3f(0.87, 0.87, 0.87);
			glBegin(GL_QUADS);
			glVertex3f(7 * ix, 1.5, 0 + iz);
			glVertex3f(7 * ix, 1.5, 1 + iz);
			glVertex3f(7 * ix, 2, 1 + iz);
			glVertex3f(7 * ix, 2, 0 + iz);
			glEnd();

			if (iz == 2) {
				glColor3f(0, 0, 1);
				glBegin(GL_QUADS);
				glVertex3f(7 * ix, 0, 0 + iz);
				glVertex3f(7 * ix, 0, 1 + iz);
				glVertex3f(7 * ix, 1.5, 1 + iz);
				glVertex3f(7 * ix, 1.5, 0 + iz);
				glNormal3f(0, 0, 1);
				glEnd();
				continue;
			}

			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(7 * ix, 0, 0 + iz);
			glVertex3f(7 * ix, 0, 1 + iz);
			glVertex3f(7 * ix, 0.5, 1 + iz);
			glVertex3f(7 * ix, 0.5, 0 + iz);
			glEnd();

			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex3f(7 * ix, 0.5, 0 + iz);
			glVertex3f(7 * ix, 0.5, 1 + iz);
			glVertex3f(7 * ix, 1.5, 1 + iz);
			glVertex3f(7 * ix, 1.5, 0 + iz);
			glNormal3f(1, 0, 0);
			glEnd();
		}
	}


	for (int ix = 0; ix < 7; ix++)
	{
		glColor3f(0.87, 0.87, 0.87);
		glBegin(GL_QUADS);
		glVertex3f(ix, 1.5, 7);
		glVertex3f(ix + 1, 1.5, 7);
		glVertex3f(ix + 1, 2, 7);
		glVertex3f(ix, 2, 7);
		glEnd();

		glColor3f(0.27, 0.27, 0.27);
		glBegin(GL_QUADS);
		glVertex3f(ix, 0, 0);
		glVertex3f(ix + 1, 0, 0);
		glVertex3f(ix + 1, 0.5, 0);
		glVertex3f(ix, 0.5, 0);
		glEnd();

		glColor3f(0.87, 0.87, 0.87);
		glBegin(GL_QUADS);
		glVertex3f(ix, 0.5, 0);
		glVertex3f(ix + 1, 0.5, 0);
		glVertex3f(ix + 1, 2, 0);
		glVertex3f(ix, 2, 0);
		glEnd();

		if (ix == 3) {

			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex3f(ix, 0, 7);
			glVertex3f(ix + 1, 0, 7);
			glVertex3f(ix + 1, 1.5, 7);
			glVertex3f(ix, 1.5, 7);
			glNormal3f(1, 0, 0);
			glEnd();
			continue;
		}

		glColor3f(0.27, 0.27, 0.27);
		glBegin(GL_QUADS);
		glVertex3f(ix, 0, 7);
		glVertex3f(ix + 1, 0, 7);
		glVertex3f(ix + 1, 0.5, 7);
		glVertex3f(ix, 0.5, 7);
		glEnd();

		glColor3f(0, 0, 1);
		glBegin(GL_QUADS);
		glVertex3f(ix, 0.5, 7);
		glVertex3f(ix + 1, 0.5, 7);
		glVertex3f(ix + 1, 1.5, 7);
		glVertex3f(ix, 1.5, 7);
		glNormal3f(1, 0, 0);
		glEnd();
	}

	//dach
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(0, 2, 0);
	glVertex3f(0, 2, 7);
	glVertex3f(7, 2, 7);
	glVertex3f(7, 2, 0);
	glEnd();


}

void Shed()
{
	float height_roof = 1;

	//glRotatef(0, 0, 1, 0);
	for (int iz = 0; iz < 2; iz++)
	{
		for (int ix = 0; ix < 10; ix++)
		{
			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(0 + (ix * 1.5), 0, 0 + (iz * 5));
			glVertex3f(0.5 + (ix * 1.5), 0, 0 + (iz * 5));
			glVertex3f(0.5 + (ix * 1.5), 0.5, 0 + (iz * 5));
			glVertex3f(0 + (ix * 1.5), 0.5, 0 + (iz * 5));
			glEnd();

			glColor3f(0.78, 0.78, 0.78);
			glBegin(GL_QUADS);
			glVertex3f(0 + (ix * 1.5), 0.5, 0 + (iz * 5));
			glVertex3f(0.5 + (ix * 1.5), 0.5, 0 + (iz * 5));
			glVertex3f(0.5 + (ix * 1.5), 2, 0 + (iz * 5));
			glVertex3f(0 + (ix * 1.5), 2, 0 + (iz * 5));
			glEnd();

			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(0.5 + (ix*1.5), 0, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 0, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 0.5, 0 + (iz * 5));
			glVertex3f(0.5 + (ix*1.5), 0.5, 0 + (iz * 5));
			glEnd();

			glColor3f(0, 0, 1);
			glBegin(GL_QUADS);
			glVertex3f(0.5 + (ix*1.5), 0.5, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 0.5, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 1.5, 0 + (iz * 5));
			glVertex3f(0.5 + (ix*1.5), 1.5, 0 + (iz * 5));
			glNormal3f(1, 0, 0);
			glEnd();

			glColor3f(0.78, 0.78, 0.78);
			glBegin(GL_QUADS);
			glVertex3f(0.5 + (ix*1.5), 1.5, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 1.5, 0 + (iz * 5));
			glVertex3f(1.5 + (ix*1.5), 2, 0 + (iz * 5));
			glVertex3f(0.5 + (ix*1.5), 2, 0 + (iz * 5));
			glEnd();
		}

		glColor3f(0.27, 0.27, 0.27);
		glBegin(GL_QUADS);
		glVertex3f(0 + (10 * 1.5), 0, 0 + (iz * 5));
		glVertex3f(0.5 + (10 * 1.5), 0, 0 + (iz * 5));
		glVertex3f(0.5 + (10 * 1.5), 0.5, 0 + (iz * 5));
		glVertex3f(0 + (10 * 1.5), 0.5, 0 + (iz * 5));
		glEnd();

		glColor3f(0.78, 0.78, 0.78);
		glBegin(GL_QUADS);
		glVertex3f(0 + (10 * 1.5), 0.5, 0 + (iz * 5));
		glVertex3f(0.5 + (10 * 1.5), 0.5, 0 + (iz * 5));
		glVertex3f(0.5 + (10 * 1.5), 2, 0 + (iz * 5));
		glVertex3f(0 + (10 * 1.5), 2, 0 + (iz * 5));
		glEnd();
	}

	for (int ix = 0; ix < 2; ix++)
	{
		for (int iz = 0; iz < 5; iz++)
		{
			if (iz == 2 && ix == 0)
			{
				glColor3f(0.44, 0.22, 0);
				glBegin(GL_QUADS);
				glVertex3f(0, 0, 0 + iz);
				glVertex3f(0, 0, 1 + iz);
				glVertex3f(0, 1.5, 1 + iz);
				glVertex3f(0, 1.5, 0 + iz);
				glEnd();

				glColor3f(0.78, 0.78, 0.78);
				glBegin(GL_QUADS);
				glVertex3f(0, 1.5, 0 + iz);
				glVertex3f(0, 1.5, 1 + iz);
				glVertex3f(0, 2, 1 + iz);
				glVertex3f(0, 2, 0 + iz);
				glEnd();

				continue;
			}

			glColor3f(0.27, 0.27, 0.27);
			glBegin(GL_QUADS);
			glVertex3f(0 + (ix * 15.5), 0, 0 + iz);
			glVertex3f(0 + (ix * 15.5), 0, 1 + iz);
			glVertex3f(0 + (ix * 15.5), 0.5, 1 + iz);
			glVertex3f(0 + (ix * 15.5), 0.5, 0 + iz);
			glEnd();

			glColor3f(0.78, 0.78, 0.78);
			glBegin(GL_QUADS);
			glVertex3f(0 + (ix * 15.5), 0.5, 0 + iz);
			glVertex3f(0 + (ix * 15.5), 0.5, 1 + iz);
			glVertex3f(0 + (ix * 15.5), 2, 1 + iz);
			glVertex3f(0 + (ix * 15.5), 2, 0 + iz);
			glEnd();
		}

		glColor3f(1, 1, 1);
		glBegin(GL_TRIANGLES);
		glVertex3f(0 + (ix*15.5), 2, 0);
		glVertex3f(0 + (ix*15.5), 2, 5);
		glVertex3f(0 + (ix*15.5), 2 + height_roof, 2.5);
		glEnd();
	
	}

	Point3Ds p;
	p.x = 2.5;
	p.y = height_roof;
	p.z = 15.5;

	glTranslatef(15.5, 2, 0);
	glRotatef(-90, 0, 1, 0);
	texture_free(2, p);
	glRotatef(90, 0, 1, 0);
	glTranslatef(-15.5, 0, 5);
	glRotatef(90, 0, 1, 0);
	texture_free(2, p);

	glTranslatef(-8, -2, 15);
	glScalef(7, 2, 5);
	GLfloat points2[12 * 3] =
	{ 0,0,0,     0.2,0,0,   0.6,0,0,    1,0,0,
		0,0,0.5,     0.3,3,0.5,   0.6,2,0.5,    1,0,0.5,
		0,0,1,     0.4,0,1,   0.6,0,1,    1,0,1
	};
	texture_surface(points2, 13);

}

#pragma endregion other_buildings

#pragma region basic

void fence_omega_filutek() 
{
	for (int i = 0; i < 30; i++)
	{
		fence(i);
	}

	glTranslatef(30, 0, 0);
	glRotatef(-90, 0, 1, 0);
	for (int i = 0; i < 7; i++) {
		fence(i);
	}
}

void fence_stokrotka_starszy_dwor()
{
	for (int i = 0; i < 40; i++)
	{
		fence(i);
	}
}

void Bulidings()
{
	glPushMatrix();
	Builds_twins(2); // Bonus
	glTranslatef(36, 0, 0);
	Builds_twins(1); //Hajduczek
	glTranslated(36, 0, -5);
	Builds_twins(0, 1, 1); //Omega 
	glTranslatef(36, 0, -50);

	
	glPushMatrix();
	glTranslatef(-6.5, 0, 10);
	glRotatef(-90, 0, 1, 0);
	fence_omega_filutek();
	glPopMatrix();
	
	glRotatef(180, 0, 1, 0);
	Builds_twins(0, 1, 1); // Filutek
	
	glPushMatrix();
	glTranslatef(20, 0, 50);
	Builds_tower(1); // Babilon
	glTranslatef(30, 0, 15);
	Builds_tower(1); // Olimip
	glTranslatef(8, 0, -25);
	glScalef(1.5, 1, 1.5);
	Management_MS(); // Dyrekcja MS
	glPopMatrix();

	glTranslatef(36, 0, 0);
	Builds_twins(0, 1); //Stumyk
	glTranslatef(36, 0, 5);
	Builds_twins(3); //Baratek
	glTranslatef(36, 0, 0);
	Builds_twins(0, 0, 1); //Zascianek
	glTranslatef(36, 0, -5);
	Builds_twins(0, 1); //Stokrotka

	glPushMatrix();
	glTranslatef(36, 0, 0);
	glRotatef(90, 0, 1, 0);
	fence_stokrotka_starszy_dwor();
	glPopMatrix();

	glTranslatef(36, 0, -40);
	glRotatef(180, 0, 1, 0);
	Builds_twins(0, 1, 1); //Starszy dwor
	glTranslatef(36, 0, -5);
	Builds_twins(0, 1); // Promyk

	glTranslatef(-30, 0, 50);
	Builds_tower(1); // Kapitol
	glPopMatrix();

	glPushMatrix();
	glTranslatef(65, 0, 35);
	glScalef(1.5, 1.3, 1.5);
	Market();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 70);
	Studio();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-60, 0, -130);
	Shed();
	glPopMatrix();
}

void Courts()
{
	glPushMatrix();

		//baskeball
	glTranslatef(35, 0, -13);
	glRotatef(90, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(12.8, 0.5, 0);
	glTexCoord2f(1, 1); glVertex3f(12.8, 0.5, 12.8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 12.8);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(41, 0, -20.8);
	glRotatef(90, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, 0.5, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(10, 0.5, 0);
	glTexCoord2f(1, 1); glVertex3f(10, 0.5, 10);
	glTexCoord2f(0.0f, 1); glVertex3f(5, 0.5, 10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(47.8, 0, -14);
	glRotatef(90, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[7]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(5, 0.5, 0);
	glTexCoord2f(1, 0.0f); glVertex3f(12, 0.5, 0);
	glTexCoord2f(1, 1); glVertex3f(12, 0.5, 12);
	glTexCoord2f(0.0f, 1); glVertex3f(5, 0.5, 12);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//parking
	glTranslatef(15, 0, -80);
	glRotatef(180, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(8, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(8, 0.5, 8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 8);
	glEnd();
	
	glTranslatef(0, 0,-7);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(8, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(8, 0.5, 8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 8);
	glEnd();

	glTranslatef(0, 0, -7);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(8, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(8, 0.5, 8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 8);
	glEnd();

	glTranslatef(0, 0, -7);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(8, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(8, 0.5, 8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 8);
	glEnd();	
	glDisable(GL_TEXTURE_2D);

	//boisko do nogi
	glTranslatef(10, 0, 30);
	glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(1, 0, 1);
	high_fence(24, 10, 15, 3);
	glTranslatef(0, 0, 23.5);
	high_fence(24, 10, 15, 3);
	glRotatef(90, 0, 1, 0);
	high_fence(24, 10, 15, 3);
	glTranslatef(0, 0, 23.5);
	high_fence(24, 10, 15, 3);
	glPopMatrix();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(24.5, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(24.5, 0.5, 24.5);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 24.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//tenis
	glTranslatef(10, 0, 30);
	//glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(2, 0, 2);
	high_fence(20, 10, 15, 3);
	glTranslatef(0, 0, 20);
	high_fence(20, 10, 15, 3);
	glRotatef(90, 0, 1, 0);
	high_fence(20, 10, 15, 3);
	glTranslatef(0, 0, 20);
	high_fence(20, 10, 15, 3);

	glPopMatrix();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[10]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2, 0.5, 2);
	glTexCoord2f(1, 0.0f); glVertex3f(22, 0.5, 2);
	glTexCoord2f(1, 1); glVertex3f(22, 0.5, 22);
	glTexCoord2f(0.0f, 1); glVertex3f(2, 0.5, 22);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//parking pod filutkiem
	glTranslatef(33, 0, -86);
	glRotatef(180, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(8, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(8, 0.5, 8);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 8);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	//korty koło grilli
	glTranslatef(-20, 0, -85);
	glRotatef(90, 0, 1, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[11]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(10, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(10, 0.5, 10);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 10);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glTranslatef(-10, 0, -10.5);
	//glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glTranslatef(1, 0, 1);
	high_fence(9, 4, 10, 3);
	glTranslatef(0, 0, 9);
	high_fence(9, 4, 10, 3);
	glRotatef(90, 0, 1, 0);
	high_fence(9, 4, 10, 3);
	glTranslatef(0, 0, 9);
	high_fence(9, 4, 10, 3);
	
	glPopMatrix();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[12]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, 0.5, 1);
	glTexCoord2f(1, 0.0f); glVertex3f(10, 0.5, 1);
	glTexCoord2f(1, 1); glVertex3f(10, 0.5, 10);
	glTexCoord2f(0.0f, 1); glVertex3f(1, 0.5, 10);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void High_roads()
{
	glPushMatrix();
	glTranslatef(-60, 0, 90);
	glRotatef(90, 0, 1, 0);

	for (int i = 0; i < 80; i++) road(i,3);

	glTranslatef(0, 0, 145);
	for (int i = 0; i < 80; i++) road(i, 3);

	glTranslatef(185, 0, 20);
	glRotatef(90, 0, 1, 0);
	for (int i = 0; i < 63; i++) road(i, 3);
	
	glPopMatrix();
}

void Low_roads()
{
	glPushMatrix();
	
	glTranslatef(57, 0, 48);  //od nawojki do studia 
	for (int i = -50; i < 28; i++) road(i, 1);

	glTranslatef(57, 0, -35);  // budryka
	for (int i = -171; i < -40; i++) road(i, 1);

	glRotatef(90, 0, 1, 0);
	glTranslatef(-20, 0, -40); // budryka do studia
	for (int i = -15; i < 20; i++) road(i, 1);

	glRotatef(90, 0, 1, 0);
	glTranslatef(113, 0, 83); //od reymonta do filutka
	for (int i = -101; i < 18; i++) road(i, 1);

	glRotatef(90, 0, 1, 0);
	glTranslatef(0, 0, -50); //od reymonta do filutka
	for (int i = -10; i < -1; i++) road(i, 1);

	glPopMatrix();
}

void Sidewalks()
{
	glPushMatrix();
	
	glTranslatef(57, 0, 49);  //wzdłuż - nawojki do studia 
	for (int i = -150; i < 93; i++)  sidewalk(i, 0.3);

	glTranslatef(0, 0, -1.3);  
	for (int i = -160; i < 57; i++)   sidewalk(i, 0.3);
	for (int i = 60; i < 93; i++)   sidewalk(i, 0.3);

	glTranslatef(57, 0, -35);  // budryka
	for (int i = -570; i < -130; i++) sidewalk(i, 0.3);

	glTranslatef(0, 0, 1.3);
	for (int i = -570; i < -133; i++) sidewalk(i, 0.3);

	glRotatef(90, 0, 1, 0);
	glTranslatef(-20, 0, -40.3); // budryka do studia
	for (int i = -46; i < 67; i++) sidewalk(i, 0.3);

	glTranslatef(0, 0, 1.3); 
	for (int i = -46; i < 71; i++) sidewalk(i, 0.3);
	
	glTranslatef(0, 0, -33); // wzdłuż lewiatana
	for (int i = -14; i < 20; i++) sidewalk(i, 1);

	glTranslatef(0, 0, 4); // wzdłuż lewiatana
	for (int i = 14; i < 32; i++) sidewalk(i, 1.5);

	glTranslatef(0.2, 0, 13.2); // do omegi
	for (int i = 16; i < 30; i++) sidewalk(i, 1.3);

	glPushMatrix();
	glTranslatef(0,0,-24); 
	for (int i = 16; i < 22; i++) sidewalk(i, 1.3);

	glTranslatef(0, 0, -24);
	for (int i = 16; i < 22; i++) sidewalk(i, 1.3);
	glPopMatrix();

	glTranslatef(-14.8, 0, -41.5); // do studia pod blizniaki
	for (double i = 0.4; i < 22.1; i+=0.3) sidewalk(i, 1.5);
	for (double i = 23.9; i < 64.8; i += 0.3) sidewalk(i, 1.5);
	for (double i = 66.4; i < 95; i += 0.3) sidewalk(i, 1.5);

	glTranslatef(0, 0, -13); // do budryka pod blizniaki
	for (double i = 23.9; i < 64.8; i += 0.3) sidewalk(i, 1.5);

	glTranslatef(0, 0, 17.5); // do budryka pod blizniaki
	for (double i = 56.8; i < 64.8; i += 0.3) sidewalk(i, 1.5);

	glTranslatef(10, 0, -24); // budryka do studia
	for (int i = -60; i < 82; i++) sidewalk(i, 0.3);

	glTranslatef(18, 0, -28.5); // budryka pod kapitol
	for (int i = -27; i < 16; i++) sidewalk(i, 0.4);

	glTranslatef(10.3, 0, 17.5); // do strasznego dworu
	for (int i = -2; i < 14; i++) sidewalk(i, 1.3);

	glTranslatef(0, 0, -23.9); // do strasznego dworu
	for (int i = -2; i < 11; i++) sidewalk(i, 1.3);

	glTranslatef(-0.2, 0, -20.3); // wzdłuz reymonta
	for (int i = -158; i < -7; i++) sidewalk(i, 0.5);
	for (int i = -5; i < 121; i++) sidewalk(i, 0.5);
	for (int i = 123; i < 212; i++) sidewalk(i, 0.5);

	glTranslatef(0, 0, -3.5);
	for (int i = -158; i < 212; i++) sidewalk(i, 0.5);

	glTranslatef(-0.5, 0, 145); // wzdłuz nawojki
	for (int i = -158; i < -76; i++) sidewalk(i, 0.5);
	for (int i = -74; i < 212; i++) sidewalk(i, 0.5);

	glTranslatef(0, 0, 3.5);
	for (int i = -158; i < 212; i++) sidewalk(i, 0.5);

	glTranslatef(52, 0, -124.5);
	for (double i = 0.2; i < 6.1; i+=0.3) sidewalk(i, 1.3);

	glTranslatef(0.9, 0, 95.9);  // do filutka
	for (int i = 0; i < 6; i++) sidewalk(i, 1.3);

	glTranslatef(0, 0, -24);
	for (int i = 0; i < 6; i++) sidewalk(i, 1.3);

	glPushMatrix();
	glTranslatef(0, 0, -24);
	for (int i = 0; i < 6; i++) sidewalk(i, 1.3);

	glTranslatef(0, 0, -24);
	for (int i = 0; i < 6; i++) sidewalk(i, 1.3);

	glPopMatrix();

	glRotatef(90, 0, 1, 0);
	glTranslatef(0.1, 0, 9.1); // miedzy filutkiem a kortami
	for (int i = -89; i < 81; i++) sidewalk(i, 0.3);
	for (int i = 84; i < 308; i++) sidewalk(i, 0.3);

	glTranslatef(0, 0, -1.3);
	for (int i = -89; i < 308; i++) sidewalk(i, 0.3);

	glTranslatef(15.5, 0, 15.7); //do apteki
	for (int i = -9; i < 2; i++) sidewalk(i, 0.8);

	glTranslatef(0, 0, 7.6); //do babilonu
	for (int i = -50; i < 2; i++) sidewalk(i, 0.6);

	glTranslatef(0, 0, 10); //do olimpu
	for (int i = -15; i < 2; i++) sidewalk(i, 0.6);


	glTranslatef(0, 0, 11.5); //wzdłuż piastowskiej
	for (int i = -130; i < 153; i++) sidewalk(i, 0.5);
	glTranslatef(0, 0, 3.5); //wzdłuż piastowskiej
	for (int i = -130; i < 153; i++) sidewalk(i, 0.5);

	glRotatef(20, 0, 1, 0);
	glTranslatef(4, 0, -25.8); //skos miedzy olimpem a babilonem
	for (int i = -160; i < -4; i++) sidewalk(i, 0.4);

	glRotatef(70, 0, 1, 0);
	glTranslatef(-0.2, 0, -20); 
	for (int i = -18; i < 54; i++) sidewalk(i, 0.4);

	glTranslatef(0, 0, -15.2);
	for (int i = -32; i < 54; i++) sidewalk(i, 0.4);

	glTranslatef(-0.5, 0, 79.55);	//kolo kortów
	for (int i = -15; i < 22; i++) sidewalk(i, 1);

	glTranslatef(0.1, 0, 10);	//kolo kortów
	for (int i = 16.7; i < 22; i++) sidewalk(i, 1);

	glTranslatef(-27, 0, 5);	//kolo kortów
	for (int i = 16.7; i < 23; i++) sidewalk(i, 1);

	glRotatef(90, 0, 1, 0);
	glTranslatef(-25.1, 0, 16.6);	//kolo kortów
	for (int i = 3; i < 30; i++) sidewalk(i, 1);

	glRotatef(30, 0, 1, 0);
	glTranslatef(100.2, 0, 90);	//od filutka na przystanek
	for (int i = -8; i < 29; i++) sidewalk(i, 0.5);

	glRotatef(120, 0, 1, 0);
	glTranslatef(1, 0,-8);	//od filutka na przystanek
	for (int i = -40; i < -3; i++) sidewalk(i, 0.5);

	glRotatef(30, 0, 1, 0);
	glTranslatef(1, 0, -13.5);	//wzdłuż filutka
	for (int i = 5; i < 48; i++) sidewalk(i, 1);

	glTranslatef(30, 0, 3.4);	//wzdłuż filutka
	for (int i = 5; i < 48; i++) sidewalk(i, 1);

	glTranslatef(-9.7, 0, -13.2);	//przez srodek grilli
	for (int i = 5; i < 93; i++) sidewalk(i, 1);

	glRotatef(90, 0, 1, 0);
	glTranslatef(-10, 0, 70.1);	//miedzy kortami
	for (int i = -5; i < 24; i++) sidewalk(i, 0.8);

	glRotatef(130, 0, 1, 0);
	glTranslatef(0, 0, 4);	//z kortów do zascianka skos
	for (int i = -9; i < 24; i++) sidewalk(i, 0.5);

	glTranslatef(11, 0, 12);	//skosy
	for (double i = -25.5; i < 35; i++) sidewalk(i, 0.5);

	glRotatef(10, 0, 1, 0);
	glTranslatef(9.5, 0, 12);
	for (int i = -9; i < 24; i++) sidewalk(i, 0.5);

	glRotatef(90, 0, 1, 0);
	glTranslatef(-2, 0, 3);
	for (int i = -88; i < 24; i++) sidewalk(i, 0.5);

	glRotatef(20, 0, 1, 0);
	glTranslatef(-8, 0, 6);
	for (int i = -37; i < 11; i++) sidewalk(i, 0.5);

	glRotatef(110, 0, 1, 0);
	glTranslatef(0, 0, -18);
	for (int i = -2; i < 10; i++) sidewalk(i, 0.8);

	glRotatef(150, 0, 1, 0);
	glTranslatef(-20, 0, -2.5);
	for (int i = -30; i < 12; i++) sidewalk(i, 0.8);

	glRotatef(55, 0, 1, 0);
	glTranslatef(-10, 0, -17.5);
	for (int i = -5; i < 60; i++) sidewalk(i, 0.5);

	//pod lewiatanem 
	glRotatef(65, 0, 1, 0);
	glTranslatef(-20, 0, -33); //lewiatan kapitol reymonta
	for (int i = -5; i < 94; i++) sidewalk(i, 1);

	glTranslatef(20, 0, -8.2); //pod studiem
	for (int i = -3; i < 38; i++) sidewalk(i, 0.5);

	//skosy
	glRotatef(150, 0, 1, 0);
	glTranslatef(-3, 0, -1);
	for (int i = -25; i < 9; i++) sidewalk(i, 0.5);

	glRotatef(17, 0, 1, 0);
	glTranslatef(-20, 0, -3.2);
	for (int i = -60; i < 4; i++) sidewalk(i, 0.5);

	glRotatef(-25, 0, 1, 0);
	glTranslatef(45, 0, -14);
	for (int i = -105; i < 5; i++) sidewalk(i, 0.5);

	glTranslatef(-5, 0, -13.9); //od lewiatana pod staszny dwor
	for (int i = -60; i < -3; i++) sidewalk(i, 0.5);

	glPopMatrix();
}

void Object()
{
	glPushMatrix();
	Object_OBJ();

	glTranslatef(30, 0, 8);
	Object_OBJ();

	glTranslatef(-60, 0, 4);
	Object_OBJ();

	glPopMatrix();
}

void Fundamentals()
{
	glPushMatrix();
	glColor3f(0, 0.8, 0);
	glScalef(10, 0.001, 10);
	glutSolidSphere(100, 100, 23);
	glPopMatrix();	
}

void Basic()
{
	LoadGLTextures();

	glTranslatef(0, -10, 30);
	Fundamentals();
	glScalef(0.5, 0.5, 0.5);
	Bulidings();

	glScalef(1.5, 0.5, 1.5);
	High_roads();
	Low_roads();
	Sidewalks();
	Courts();
	//Object();	
}

#pragma endregion basic

void GenerujListyWyswietlania()
{
	idlisty = glGenLists(4);

	// utworzenie pierwszej listy
	glNewList(idlisty + 0, GL_COMPILE);

	Basic();

	glEndList();
	
	// utworzenie drugiej listy
	glNewList(idlisty + 1, GL_COMPILE);
	
	glEnable(GL_FOG);

	float fog_color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glHint(GL_FOG_HINT, GL_FASTEST);
	glFogfv(GL_FOG_COLOR, fog_color);
	glFogf(GL_FOG_START, 0.2f);
	glFogf(GL_FOG_END, 25.0f);
	glFogf(GL_FOG_DENSITY, denisty_fog);
	glFogi(GL_FOG_MODE, GL_EXP2);

	Basic();

	glDisable(GL_FOG);

	glEndList();

	// utworzenie trzeciej listy
	glNewList(idlisty + 2, GL_COMPILE);

	Basic();

	glEndList();
}

void UstawKamere()
{
	double cosx, siny, sinx;
	double CelX, CelY, CelZ;

	// wartości funkcji trygonometrycznych

	// obliczamy tylko raz dla oszczędności

	if (KeyDOWN) PredkoscKamery = -0.5*scale;
	if (KeyUP) PredkoscKamery = 0.5*scale;
	if (KeyLEFT) ObrotX -= 0.02*scale / 2;
	if (KeyRIGHT) ObrotX += 0.02*scale / 2;

	siny = sin(ObrotY);
	cosx = cos(ObrotX);
	sinx = sin(ObrotX);

	// nowa pozycja kamery

	eyex += cosx*PredkoscKamery;
	eyey += siny*PredkoscKamery;
	eyez += sinx*PredkoscKamery;

	// punkt wycelowania kamery powinien

	// znajdować się gdzieś "przed jej nosem"

	CelX = eyex + cosx;
	CelY = eyey + siny;
	CelZ = eyez + sinx;

	gluLookAt(eyex, eyey, eyez, CelX, CelY, CelZ, 0, 1, 0);

	//gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0 );

	PredkoscKamery = 0;
}

void Display()
{
	// kolor tła - zawartość bufora koloru
	if (figure == 2) {
		glClearColor(0.8, 0.8, 0.8, 1.0);
	}
	else
	{
		glClearColor(0.2421875, 0.578125, 1, 0.5);
	}
	//
	

	glEnable(GL_DEPTH_TEST);

	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);

	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();

	if (stereoscopic && figure != 3 && figure != 2)
	{
		stereoscopic_left_eye();
	}
	
	UstawKamere();

	if (figure == 3) lighting();

	if (figure == 0) glCallList(idlisty + 0);
	switch (figure)
	{
	case 1:
		glCallList(idlisty + 0);
		break;

	case 2:
		glCallList(idlisty + 1);
		break;

	case 3:
		glCallList(idlisty + 2);
		break;
	}

	if (stereoscopic && figure != 3 && figure != 2)
	{
		stereoscopic_right_eye();
	}

	// skierowanie poleceń do wykonania
	glFlush();

	// zamiana buforów koloru
	glutSwapBuffers();


	

}

// zmiana wielkości okna

void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);

	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);

	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();

	// obliczenie aspektu obrazu z uwzględnieniem
	// przypadku, gdy wysokość obrazu wynosi 0
	GLdouble aspect = 1;

	if (height > 0)

		aspect = width / (GLdouble)height;

	// rzutowanie perspektywiczne
	if (!stereoscopic) gluPerspective(fovy, aspect, 0.01, 100.0);

	// generowanie sceny 3D
	Display();
}

// obsługa klawiatury
void Klawisze(unsigned char key, int x, int y)

{
	// klawisz +
	if (key == '+' && fovy < 180)
		fovy++;

	// klawisz -
	if (key == '-' && fovy > 0)
		fovy--;

	if (key == 'a' && fovy > 0)
		kat++;

	if (key == 'w')
		scale += 0.5;
	//PredkoscKamery=0.02;

	if (key == 's')
		scale = +0.5;
	//PredkoscKamery=-0.02;

	if (key == 'e')
		ObrotX += 0.02;

	if (key == 'q')
		scale = 0.5;
	//ObrotX -= 0.02;

	if (key == 'r')
		ObrotY += 0.02;

	if (key == 'f')
		ObrotY -= 0.02;
	
	if (figure == 2)
	{
		if (key == 'o')
		{
			denisty_fog += 0.02;
			GenerujListyWyswietlania();
		}

		if (key == 'p')
		{
			if (denisty_fog >= 0)
			{
				denisty_fog -= 0.02;
				GenerujListyWyswietlania();
			}
			
		}
	}

	if (figure == 3)
	{
		if (key == 'o')
		{
			velocity_lighting += 0.5;
			GenerujListyWyswietlania();
		}

		if (key == 'p')
		{
			if (velocity_lighting >= 0) 
			{
				velocity_lighting -= 0.5;
				GenerujListyWyswietlania();

			}
			
		}
	}

	if (key == '1')
	{
		figure = 1;
		Display();
		GenerujListyWyswietlania();
	}

	if (key == '2')
	{
		if(!stereoscopic) figure = 2;
	}
		
	if (key == '3')
	{
		if(!stereoscopic) figure = 3;
	}

	if (stereoscopic)
	{
		if (key == 'o')
		{
			k_eye += 0.02;
			GenerujListyWyswietlania();
		}

		if (key == 'p')
		{
			k_eye -= 0.02;
			GenerujListyWyswietlania();
		}

		if (key == 'k')
		{
			zeroo += 0.5;
		}

		if (key == 'l')
		{
			zeroo -= 0.5;
		}
	}

	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void KlawiszeSpecjalne(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		eyex += 0.1;
		break;

		// kursor w górę
	case GLUT_KEY_UP:
		eyey -= 0.1;
		break;

		// kursor w prawo
	case GLUT_KEY_RIGHT:
		eyex -= 0.1;
		break;

		// kursor w dół
	case GLUT_KEY_DOWN:
		eyey += 0.1;
		break;
	}
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

// obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
	case EXIT:
		exit(0);

	case 1:
		figure = 1;
		break;

	case 2:
		figure = 2;
		break;

	case 3:
		figure = 3;
		break;
	}

	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void KeyDown(int key, int x, int y)
{
	switch (key) {

	case GLUT_KEY_UP:
		KeyUP = true;
		break;

	case GLUT_KEY_DOWN:
		KeyDOWN = true;
		break;

	case GLUT_KEY_LEFT:
		KeyLEFT = true;
		break;

	case GLUT_KEY_RIGHT:
		KeyRIGHT = true;
		break;
	}
}

void KeyUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		KeyUP = false;
		break;

	case GLUT_KEY_DOWN:
		KeyDOWN = false;
		break;

	case GLUT_KEY_LEFT:
		KeyLEFT = false;
		break;

	case GLUT_KEY_RIGHT:
		KeyRIGHT = false;
		break;
	}
}

int main(int argc, char * argv[])
{

	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);

	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// rozmiary głównego okna programu
	glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN) - 5, GetSystemMetrics(SM_CYSCREEN) - 5);

	// utworzenie głównego okna programu

	glutCreateWindow("Miasteczko Studenckie AGH");

	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(Display);

	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);

	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Klawisze);

	// dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
	// glutSpecialFunc( KlawiszeSpecjalne );

	glutSpecialFunc(KeyDown);

	glutSpecialUpFunc(KeyUp);

	// utworzenie menu podręcznego
	glutCreateMenu(Menu);

	// dodanie pozycji do menu podręcznego
	glutAddMenuEntry("Exit", EXIT);
	glutAddMenuEntry("Standard", 1);
	glutAddMenuEntry("Fog", 2);
	glutAddMenuEntry("Rys3", 3);

	// określenie przycisku myszki obsługującej menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	GenerujListyWyswietlania();

	glutIdleFunc(Display);
	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();

	return 0;
}

