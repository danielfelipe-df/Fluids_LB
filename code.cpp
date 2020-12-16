#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <LB_Fluids.h>
#include <Figures.h>

/*
  La animación que se ve bien pero es lenta tiene:
  tau =0.505;  Lx=900; Ly=200; iyc=100;  R=5;
  Rhoinicial=1.0; Vventilador=0.1;

*/

double rho1[Lx][Ly], Jx1[Lx][Ly], Jy1[Lx][Ly];
LatticeBoltzmann Aire;
int t=0;


//---------- Funciones Generales --------

void RenderString(float x, float y, void *font, const unsigned char* string);

void display(void);

void init(void);

void AmplitudDisplay(void);

void mouse(int button, int state, int x, int y);



int main(int argc, char** argv)
{
  //OpenGL

  Aire.Inicie(Rhoinicial,Vventilador,0);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(900, 200);
  glutInitWindowPosition(500, 100);
  glutCreateWindow("Ala");
  init();
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  //const unsigned char * strings = reinterpret_cast<const unsigned char *>( "123" );
  //RenderString(600.0f, 100.0f, GLUT_BITMAP_TIMES_ROMAN_24, strings);
  glutMainLoop();
  //std::cout << "Elipse: " << t << std::endl;

  //Gnuplot
  /*
  int t,tmax=2e4;
  double RHOinicial=1.0, Vventilador=0.1;

  Aire.Inicie(RHOinicial,Vventilador,0);
  std::cout << "set pm3d map" << std::endl;
  //std::cout << "set term gif animate" << std::endl;
  //std::cout << "set output 'fluido_2.gif'" << std::endl;
  std::cout << "set parametric; set urange[0:2*pi]; set vrange[0:2*pi]" << std::endl;
  std::cout << "set xrange[0:300]; set yrange[0:70]" << std::endl;

  for(t=0;t<tmax;t++){
    Aire.Colisione(Vventilador);
    //Aire.ImponerCampos(Vventilador);
    Aire.Adveccione();
    Aire.Imprimase("Aire.dat",Vventilador);
    //std::cout << "plot 'Aire.dat' w vec, 12*cos(t)+32,12*sin(t)+32" << std::endl;
    std::cout << "splot 'Aire.dat', 12*cos(u)+32,12*sin(v)+32,0" << std::endl;
  }

  Aire.Imprimase("Aire.dat",Vventilador);
  */
  return 0;
}


void RenderString(float x, float y, void *font, const unsigned char* string){
  //char *c;

  //glColor3f(rgb.r, rgb.g, rgb.b);
  glRasterPos2f(x, y);
  glutBitmapLength(font, string);
}


void display(void){
  glClear(GL_COLOR_BUFFER_BIT);

  double rho0, Jx0, Jy0, Ux0, Uy0, normalization=3.0;
  double x,y,r;
  double red0 = 160.0/256.0, green0 = 32.0/256.0, blue0 = 240.0/256.0;
  //double red1 = 256.0/256.0, green1 = 256.0/256.0, blue1 = 0.0;
  Aire.Colisione(Vventilador);
  //Aire.ImponerCampos(Vventilador);
  Aire.Adveccione();
  glPointSize(1.0);
  glBegin(GL_POINTS);
  for(int ix=0; ix<Lx; ix++){
    for(int iy=0; iy<Ly; iy++){
      rho0 = rho1[ix][iy];  Jx0 = Jx1[ix][iy];  Jy0 = Jy1[ix][iy];
      Uy0 = Jy0/rho0;      Ux0 = Jx0/rho0;
      x = (Ux0-Vventilador)/Vventilador;  y=Uy0/Vventilador;
      r=sqrt(x*x+y*y)/normalization;
      if(r>1.0){glColor3f(red0,green0,blue0);}
      else{glColor3f(red0*r,green0*r,blue0*r);}//red0*r,green0*r,blue0*r para una combinación linda
      glVertex3f(ix*0.0011,iy*0.005,0);
    }
  }
  glEnd();
  //std::cout << t << std::endl;
  glFlush();
}


void init(void){
  /* select clearing (background) color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  */
  /*initialize viewing values */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}


void AmplitudDisplay(void){
  for(int ix=0; ix<Lx; ix++){
    for(int iy=0; iy<Ly; iy++){
      rho1[ix][iy] = Aire.rho(ix,iy,true);
      Jx1[ix][iy] = Aire.Jx(ix,iy,true);
      Jy1[ix][iy] = Aire.Jy(ix,iy,true);
    }
  }
  t++;
  glutPostRedisplay();
}


void mouse(int button, int state, int x, int y){
  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN)
      glutIdleFunc(AmplitudDisplay);
    break;
  case GLUT_MIDDLE_BUTTON:
    if (state == GLUT_DOWN)
      glutIdleFunc(NULL);
    break;
  default:
    break;
  }
}
