#define GLUT_DISABLE_ATEXIT_HACK

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <GL/glut.h>
#include <string>
#include <complex>

#define WINDOW_X 600
#define WINDOW_Y 600
#define INIT_X_DEFAULT -0.8f
#define INIT_Y_DEFAULT -0.9f
#define FINAL_X_DEFAULT 0.1f
#define FINAL_Y_DEFAULT 0.0f
#define PRECISION_DEFAULT 0.001f
#define ITR_DEFAULT 100

using namespace std;

typedef complex<float> Complex;

Complex _z_int;
vector<Complex> _list;
vector<short> _colors;

float INIT_X = INIT_X_DEFAULT, 
      INIT_Y = INIT_Y_DEFAULT, 
      FINAL_X = FINAL_X_DEFAULT, 
      FINAL_Y = FINAL_Y_DEFAULT, 
      PRECISION = PRECISION_DEFAULT; 
int ITR = ITR_DEFAULT;
float _slope_x, _slope_y , _intercept_x, _intercept_y;
bool _contrast_map = false;

void reshapeFunc(int w, int h) 
{
    if (h == 0)
            h = 1;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ((float)w)/((float)h), 1, 200);

    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
            0.0, 0.0,0.0,
            0.0, 0.0, 0.0);    
}

void displayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    gluOrtho2D(-WINDOW_X/2, WINDOW_X/2, -WINDOW_Y/2, WINDOW_Y/2);
    
    for (int i = 0; i < _list.size(); i++)
    {
        if (_contrast_map) 
            glColor3f(_colors[i] / (float)ITR, 
                      _colors[i] / (float)ITR,
                      _colors[i] / (float)ITR);
        else 
            glColor3f(1.0f, 1.0f, 1.0f); 
        
        glBegin(GL_POINTS);
            glVertex2f(real(_list[i]) * _slope_x + _intercept_x, 
                       imag(_list[i]) * _slope_y + _intercept_y);
        glEnd();
    } 
                           
    glFlush();
    glutSwapBuffers();
}

string input(string prompt)
{
    cout << prompt;
    string in;
    getline(cin, in);
    return in;
}

void initRendering() 
{
    glDisable(GLUT_DEPTH);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    
    string in;
    
    cout << "Mandelbrot Set Generator" << endl;
    
    in = input("Enter Initial x-value (Hit Enter for default value): ");
 
    if (in.length()) 
        INIT_X = atof(in.c_str());
    
    in = input("Enter Initial y-value (Hit Enter for default value): ");
    
    if (in.length()) 
        INIT_Y = atof(in.c_str());
    
    in = input("Enter Width to be Rendered (Hit Enter for default value): ");
    
    if (in.length()) 
    {
        FINAL_X = INIT_X + atof(in.c_str());
        FINAL_Y = INIT_Y + atof(in.c_str());
    }
    
    in = input("Enter Precision (Hit Enter for default value): ");
    
    if (in.length()) 
        PRECISION = atof(in.c_str());
    
    in = input("Enter Number of Iterations (Hit Enter for default value): ");
    
    if (in.length()) 
        ITR = atoi(in.c_str());
    
    in = input("Render with Contrast Map? (Y/N) (Hit Enter for default value): ");
    
    if (in.length() && !in.compare("Y")) 
        _contrast_map = true;
    
    real(_z_int) = INIT_X;
    imag(_z_int) = INIT_Y;
    
    _slope_x = WINDOW_X / (FINAL_X - INIT_X);
    _slope_y = WINDOW_Y / (FINAL_Y - INIT_Y);
    
    _intercept_x = -WINDOW_X / 2 - (_slope_x) * INIT_X;
    _intercept_y = -WINDOW_Y / 2 - (_slope_y) * INIT_Y;
}

void update(int value) 
{   
    if (imag(_z_int) > FINAL_Y)
        return;

    Complex _z_tmp = _z_int;
    
    for (int i = 0; i < ITR; i++)
    {
        _z_tmp *= _z_tmp;
        _z_tmp += _z_int;
        if (abs(_z_tmp) >= 2.0f)
        {
             _colors.push_back(i+1);
             _list.push_back(_z_int);
             break;
        }
    }

    if (real(_z_int) < FINAL_X)
        real(_z_int) += PRECISION;
    else
    {
        real(_z_int) = INIT_X;
        imag(_z_int) += PRECISION;
    }

    glutPostRedisplay();
    glutTimerFunc(1, update, 0);    
}

int main( int argc, char** argv ) 
{ 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_X,WINDOW_Y);

    glutCreateWindow("Mandelbrot");
    initRendering();

    glutDisplayFunc(displayFunc);
    glutReshapeFunc(reshapeFunc);

    glutTimerFunc(25, update, 0);
    glutMainLoop();
   
    return 0; 
}
