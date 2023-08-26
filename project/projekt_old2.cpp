
#include "stdafx.h"

// stała do obsługi menu podręcznego

enum
{
	EXIT // wyjście
};
 
GLuint texture[5];


 AUX_RGBImageRec *LoadBMP(char* filename)
{
    FILE* file = NULL;

    if(!filename)
    {
       return NULL;
    }

    file = fopen(filename, "r");

    if(file)
    {
       fclose(file);
       return auxDIBImageLoad(filename);
    }

    return NULL;
}


int LoadGLTextures()
{
    int status = false;

    AUX_RGBImageRec *TextureImage[5];

    memset(TextureImage, 0, sizeof(void *)*1);

    if(TextureImage[0] = LoadBMP("crate.bmp"))
    {
        status = true;

        glGenTextures(1, &texture[0]);

        glBindTexture(GL_TEXTURE_2D, texture[0]);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     }

    if(TextureImage[1] = LoadBMP("metal.bmp"))
    {
        status = true;

        glGenTextures(1, &texture[1]);

        glBindTexture(GL_TEXTURE_2D, texture[1]);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[1]->sizeX, TextureImage[1]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[1]->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     }

    if(TextureImage[2] = LoadBMP("obraz.bmp"))
    {
        status = true;

        glGenTextures(1, &texture[2]);

        glBindTexture(GL_TEXTURE_2D, texture[2]);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[2]->sizeX, TextureImage[2]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[2]->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     }

    if(TextureImage[3] = LoadBMP("wall.bmp"))
    {
        status = true;

        glGenTextures(1, &texture[3]);

        glBindTexture(GL_TEXTURE_2D, texture[3]);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[3]->sizeX, TextureImage[3]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[3]->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     }


    if(TextureImage[4] = LoadBMP("wood.bmp"))
    {
        status = true;

        glGenTextures(1, &texture[4]);

        glBindTexture(GL_TEXTURE_2D, texture[4]);

        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[4]->sizeX, TextureImage[4]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[4]->data);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

     }

        if(TextureImage[0])
        {
            if(TextureImage[0]->data)
            {
               free(TextureImage[0]->data);

            }

            free(TextureImage[0]);
        }

     return status;
}

void Textures() {
	LoadGLTextures();
	glEnable(GL_TEXTURE_2D);


	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);


	// Front face

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-1.0f, 1.0f, 1.0f);

	// Back face

	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	// Top face

	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	// Bottom face

	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	// Right face

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	// Left face

	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}



double PredkoscKamery=0;
double scale = 5;
 
// pionowy kąt pola widzenia
 
GLdouble fovy = 60;
 
float czajnikrot=0;
 
// wpółrzędne położenia obserwatora
 
GLdouble eyex = 0;
 
GLdouble eyey = 0;
 
GLdouble eyez = 3;
 
GLdouble ObrotX=3.14/2.0;
 
GLdouble ObrotY=0;
 
bool KeyUP=false;
 
bool KeyDOWN=false;
 
bool KeyRIGHT=false;
 
bool KeyLEFT=false;
 
// współrzędne punktu w którego kierunku jest zwrócony obserwator,
 
GLdouble centerx = 0;
 
GLdouble centery = 0;
 
GLdouble centerz = -100;
 
int figure=0;
 
float kat=0;
 
GLuint idlisty;

void twins(int m, int n, int door, int peak, int club);
void twins_door(int ix, int iy, int iz);
// funkcja generująca scenę 3D

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
	glNormal3f(0, 0, -1);
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
	glNormal3f(0, 0, -1);
	glVertex3f(2.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 0.5 + (iy*1.5), iz * 10);
	glVertex3f(3.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(2.25 + (ix*3.25), 1.5 + (iy*1.5), iz * 10);
	glEnd();
}

