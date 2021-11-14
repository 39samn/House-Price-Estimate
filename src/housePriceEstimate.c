#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//transposing a matrix
void transpose(double** res, double** original, int row, int col){
   for(int i=0; i<row; ++i){
      for(int j=0; j<col; ++j){
         res[j][i] = original[i][j];
      }
   }
}

//multiplying two matrices
void multiply(double** res, double** x, double** y, int row1, int col1, int row2, int col2){
   for (int i = 0; i < row1; ++i) {
      for (int j = 0; j < col2; ++j) {
         res[i][j] = 0;
      }
   }
   for (int i = 0; i < row1; ++i) {
      for (int j = 0; j < col2; ++j) {
         for (int k = 0; k < col1; ++k) {
            res[i][j] += x[i][k] * y[k][j];
         }
      }
   }
}

//inverting a square matrix
void invert(double** res, double** toInvert, int rowCol){
   //making identity matrix
   for(int r=0; r<rowCol; r++ ){
      for(int c=0; c<rowCol; c++){
         if(r==c){
            res[r][c]=1;
         }
         else{
            res[r][c]=0;
         }
      }
   }
   for(int p=0; p<rowCol; ++p){
      double f = toInvert[p][p];
      for(int p2=0; p2<rowCol; ++p2){
         toInvert[p][p2]/=f;
         res[p][p2]/=f; 
      }
      for(int i =p+1; i<rowCol; ++i){
         double f2 = toInvert[i][p];
         for(int i2=0; i2<rowCol; ++i2){
            toInvert[i][i2]-=(toInvert[p][i2]*f2);
            res[i][i2]-=(res[p][i2]*f2);
         } 
      }
   } 
   for(int p = rowCol-1; p>=0; --p){
      for(int i= p-1; i>=0; --i){
         double f3 = toInvert[i][p];
         for(int i2=0; i2<rowCol; i2++){
            toInvert[i][i2]-=(toInvert[p][i2]*f3);
            res[i][i2]-=(res[p][i2]*f3);
         }   
      }
   }
}

//for printing the house price or test matrices
void print(double** mat, int row, int col){
   for(int i=0; i<row; ++i){
      for(int j=0; j<col; ++j){
         //printf("%.0f", mat[i][j]);
        printf("$%lf ", mat[i][j]);
      }
      printf("\n");
   }
}

//A method to free memory
void freeMat(double** toFree, int row){
   for(int i=0; i<row; ++i){
      free(toFree[i]);
   }
   free(toFree);
}

//A method to dynamically allocation memory for matrices
double** makeMat(int row, int col){
   double** toMake = (double**) malloc(row *sizeof(double*));
   for(int j=0; j<row; j++){
          toMake[j]= (double*) malloc(col*sizeof(double));
   }
   return toMake;
}

int main(int argc, char **argv) {
   FILE *fp = fopen(argv[1], "r");
   int houseR, attrC; 
   char fileName[6];
   fscanf(fp, "%5s", fileName);
   if(strcmp(fileName,"train")==0){
      fscanf(fp, "%d", &attrC); //maybe this
      fscanf(fp, "%d", &houseR); 
   }
   attrC++; //for adding constant 1 at the front for X and X'

   //dynamically allocated for 2D array X and Y
   double **X = makeMat(houseR, attrC);
   double **Y = makeMat(houseR, 1);
   //reading values into array X and Y
   for(int i=0; i<houseR; ++i){
      X[i][0]=1.0;
      for(int j=1; j<attrC; ++j){
            fscanf(fp, "%lf", &X[i][j]);
      }
       fscanf(fp, "%lf", &Y[i][0]);
   }
   //opening another file setting up for X'
   FILE *fp2 = fopen(argv[2], "r");
   int houseNR, attrNC; 
   char fileName2[5];
   fscanf(fp2, "%4s", fileName2);
   if(strcmp(fileName2,"data")==0){
      fscanf(fp2, "%d", &attrNC); 
      fscanf(fp2, "%d", &houseNR); 
   }
   attrNC++;//adding the constant 1
   double **XP = makeMat(houseNR, attrNC);
   //get value for XP
   for(int i=0; i<houseNR; ++i){
      XP[i][0]=1.0;
      for(int j=1; j<attrNC; ++j){
            fscanf(fp2, "%lf", &XP[i][j]);
      }
   }

   //(X^T*X)^-1 * X^T * Y 
   //get the value of X transpose to XT
   double **XT = makeMat(attrC, houseR);
   transpose(XT,X,houseR,attrC);
   //result of (X^T*X)
   double **W1 = makeMat(attrC, attrC);
   multiply(W1, XT, X, attrC, houseR, houseR, attrC); 
   //result of (X^T*X)^-1
   double **W2 = makeMat(attrC, attrC);
   invert(W2, W1, attrC);
   //result of (X^T*X)^-1 * X^T
   double **W3 = makeMat(attrC, houseR);
   multiply(W3, W2, XT, attrC,attrC,attrC, houseR);
   //result of (X^T*X)^-1 * X^T * Y
   double **W = makeMat(attrC, 1);
   multiply(W, W3, Y, attrC, houseR, houseR, 1);
   //Y' = X'* W
   double **YP = makeMat(houseNR, 1);
   multiply(YP, XP, W, houseNR, attrNC, attrC, 1);

   /*
   //for testing each calulation of each matrices
   for printing each steps
   print(X, houseR, attrC);
   print(XT, attrC, houseR);
   print(Y, houseR, 1);
   print(XP, houseNR, attrNC);
   print(W1, attrC, attrC);
   print(W2,attrC, attrC);
   print(W3, attrC, houseR);
   print(W ,attrC, 1);
   */

   //print the estimated house prices
   print(YP ,houseNR, 1);

   //freeing memory
   freeMat(X, houseR);
   freeMat(Y, houseR);
   freeMat(XP, houseNR);
   freeMat(XT, attrC);
   freeMat(W1, attrC);
   freeMat(W2, attrC);
   freeMat(W3, attrC);
   freeMat(W, attrC);
   freeMat(YP, houseNR);

   fclose(fp);
   fclose(fp2);
   return 0;
}