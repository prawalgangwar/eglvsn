#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
// #include <file.h>

GLuint tex;
GLUquadric* sphere;
GLint rotateX, rotateY;

void keyboard(int key, int x, int y)
{
    switch(key) {
    case GLUT_KEY_UP:
        rotateX += 15;
        break;
    case GLUT_KEY_DOWN:
        rotateX -= 15;
        break;
    case GLUT_KEY_LEFT:
        rotateY += 15;
        break;
    case GLUT_KEY_RIGHT:
        rotateY -= 15;
        break;
    }
    glutPostRedisplay();
}



void LoadTextureRAW(unsigned char * data)
{
   GLuint texture;
   int width, height;
   // unsigned short * data;
   FILE * file;
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   file = fopen( "earthbump.bmp", "r" );
   if ( file == NULL )return;
   width = 1024;
   height = 512;
   fread( data, width * height * 3, 1, file);
   fclose( file );
   return;
}


void make_tex(void)
{
    unsigned char data[256][256][3];
    // for (int y = 0; y < 255; y++) {
    // for (int x = 0; x < 255; x++) {
        unsigned char *p = (unsigned char*) malloc( 1024 * 512 * 3 + 54);
        LoadTextureRAW(p);
        p = p + 54; 

    //     p[0] = p[1] = p[2] = (x ^ y) & 8 ? 255 : 0;
    // }
    // }
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid *) p);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
 
void draw(void)
{
    glClearColor(0.5, 0.5, 1.0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0,-5.0);
    glRotatef(rotateX, 1, 0, 0);
    glRotatef(rotateY, 0, 1, 0);
 
    glColor3f(0.0,1.0,0.0);
    gluQuadricDrawStyle(sphere, GLU_FILL);
    glBindTexture(GL_TEXTURE_2D, tex);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    gluSphere(sphere, 2.0, 32, 16);
 
    glutSwapBuffers();
}
 
void resize(int w, int h)
{
    if (!h)
    h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, 1.0*w/h, 0.1, 100.0);
}
 
void init(void)
{
    glEnable(GL_DEPTH_TEST);
    make_tex();
    sphere = gluNewQuadric();
    glEnable(GL_TEXTURE_2D);
    rotateX = 90;
    rotateY = 90;

}
 
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(640, 512);
 
    glutCreateWindow("Test");
 
    glutDisplayFunc(draw);
    glutReshapeFunc(resize);
    glutSpecialFunc(keyboard);
 
    init();
 
    glutMainLoop();
}