void last_row_windows(int n, int iy, int iz)
{
	glColor3f(0.78, 0.78, 0.78);//między oknami
	glBegin(GL_QUADS);
	glVertex3f(0 + (n * 3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 0 + (iy*1.5), iz * 10);
	glVertex3f(0.25 + (n * 3.25), 1.5 + (iy*1.5), iz * 10);
	glVertex3f(0 + (n * 3.25), 1.5 + (iy*1.5), iz * 10);
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

#pragma region twins

void club(int m=2, int n=5)
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
		glEnd();

		glColor3f(0.78, 0.78, 0.78);//poddach
		glBegin(GL_QUADS);
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
		glVertex3f(1 + (ix*3.25) + (i*1.5), 0 , 0);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 1.5, 0);
		glVertex3f(1 + (ix*3.25) + (i*1.5), 1.5, -5);
		glEnd();
	}

	//dach
	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glVertex3f(1 + (ix*3.25), 1.5, 0);
	glVertex3f(2.5 + (ix*3.25), 1.5, 0);
	glVertex3f(2.5 + (ix*3.25), 1.5, -5);
	glVertex3f(1 + (ix*3.25), 1.5, -5);
	glEnd();

	glTranslated(((ix-3)*3.25), 0, -10);
	club();
	glTranslated(-((ix-3)*3.25), 0, 10);

	
	
}

