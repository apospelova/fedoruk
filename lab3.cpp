#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
using namespace std;

FILE *area1 = fopen("area1.txt","w");
FILE *area0 = fopen("area0.txt","w");
FILE *gnuplot = fopen("gnuplot.txt","w");
FILE *pointtest = fopen("pointtest.txt","w");
int spsize =0; 
double eta;

typedef struct Point
{
double x,y; 
} Point;
Point* studypoints;
Point w;
Point ptest;
 
void HelpMassage(void)
{
     cout<<"Create a file  Points.txt  with the training data."<<endl;
     cout<<"Data format: Xvalue Yvalue Dvalue."<<endl;
}

double Eta()
{     
       cout<<"Enter the learning coefficient in the range of (0,1):"<<endl;
       cin>>eta;
       if (eta<=0 || eta>=1)
       {
          cout<<"ERROR!"<<endl;
          exit(-1);
       }
       return eta;                  
}

void normal(Point* a)
{
     double Sqrt;
     Sqrt = sqrt((a->x)*(a->x)+(a->y)*(a->y)); 
     a->x = (a->x)/Sqrt;
     a->y = (a->y)/Sqrt;         
 }

void CreateNormalPoints(FILE *studyfile)
{    
    int i=0;
    char str[255];
    int dvalue;    
    while (fgets(str,255,studyfile))
          spsize++;          
    studypoints = new Point [spsize];   
    fseek(studyfile, 0, SEEK_SET);
     while (fgets(str,255,studyfile))
     {
           sscanf(str,"%lf\t%lf\t%d\n",&studypoints[i].x,&studypoints[i].y,&dvalue);
           normal(&studypoints[i]);           
           if (dvalue ==0)
           {
                 fprintf(area0,"%lf\t%lf\n", studypoints[i].x,studypoints[i].y);                                     
           }
           if (dvalue ==1){
           fprintf(area1,"%lf\t%lf\n", studypoints[i].x,studypoints[i].y);
           i++;  }         
     }
     spsize = i;
     //cout<<spsize<<endl;
     fclose (studyfile);
     fclose (area0); 
     fclose (area1);      
 }
 
int Test()
 {
     
     double cos_W_PointTest;
     cout<<"x="; cin >> ptest.x;
     cout<<"y="; cin >> ptest.y;
     if (ptest.x==0 && ptest.y==0)
     cout<<"Invalid value.\n"<<endl;
     else{
     normal(&ptest);
     cos_W_PointTest = w.x*ptest.x+w.y*ptest.y;
     cout<<"cos_W_PointTest="<<cos_W_PointTest<<"\n"<<endl;
     fprintf(pointtest,"%lf\t%lf\n", ptest.x,ptest.y);}     
     char answer;
     cout<<"Continue testing? (y/n)"<<endl;
     cin>> answer;
     if (answer!='y' && answer !='Y') cout<<"The End!"<<endl; 
     else Test();      
 }


int main (int argc, char** argv)
{    
    int i; 
    if (argc !=2)
    {
       HelpMassage(); 
       return -1;      
    }
    FILE * studyfile = fopen(argv[1],"r");    
    if (studyfile==NULL)
    {
       HelpMassage();	
       return -1;       
    }
    Eta();
    CreateNormalPoints(studyfile);
    fprintf(gnuplot,"set terminal windows\n"); 
    fprintf(gnuplot,"set nokey\n");  
    fprintf(gnuplot,"set xrange [-1:1]\n"); 
    fprintf(gnuplot,"set yrange [0:1]\n");  
    fprintf(gnuplot,"set xlabel 'x'\n"); 
    fprintf(gnuplot,"set ylabel 'y'\n");
    w.x = 0.0;
    w.y = 0.0;
    for (i=0;i<spsize;i++)
    {
        w.x += eta*(studypoints[i].x - w.x);
        w.y += eta*(studypoints[i].y - w.y);
        normal(&w); 
        fprintf(gnuplot,"plot 'area0.txt' with points pt 6 lc rgb \"blue\",");  
        fprintf(gnuplot,"'area1.txt' with points pt 2 lc rgb \"green\",");       
        fprintf(gnuplot,"'-' with points pt 3 ps 2 lc rgb \"red\"\n");              
        fprintf(gnuplot,"%lf\t%lf\n", studypoints[i].x,studypoints[i].y); 
        fprintf(gnuplot,"e\n"); 
        fprintf(gnuplot,"pause 0.5\n");          
        fprintf(gnuplot,"plot 'area0.txt' with points pt 6 lc rgb \"blue\",");  
        fprintf(gnuplot,"'area1.txt' with points pt 2 lc rgb \"green\",");
        fprintf(gnuplot,"'-' with linespoints lc rgb \"red\"\n"); 
        fprintf(gnuplot,"0 0\n");
        fprintf(gnuplot,"%lf\t%lf\n", w.x,w.y);
        fprintf(gnuplot,"e\n"); 
        fprintf(gnuplot,"pause 0.5\n");        
    }
    char answer;
    cout<<"Neuron is trained on the input file."<<endl;
    cout<<"Start testing? (y/n)"<<endl;
    cin>> answer;
    if (answer!='y' && answer !='Y') cout<<"The End!"<<endl;    
    else {cout<<"Begin testing!"<<endl; Test();} 
    fprintf(gnuplot,"plot 'area0.txt' with points pt 6 lc rgb \"blue\",");  
    fprintf(gnuplot,"'area1.txt' with points pt 2 lc rgb \"green\",");    
    fprintf(gnuplot,"'pointtest.txt' with points pt 3 lc rgb \"red\",");
    fprintf(gnuplot,"'-' with linespoints lc rgb \"red\"\n"); 
    fprintf(gnuplot,"0 0\n");
    fprintf(gnuplot,"%lf\t%lf\n", w.x,w.y);
    fprintf(gnuplot,"e\n"); 
    fprintf(gnuplot,"pause 0.5\n");
    fclose(pointtest); 
    fclose (gnuplot);
    int qwerty;
    cin>> qwerty;
    }
