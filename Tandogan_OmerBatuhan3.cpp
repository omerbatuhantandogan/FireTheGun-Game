/*********
   CTIS164 - Template Source Program
----------
STUDENT : Omer Batuhan Tandogan
SECTION : 1
HOMEWORK: 3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 500

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height


int gunAng = 0;
int gunPos = 0;
int gunFirePosX, gunFirePosY;                               ////////global variables needed during all execution
int lastBullet = 0;
int bossY = 0;
bool bossAction = true;
bool game = true;
int remainingTarget = 6;




typedef struct {
    int x, y;
    int firedAngle;
    bool action = false;
}bulletType;


typedef struct {
    int x, y;
    int relAngle;
    bool action = true;
}starType;

bulletType bullets[15];
starType stars[5];               //creating bundles for bullets and stars


//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


int sx(int x, int y, int ang) {
    return x * cos(ang * D2R) - y * sin(ang * D2R);
}                                                              
// rotation formula with given angle
int sy(int x, int y, int ang) {
    return x * sin(ang * D2R) + y * cos(ang * D2R);
}

//gun
void gun(int x, int y, int angle) {

    glLineWidth(4);
    glColor3f(1, 1, 1);
    circle_wire(sx(10, -5, angle)+x,sy(10, -5, angle) + y, 15);
    glLineWidth(1);

    glColor3f(0, 0, 0);
    glBegin(GL_QUAD_STRIP);
    glVertex2d(sx(53.5, 36.5, angle) + x,sy(53.5, 36.5, angle) + y);
    glVertex2d(sx(60, 36.5, angle) + x,sy(60, 36.5, angle) + y);
    glVertex2d(sx(53.5, 24.5, angle) + x,sy(53.5, 24.5, angle) + y);
    glVertex2d(sx(60, 24.5, angle) + x,sy(60, 24.5, angle) + y);
    glVertex2d(sx(60, 24.5, angle) + x,sy(60, 24.5, angle) + y);
    glVertex2d(sx(60, 0.5, angle) + x,sy(60, 0.5, angle) + y);
    glVertex2d(sx(48.5, 24.5, angle) + x,sy(48.5, 24.5, angle) + y);
    glVertex2d(sx(48.5, -0.5, angle) + x,sy(48.5, -0.5, angle) + y);
    glVertex2d(sx(0, 24, angle) + x,sy(0, 24, angle) + y);
    glColor3f(1, 1, 1);
    glVertex2d(sx(-0.5, 0.5, angle) + x,sy(-0.5, 0.5, angle) + y);
    glVertex2d(sx(-24.5, 24, angle) + x,sy(-24.5, 24, angle) + y);
    glVertex2d(sx(-25.5, 1, angle) + x,sy(-25.5, 1, angle) + y);
    glVertex2d(sx(-25.5, 1, angle) + x,sy(-25.5, 1, angle) + y);
    glVertex2d(sx(-25.5, 1, angle) + x,sy(-25.5, 1, angle) + y);
    glVertex2d(sx(-25.5, 1, angle) + x,sy(-25.5, 1, angle) + y);
    glColor3f(0, 0, 0);
    glVertex2d(sx(-0.5, 0, angle) + x,sy(-0.5, 0, angle) + y);
    glVertex2d(sx(-24.5, -45, angle) + x,sy(-24.5, -45, angle) + y);
    glVertex2d(sx(-0.5, -45, angle) + x,sy(-0.5, -45, angle) + y);
    glEnd();

    
    gunFirePosX = sx(65,10, angle)+x;
    gunFirePosY = sy(65,10, angle)+y;

    

   
    


}

//bullets
void bullet() {
    glColor3f(1, 1, 0);
    for (int i = 0; i < 15; i++) {
        if (bullets[i].action) {
            circle(bullets[i].x, bullets[i].y, 5);
        }
    }
}




//stars moving on an orbit
void star() {

    
    glColor3f(0, 1, 0);

    for (int i = 0; i < 5; i++) {
        if (stars[i].action) {
            glBegin(GL_TRIANGLES);
            glVertex2d(15+stars[i].x, 10 + stars[i].y);
            glVertex2d(-15 + stars[i].x, 10 + stars[i].y);
            glVertex2d(0 + stars[i].x, -15 + stars[i].y);


            glVertex2d(15 + stars[i].x, -5 + stars[i].y);
            glVertex2d(-15 + stars[i].x, -5 + stars[i].y);
            glVertex2d(0 + stars[i].x, 20 + stars[i].y);


            glEnd();
        }
    }
    
    
}

//final target
void boss() {
    if (bossAction) {
        glColor3f(1, 1, 1);
        circle_wire(300, bossY, 30);
        circle_wire(300, bossY, 25);
        circle_wire(300, bossY, 20);
        circle_wire(300, bossY, 15);
        circle_wire(300, bossY, 10);
        circle_wire(300, bossY, 5);
    }
}


//giving initial angles
void setupStars() {
    for (int i = 0; i <= 360; i += 72) {
        stars[i / 72].action = true;
        stars[i / 72].relAngle = i;
    }
}




//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3ub(100, 100, 100);
    glRectf(-500,-250,500,250);
    gun(-475,gunPos,gunAng);
    bullet();
    star();
    boss();

    glColor3f(0, 0, 1);
    glRectf(-500, 250, 500, 210);
    glColor3f(1, 1, 0);
    vprint(-500, 220, GLUT_BITMAP_HELVETICA_18, "Spacebar: FIRE      UpDownArrows: Vertical Move      LeftRightArrows: Rotation      F1: Pause&Restart");

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ' && game) {
        bullets[lastBullet].x = gunFirePosX;
        bullets[lastBullet].y = gunFirePosY;             //creating a bullet
        bullets[lastBullet].action = true;
        bullets[lastBullet].firedAngle = gunAng;
        lastBullet = (lastBullet + 1) % 15;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {

    
    case GLUT_KEY_UP: up = true; gunPos += 2; break;
    case GLUT_KEY_DOWN: down = true; gunPos -= 2; break;          //controlling gun
    case GLUT_KEY_LEFT: left = true; gunAng += 2; break;
    case GLUT_KEY_RIGHT: right = true; gunAng -= 2; break;
        
    case GLUT_KEY_F1: game = !game;
                                                          //pause restart
        if (remainingTarget <= 0) {
            for (int i = 0; i < 5; i++)
                stars[i].action = true;
            for (int i = 0; i < 15; i++)
                bullets[i].action = false;
            bossAction = true;
            bossY = 0;
            remainingTarget = 6;
        }
        
    break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if (game) {
        //movements and applying collision requirements
        for (int i = 0; i < 15; i++) {
            if (bullets[i].action) {
                bullets[i].x += cos(bullets[i].firedAngle * D2R) * 10;
                bullets[i].y += sin(bullets[i].firedAngle * D2R) * 10;
            }
        }


        for (int i = 0; i < 5; i++) {
            stars[i].relAngle = (stars[i].relAngle + 1) % 360;
            stars[i].x = cos(stars[i].relAngle * D2R) * 300 + -400;
            stars[i].y = sin(stars[i].relAngle * D2R) * 300;
        }




        bossY += 10;
        if (bossY > 250)
            bossY = -250;



        for (int i = 0; i < 15; i++) {
            for (int c = 0; c < 5; c++) {
                if (bullets[i].action && stars[c].action)
                    if ((bullets[i].x - stars[c].x) * (bullets[i].x - stars[c].x) + (bullets[i].y - stars[c].y) * (bullets[i].y - stars[c].y) < 500)
                    {
                        bullets[i].action = false;
                        stars[c].action = false;
                        remainingTarget--;
                    }

                if (bullets[i].action && bossAction)
                    if ((bullets[i].x - 300) * (bullets[i].x - 300) + (bullets[i].y - bossY) * (bullets[i].y - bossY) < 1000)
                    {
                        remainingTarget--;
                        bossAction = false;
                    }

            }
        }

        if (remainingTarget <= 0)
            game = false;


    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    setupStars();
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Fire the Gun - Omer Batuhan TANDOGAN - Homework 3");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