void twins_door(int ix, int iy, int iz) {
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
	glNormal3f(0, 0, -1);
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
		glVertex3f((ix*3.25), 1.5+(i*0.25), 10);
		glVertex3f(((ix + 1)*3.25), 1.5 + (i*0.25), 10);
		glVertex3f(((ix + 1)*3.25), 1.5 + (i*0.25), 13);
		glVertex3f((ix*3.25), 1.5 + (i*0.25), 13);
		
		glVertex3f(((ix + 1)*3.25), 1.5 , 13);
		glVertex3f(((ix + 1)*3.25), 1.75, 13);
		glVertex3f((ix*3.25), 1.75, 13);
		glVertex3f((ix*3.25), 1.5 , 13);

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

void twins_tunnel(int tunnel, int n) {
	for (int iz = 0; iz < 2; iz++)
	{
		for (int ix = 0; ix < n; ix++)
		{
			if (tunnel == 2) {
				if (ix == 2 || ix == n -3)
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
				twins_door(ix,0,iz);
				continue;
			}

			row_windows(ix, 0, iz);
		}
		if (tunnel == 1) continue;

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
					if(peak) twins_peak(ix, iy, iz);
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

void Builds_twins(int tunnel = 0, int peak = 0, int club = 0 , int m = 5, int n = 11)
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

void tower(int door, int m, int n) {
	for (int iz = 0; iz < 2; iz++)
	{
		for (int iy = 0; iy < m; iy++)
		{
			for (int ix = 0; ix < n; ix++)
			{
				row_windows(ix,iy,iz);
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
					glVertex3f((ix) ? (ix*3.25*n) + 0.25 - 2 : 2, 1.5+(i*0.25), 4);
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
	glVertex3f((3.25*n) + 0.25-2, (m*1.5 + 0.5), 0);
	glVertex3f((3.25*n) + 0.25-2, (m*1.5 + 0.5), 10);
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

void Builds_tower(int door = 1, int m = 16, int n = 4) 
{
	glPushMatrix();
		tower(door, m, n);
	glPopMatrix();
}

#pragma endregion tower

#pragma region basic

void Bulidings() 
{
	glPushMatrix();
		Builds_twins(2); // Bonus
		glTranslatef(36, 0, 0);
		Builds_twins(1); //Hajduczek
		glTranslated(36, 0, -5);
		Builds_twins(0,1,1); //Omega daszek
		glTranslatef(36, 0, -50);
		glRotatef(180, 0, 1, 0);
		Builds_twins(0,1,1); // Filutek daszek
		glTranslatef(0, 0, 20);

		Builds_tower(); // Babilon
		glTranslatef(17, 0, 15);
		Builds_tower(); // Olimip
		glTranslatef(-17, 0, -35);
		glTranslated(36, 0, 0);
		Builds_twins(); //Stumyk
		glTranslated(36, 0, 5);
		Builds_twins(3); //Baratek
		glTranslatef(36, 0, 0);
		Builds_twins(0,0,1); //Zascianek
		glTranslatef(36, 0, -5);
		Builds_twins(0,1); //Stokrotka daszek
		glTranslatef(36, 0, -40);
		glRotatef(180, 0, 1, 0);
		Builds_twins(0,1,1); //Starszy dwor
		glTranslatef(36, 0, -5);
		Builds_twins(0,1); // Promyk

		glTranslatef(-30, 0, 50);
		Builds_tower(); // Kapitol
	glPopMatrix();

}

void Fundamentals()
{
	glPushMatrix();
		glColor3f(0, 1, 0);
		glScalef(10, 0.001, 10);
		glutSolidSphere(100, 100, 23);
	glPopMatrix();
}

#pragma endregion basic
 
void GenerujListyWyswietlania()
{
        idlisty = glGenLists( 4 );
 
        // utworzenie pierwszej listy
        glNewList( idlisty + 0, GL_COMPILE );
 
        glTranslatef(0,-10,30);

		Fundamentals();
		glScalef(0.5, 0.5, 0.5);
		Bulidings();


        glEndList();
 
 
 
        // utworzenie drugiej listy
        glNewList( idlisty + 1, GL_COMPILE );
 
		Fundamentals();

        glTranslatef(0,0,8);
 
        glutSolidTeapot(1);
 
        glEndList();
 
        // utworzenie trzeciej listy
        glNewList( idlisty + 2, GL_COMPILE );

		Fundamentals();
		
		
		glTranslatef(0,0,10);
		Textures();
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
 
		if (KeyLEFT) ObrotX -= 0.02*scale/5;
 
		if (KeyRIGHT) ObrotX += 0.02*scale/5;
 
        siny = sin(ObrotY);
 
        cosx = cos(ObrotX);
 
        sinx = sin(ObrotX);
 
        // nowa pozycja kamery
 
        eyex += cosx*PredkoscKamery;
 
        eyey += siny*PredkoscKamery;
 
        eyez += sinx*PredkoscKamery;
 
        // punkt wycelowania kamery powinien
 
        // znajdować się gdzieś "przed jej nosem"
 
        CelX = eyex+cosx;
 
        CelY = eyey+siny;
 
        CelZ = eyez+sinx;
 
        gluLookAt(eyex, eyey, eyez,CelX, CelY, CelZ, 0, 1, 0);
 
        //gluLookAt( eyex, eyey, eyez, centerx, centery, centerz, 0, 1, 0 );
 
        PredkoscKamery=0;
}
 
void Display()
{
        // kolor tła - zawartość bufora koloru
        glClearColor(0.2421875, 0.578125, 1, 0.5 );
 
        glEnable(GL_DEPTH_TEST);
 
        // czyszczenie bufora koloru
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 
        // wybór macierzy modelowania
        glMatrixMode( GL_MODELVIEW );
 
        // macierz modelowania = macierz jednostkowa
        glLoadIdentity();
 
        UstawKamere();
 
        // ustawienie obserwatora
 
        //glRotatef(kat,1,0,0);
 
        // przesunięcie obiektu o wektor [0,0,-3]
 
        //glTranslatef( 0, 0, 2.0 );
 
        // kolor krawędzi sześcianu
        glColor3f( 0.5, 1.0, 0.5 );
 
        glEnable(GL_LIGHTING);
        GLfloat ambientlight[]={1.0, 1.0, 1.0, 1.0};
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientlight);
        GLfloat gray[] = {0.75, 0.75, 0.75, 1.0};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
 
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
       
        glNormal3f(0.0, -1.0, 0.0);
         
        glEnable(GL_NORMALIZE);
         
        GLfloat DiffuseLight1[]={1.0,1.0,1.0,1.0};
        GLfloat LightPosition1[]={0.0, 100.0, -100.0, 1.0};
        GLfloat AmbientLight[]={0.1, 0.1, 0.1, 1.0};
         
        glLightfv( GL_LIGHT0, GL_DIFFUSE, DiffuseLight1 );
        glLightfv( GL_LIGHT0, GL_POSITION, LightPosition1 );
        glLightfv( GL_LIGHT0, GL_AMBIENT, AmbientLight );
 
        GLfloat specular[]={1.0,1.0,1.0,1.0};
 
        glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
       
        GLfloat specref[]={1.0,1.0,1.0,1.0};
        glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
        glMateriali(GL_FRONT, GL_SHININESS, 128);
         
       
 
        // glutWireCube(1);
        switch (figure)
        {
                case 1:
                        glCallList(idlisty+0);
                        break;
 
                case 2:
                        glCallList(idlisty+1);
                        break;
 
                case 3:
                        glCallList(idlisty+2);
                        break;
        }
 
        // skierowanie poleceń do wykonania
        glFlush();
 
        // zamiana buforów koloru
        glutSwapBuffers();
 
}
 
// zmiana wielkości okna
 
void Reshape( int width, int height )
{
        // obszar renderingu - całe okno
        glViewport( 0, 0, width, height );
 
        // wybór macierzy rzutowania
        glMatrixMode( GL_PROJECTION );
 
        // macierz rzutowania = macierz jednostkowa
        glLoadIdentity();
 
        // obliczenie aspektu obrazu z uwzględnieniem
        // przypadku, gdy wysokość obrazu wynosi 0
        GLdouble aspect = 1;
 
        if( height > 0 )
 
                aspect = width /( GLdouble ) height;
 
        // rzutowanie perspektywiczne
        gluPerspective( fovy, aspect, 0.01, 100.0 );
 
        // generowanie sceny 3D
        Display();
}
 
// obsługa klawiatury
 
void Klawisze( unsigned char key, int x, int y )
 
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

        // odrysowanie okna
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}
 
void KlawiszeSpecjalne( int key, int x, int y )
{
        switch( key )
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
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}
 
// obsługa menu podręcznego
 
void Menu( int value )
{
 
        switch( value )
        {
                // wyjście
        case EXIT:
 
                exit( 0 );
 
                // wyjście
        case 1:
 
                figure=1;
 
                break;
 
        case 2:
 
                figure=2;
 
                break;
 
        case 3:
 
                figure=3;
 
                break;
 
        }
 
        Reshape( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );
}
 
void KeyDown(int key,int x,int y)
{
 
        switch(key){
 
        case GLUT_KEY_UP:
 
                KeyUP=true;
 
                break;
 
        case GLUT_KEY_DOWN:
 
                KeyDOWN=true;
 
                break;
 
        case GLUT_KEY_LEFT:
 
                KeyLEFT=true;
 
 
                break;
 
        case GLUT_KEY_RIGHT:
 
                KeyRIGHT=true;
 
                break;
        }
}
 
void KeyUp(int key,int x,int y)
{
 
        switch(key)
        {
                case GLUT_KEY_UP:
                        KeyUP=false;
                        break;
 
                case GLUT_KEY_DOWN:
                        KeyDOWN=false;
                        break;
 
                case GLUT_KEY_LEFT:
                        KeyLEFT=false;
                        break;
 
                case GLUT_KEY_RIGHT:
                        KeyRIGHT=false;
                        break;
        }
}
 
int main( int argc, char * argv[] )
{

        // inicjalizacja biblioteki GLUT
        glutInit( & argc, argv );
 
        // inicjalizacja bufora ramki
        glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
 
        // rozmiary głównego okna programu
        glutInitWindowSize(GetSystemMetrics(SM_CXSCREEN)-5 ,GetSystemMetrics(SM_CYSCREEN)-5);

 
        // utworzenie głównego okna programu
#ifdef WIN32
        glutCreateWindow( "Sześcian 3" );
#else
        glutCreateWindow( "Szescian 3" );
#endif
 
        // dołączenie funkcji generującej scenę 3D
        glutDisplayFunc( Display );
 
        // dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
        glutReshapeFunc( Reshape );
 
        // dołączenie funkcji obsługi klawiatury
        glutKeyboardFunc( Klawisze );
 
        // dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
        // glutSpecialFunc( KlawiszeSpecjalne );
 
        glutSpecialFunc(KeyDown);
 
        glutSpecialUpFunc(KeyUp);
		 
        // utworzenie menu podręcznego
        glutCreateMenu( Menu );
 
        // dodanie pozycji do menu podręcznego
        glutAddMenuEntry( "Wyjście", EXIT );
        glutAddMenuEntry( "Rys1", 1 );
        glutAddMenuEntry( "Rys2", 2 );
        glutAddMenuEntry( "Rys3", 3 );
 
        // określenie przycisku myszki obsługującej menu podręczne
        glutAttachMenu( GLUT_RIGHT_BUTTON );
 
        GenerujListyWyswietlania();
 
        glutIdleFunc(Display);
        // wprowadzenie programu do obsługi pętli komunikatów
        glutMainLoop();

        return 0;
}