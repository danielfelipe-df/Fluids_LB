#ifndef LB_FLUIDS_H
#define LB_FLUIDS_H

/* LB constants ****************************************************************/

const int Lx = 900;
const int Ly = 200;

const int Q = 9;

const double tau = 0.6;
const double Utau = 1.0/tau;
const double UmUtau = 1-Utau;

const double Rhoinicial = 1.0;
const double Vventilador = 0.1;
const double Rhocontinua = 1.0;


/* LB class ********************************************************************/

class LatticeBoltzmann{
private:
  double w[Q];
  int V[2][Q];
  double f[Lx][Ly][Q], fnew[Lx][Ly][Q];
public:
  LatticeBoltzmann(void);
  double rho(int ix,int iy,bool UseNew);
  double Jx(int ix,int iy,bool UseNew);
  double Jy(int ix,int iy,bool UseNew);
  double feq(double rho0,double Ux0,double Uy0,int i);
  void Colisione(double Vventilador);
  void Adveccione(void);
  void Inicie(double rho0,double Ux0,double Uy0);
  //void ImponerCampos(double Vventilador);
  void Imprimase(const char * NombreArchivo,double Vventilador);
};

/*******************************************************************************/

#endif
