#include <LB_Fluids.h>
#include <Figures.h>
#include <fstream>
#include <math.h>

LatticeBoltzmann::LatticeBoltzmann(void){
  //Cargar los pesos
  w[0]=4/9.0;
  w[1]=w[2]=w[3]=w[4]=1/9.0;
  w[5]=w[6]=w[7]=w[8]=1/36.0;
  //Cargar los vectores
  V[0][0]=0;
  V[1][0]=0;

  V[0][1]=1;  V[0][2]=0;  V[0][3]=-1; V[0][4]=0;
  V[1][1]=0;  V[1][2]=1;  V[1][3]=0;  V[1][4]=-1;

  V[0][5]=1;  V[0][6]=-1; V[0][7]=-1; V[0][8]=1;
  V[1][5]=1;  V[1][6]=1;  V[1][7]=-1; V[1][8]=-1;
}


double LatticeBoltzmann::rho(int ix,int iy,bool UseNew){
  int i; double suma;
  for(suma=0,i=0;i<Q;i++){
    if(UseNew){suma+=fnew[ix][iy][i];}
    else{suma+=f[ix][iy][i];}
  }
  return suma;
}


double LatticeBoltzmann::Jx(int ix,int iy,bool UseNew){
  int i; double suma;
  for(suma=0,i=0;i<Q;i++){
    if(UseNew){suma+=fnew[ix][iy][i]*V[0][i];}
    else{suma+=f[ix][iy][i]*V[0][i];}
  }
  return suma;
}


double LatticeBoltzmann::Jy(int ix,int iy,bool UseNew){
  int i; double suma;
  for(suma=0,i=0;i<Q;i++){
    if(UseNew){suma+=fnew[ix][iy][i]*V[1][i];}
    else{suma+=f[ix][iy][i]*V[1][i];}
  }
  return suma;
}


double LatticeBoltzmann::feq(double rho0,double Ux0,double Uy0,int i){
  double UdotVi=Ux0*V[0][i]+Uy0*V[1][i], U2=Ux0*Ux0+Uy0*Uy0;
  return rho0*w[i]*(1+3*UdotVi+4.5*UdotVi*UdotVi-1.5*U2);
}


void LatticeBoltzmann::Colisione(double Vventilador){
  int ix,iy,i; double rho0,Ux0,Uy0;

  for(ix=0; ix<3; ix++){
    for(iy=0; iy<Ly; iy++){
      for(i=0;i<Q;i++){
	fnew[ix][iy][i]=feq(Rhocontinua,Vventilador,0,i);
      }
    }
  }

  //Para cada celda
  for(ix=3;ix<250;ix++){
    for(iy=0;iy<Ly;iy++){
      //Calcular las cantidades macroscópicas
      rho0=rho(ix,iy,false);  Ux0=Jx(ix,iy,false)/rho0;  Uy0=Jy(ix,iy,false)/rho0;
      if(Elipse(ix,iy,30.0,100.0,20.0,50.0)){
      //if(Cuadrado(ix,iy,10,50,100)){
      //if(Circunferencia(ix,iy,60.0,100.0,50.0)){
      //if(Ala(ix,iy,10.0,100.0,200.0,50.0)){
      //if(Corazon(ix,iy,10,100,100,50)){
      //if(Arbol(ix,iy,20.0,100.0,1.0,100.0)){
        for(i=0;i<Q;i++){fnew[ix][iy][i]=feq(rho0,0,0,i);}
      }
      else if(ix==50 && iy==100+50+1){
        for(i=0;i<Q;i++){fnew[ix][iy][i]=feq(rho0,0,0,i);}
      }
      else{
	for(i=0;i<Q;i++){fnew[ix][iy][i]=UmUtau*f[ix][iy][i]+Utau*feq(rho0,Ux0,Uy0,i);}
      }
    }
  }

  for(ix=250;ix<Lx;ix++){
    for(iy=0;iy<Ly;iy++){
      //Calcular las cantidades macroscópicas
      rho0=rho(ix,iy,false);  Ux0=Jx(ix,iy,false)/rho0;  Uy0=Jy(ix,iy,false)/rho0;
      for(i=0;i<Q;i++){fnew[ix][iy][i]=UmUtau*f[ix][iy][i]+Utau*feq(rho0,Ux0,Uy0,i);}
    }
  }
}


void LatticeBoltzmann::Adveccione(void){
  for(int ix=0;ix<Lx;ix++){
    for(int iy=0;iy<Ly;iy++){
      for(int i=0;i<Q;i++){
        if(ix+V[0][i]<Lx && ix+V[0][i]>=0 && iy+V[1][i]<Ly && iy+V[1][i]>=0){
          f[ix+V[0][i]][iy+V[1][i]][i]=fnew[ix][iy][i];
        }
      }
    }
  }
}


void LatticeBoltzmann::Inicie(double rho0,double Ux0,double Uy0){
  for(int ix=0;ix<Lx;ix++){
    for(int iy=0;iy<Ly;iy++){
      for(int i=0;i<Q;i++){
        f[ix][iy][i]=feq(rho0,Ux0,Uy0,i);
      }
    }
  }
}


/*
void LatticeBoltzmann::ImponerCampos(double Vventilador){
  int i,ix,iy; double rho0; int ixc=60,iyc=100; int R=50, R2=R*R;

  for(ix=0;ix<250;ix++)
    for(iy=0;iy<Ly;iy++){
      rho0=rho(ix,iy,false);
      //ventilador
      if(ix==1 || ix==2)
        for(i=0;i<Q;i++)  fnew[ix][iy][i]=feq(Rhocontinua,Vventilador,0,i);
      else if(Elipse(ix,iy,30.0,100.0,20.0,50.0))
      //else if(Cuadrado(ix,iy,10,50,100))
      //else if(Circunferencia(ix,iy,ixc,iyc,R))
      //else if(Ala(ix,iy,10.0,100.0,200.0,50.0))
      //else if(Corazon(ix,iy,10,100,100,50))
        for(i=0;i<Q;i++)  fnew[ix][iy][i]=feq(rho0,0,0,i);
      else if(ix==ixc && iy==iyc+150+3)
        for(i=0;i<Q;i++)  fnew[ix][iy][i]=feq(rho0,0,0,i);
      else if(iy == 0 || iy ==Ly-1)
        for(i=0;i<Q;i++)  fnew[ix][iy][i]=feq(rho0,0,0,i);
    }
}
*/


void LatticeBoltzmann::Imprimase(const char * NombreArchivo,double Vventilador){
  std::ofstream MiArchivo(NombreArchivo);
  double rho0,Ux0,Uy0, vx, vy;
  for(int ix=0;ix<Lx;ix+=4){
    for(int iy=0;iy<Ly;iy+=4){
      rho0=rho(ix,iy,true);  Ux0=Jx(ix,iy,true)/rho0;  Uy0=Jy(ix,iy,true)/rho0;
      vx = 4*(Ux0+Vventilador)/Vventilador; vy = 4*Uy0/Vventilador;
      //MiArchivo<<ix<<" "<<iy<<" "<<4*(Ux0-Vventilador)/Vventilador<<" "<<4*Uy0/Vventilador<<endl;
      //MiArchivo << ix << " " << iy << " " << rho0 << endl;
      MiArchivo << ix << " " << iy << " " << std::sqrt(vx*vx + vy*vy) << std::endl;
    }
    MiArchivo << std::endl;
  }
  MiArchivo.close();
}
