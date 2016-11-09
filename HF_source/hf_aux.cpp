#include <iostream>
#include <fstream>
#include <iomanip>
//#include <cstdio>
#include "hf_aux.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <mkl.h>
#include <iomanip>
//////Here are the functions required for HF


typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Matrix;
typedef std::vector<vector<double> > Real_Matrix;
typedef std::vector<vector<vector<vector<double> > > > Real_4dMatrix;

void read_nuc_en(double nuc_en)
{
   std::ifstream nuc_ener("enuc.dat");
   while(!nuc_ener.eof()){
      nuc_ener >> nuc_en;
   }


   return;

}


void read_T(int ao, Matrix& T_int){
   
   double val;
   int i;
   int j;

   std::ifstream kin_en;
   kin_en.open("T.dat");
   for(int k=0;k<((ao)*(ao+1))/2;k++){
      kin_en >> i;
      kin_en >> j;
      kin_en >> val;
      T_int(i-1,j-1) = val;
      T_int(j-1,i-1) = T_int(i-1,j-1);
      std::cout << i << " " << j << " " << T_int(i-1,j-1) << std::endl;
   }
   return;
}

void read_S(int ao, Matrix& S){

   double val;
   int i;
   int j;

   std::ifstream overlap;
   overlap.open("overlap.dat");
   for(int k=0;k<((ao)*(ao+1))/2;k++){
      overlap >> i;
      overlap >> j;
      overlap >> val;
      S(i-1,j-1) = val;
      S(j-1,i-1) = S(i-1,j-1);
      std::cout << i << " " << j << " " << S(i-1,j-1) << std::endl;
   }
   return;
}

void read_v_nuc(int ao, Matrix& v_nuc){

   double val;
   int i;
   int j;


   std::ifstream interaction;
   interaction.open("v_nuc.dat");
   for(int k=0;k<((ao)*(ao+1))/2;k++){
      interaction >> i;
      interaction >> j;
      interaction >> val;
      v_nuc(i-1,j-1) = val;
      v_nuc(j-1,i-1) = v_nuc(i-1,j-1);
      std::cout << i << " " << j << " " << v_nuc(i-1,j-1) << std::endl;
   }
   return;

}


void build_H_core(int ao, Matrix& v_nuc, Matrix& T_int, Matrix& H_core){

   int i;
   int j;


   for(int i=0; i < ao ; i++){
      for(int j=0; j < ao ; j++){
         H_core(j,i) = T_int(j,i) + v_nuc(j,i);
         std::cout << i+1 << " " << j+1 << " " << H_core(j,i) << std::endl;
         //std::cout << T_int[j][i] << "+" << v_nuc[j][i] << "=" << H_core[j][i] << std::endl;
      }
   }
   return;

}

void read_v_int(int ao, Real_4dMatrix& v_int){

   double val;
   int i;
   int j;
   int k;
   int l;
   int z;
  

   std::ifstream interaction;
   interaction.open("v_int.dat");
   while(!interaction.eof()){      
      interaction >> i;
      interaction >> j;
      interaction >> k;
      interaction >> l;
      interaction >> val;
      v_int[i-1][j-1][k-1][l-1] = val;
      std::cout << i << " " << j << " " << k << " " << l << " " << v_int[i-1][j-1][k-1][l-1] << std::endl;
   }

   for (int mu=0; mu<ao;mu++){
      for (int nu=0; nu<mu+1;nu++){
          for (int lam=0; lam<ao;lam++){
             for(int sig=0; sig<lam+1;sig++){
                        v_int[nu][mu][lam][sig]=v_int[mu][nu][lam][sig];
                        v_int[mu][nu][sig][lam]=v_int[mu][nu][lam][sig];
                        v_int[nu][mu][sig][lam]=v_int[mu][nu][lam][sig];
                        v_int[nu][mu][lam][sig]=v_int[mu][nu][lam][sig];
                        v_int[lam][sig][mu][nu]=v_int[mu][nu][lam][sig];
                        v_int[sig][lam][mu][nu]=v_int[mu][nu][lam][sig];
                        v_int[lam][sig][nu][mu]=v_int[mu][nu][lam][sig];
                        v_int[sig][lam][nu][mu]=v_int[mu][nu][lam][sig];
       


             }
         }
      }
   }

   return;
};

void calculate_S12(int ao, Matrix& S, Matrix& S12, Matrix& Xmat){

   Eigen::SelfAdjointEigenSolver<Matrix> solver(S);
   Matrix evecs = solver.eigenvectors();
   Matrix evals = solver.eigenvalues();

   std::cout << "eigenvectors are: " << evecs << std::endl;
   std::cout << "eigenvalues are: " << evals << std::endl;

   for (int i=0; i < ao ; i++){
      //for (int j=0; j < ao ; j++){
          std::cout << evals(i) << std::endl;
          S12(i,i)=1/sqrt(evals(i));
      //}
   }
   std::cout << "Square root of eigenvalues are: " << S12 << std::endl;
   
   Matrix evecs_trans = evecs.transpose();
   Matrix Temp = S12*evecs_trans;
   Xmat = evecs*Temp;

   std::cout << "Xmat is: " << Xmat << endl;


   return;
};


void diagonalize_Fock(int ao, Matrix& H_core, Matrix& Xmat, Matrix& Fock, Matrix& C_ao){

   Fock=Xmat.transpose()*H_core*Xmat;
      
   Eigen::SelfAdjointEigenSolver<Matrix> solver(Fock);
   Matrix evecs = solver.eigenvectors();
   Matrix evals = solver.eigenvalues();




   return;

};

