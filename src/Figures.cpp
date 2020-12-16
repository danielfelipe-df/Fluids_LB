#include <Figures.h>
#include <math.h>

bool Cuadrado(int x, int y, int ix, int iy, int L){
  if(x>=ix && x<=ix+L && y>=iy && y<=iy+L){return true;}
  else{return false;}
}


bool Circunferencia(int x, int y, int ix, int iy, int R){
  if((x-ix)*(x-ix) + (y-iy)*(y-iy) <= R*R){return true;}
  else{return false;}
}


bool Elipse(int x, int y, double ix, double iy, double Ra, double Rb){
  if(((x-ix)*(x-ix)/(Ra*Ra)) + ((y-iy)*(y-iy)/(Rb*Rb)) <= 1){return true;}
  else{return false;}
}


bool Ala(int x, int y, double ix, double iy,double lx, double ly){
  if(x>=ix && x<=lx+ix){
    double sup = ly*std::sin(M_PI*x/lx);
    if(y <= sup+iy && y>= iy-sup){return true;}
    else{return false;}
  }
  else{return false;}
}


bool Corazon(int x, int y, double ix, double iy, double lx, double ly){
  if(x>=ix && x<=lx+ix){
    double sup = ly*std::abs(std::sin(2*M_PI*(x-ix)/lx));
    double inf = ly*0.03*(std::abs((x-ix)-0.5*lx)-0.5*lx);
    if(y<=sup+iy && y>=inf+iy){return true;}
    else{return false;}
  }
  else{return false;}
}


bool Arbol(int x, int y, double ix, double iy, double Ax, double ly){
  /*
  if(x==ix && y==iy){return true;}
  else if(x==ix+(ly/2) && y==iy){return true;}
  else if(x==ix+ly && y==iy){return true;}
  else if(x>=ix && x<=ix+ly){
    double der = Ax*(x-ix)*abs(sin(2*M_PI*(x-ix)/ly));
    if(y>=-der+iy && y<=der+iy){return true;}
    else{return false;}
  }
  else{return false;}
  */
  if(y>=iy && y<=iy+ly){
    double der = Ax*(y-iy)*std::abs(std::sin(2*M_PI*(y-iy)/ly));
    if(x>=-der+ix && x<=der+ix){return true;}
    else{return false;}
  }
  else{return false;}
}
