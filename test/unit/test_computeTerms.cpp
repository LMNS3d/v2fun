#include<iostream>
#include"../../src/computeTerms.h"
#include<math.h>
using namespace std;

int Setuptest(gsl_vector * xi, struct constants * modelConst)
{
	modelConst->reyn=2;
	modelConst->Cmu=3; 
	modelConst->C1=1; 
	modelConst->C2=2; 
	modelConst->Cep1=4; 
	modelConst->Cep2=5; 
	modelConst->Ceta=6; 
	modelConst->CL=7; 
	modelConst->sigmaEp=8;
	
	gsl_vector_set(xi,0,1);
	gsl_vector_set(xi,1,3);
	gsl_vector_set(xi,2,8);
	gsl_vector_set(xi,3,7);
	gsl_vector_set(xi,4,9);
	gsl_vector_set(xi,5,2);
	gsl_vector_set(xi,6,8);
	gsl_vector_set(xi,7,2);
	gsl_vector_set(xi,8,8);
	gsl_vector_set(xi,9,10);

	return 0; 

}

int ComputeT_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);
	gsl_vector * T = gsl_vector_alloc(2); 
	gsl_vector_set(T,0,1.5);
	gsl_vector_set(T,1,4); 

	for(unsigned int i=0; i<T->size;i++)
	{
		if(ComputeT(xi,modelConst,i+1)!=gsl_vector_get(T,i))
		{
			cout << "FAIL: Compute Turbulent Time Scale, T" << endl; 
			return 1; 
		}
	}
	cout << "PASS: Compute Turbulent Time Scale, T" << endl; 
	gsl_vector_free(T);
	gsl_vector_free(xi);
	return 0; 
}

int ComputeL_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);

	gsl_vector * L = gsl_vector_alloc(2); 
	gsl_vector_set(L,1,79.195959492);
	gsl_vector_set(L,0,14.849242404);
	
	for(unsigned int i=0; i<L->size;i++)
	{
		if( fabs(ComputeL(xi,modelConst,i+1)-gsl_vector_get(L,i))>0.0000001)
		{
			cout << "FAIL: Compute Turbulent Length Scale, L" << endl; 
			return 1; 
		}
	}
	cout << "PASS: Compute Turbulent Length Scale, L" << endl; 
	gsl_vector_free(L);
	gsl_vector_free(xi);
	return 0; 
}

int ComputeEddyVisc_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);

	gsl_vector * vT = gsl_vector_calloc(3); 
	gsl_vector * T  = gsl_vector_calloc(3);

	gsl_vector_set(vT,1,31.5);
	gsl_vector_set(vT,2,96);
	
	for(unsigned int i=1; i<vT->size;i++)
	{
		gsl_vector_set(T,i,ComputeT(xi,modelConst,i));
		if(ComputeEddyVisc(xi,T,modelConst,i)!=gsl_vector_get(vT,i))
		{
			cout << "FAIL: Compute Eddy Viscosity, vT" << endl; 
			return 1; 
		}
	}
	cout << "PASS: Compute Eddy Viscosity, vT" << endl; 
	return 0; 
}

int ComputeP_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);
	double deltaEta = 0.5;
	Grid grid(true, 1.0, deltaEta);

	//we don't need P at vall and know = 0 at delta so just test middle point
	gsl_vector * P = gsl_vector_calloc(1); 
	gsl_vector * vT = gsl_vector_calloc(2);
	gsl_vector * T  = gsl_vector_calloc(2);
	gsl_vector_set(P,0,126);


	gsl_vector_set(T,1,ComputeT(xi,modelConst,1));
	gsl_vector_set(vT,1,ComputeEddyVisc(xi,T,modelConst,1));

	if(ComputeP(xi,vT,&grid,1)!=gsl_vector_get(P,0))
	{
		cout << "FAIL: Compute Production Rate,P" << endl; 
		return 1; 
	}
	cout << "PASS: Compute Production Rate, P" << endl; 
	gsl_vector_free(P);
	gsl_vector_free(xi);
	return 0; 
}

int ComputeEp0_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);
	double deltaEta = 0.5; 
  Grid grid(true, 1.0, deltaEta);

	if(ComputeEp0(xi,modelConst,&grid)!=12)
	{
		cout << "FAIL: Compute Dissipation at Wall" << endl; 
		return 1; 
	}
	cout <<"PASS: Compute Dissipation at Wall" << endl; 
	return 0; 
}

int Computef0_test()
{
	gsl_vector * xi = gsl_vector_alloc(10); 
	struct constants Const = {
		.reyn=0,.Cmu=0,.C1=0,.C2=0,.Cep1=0,.Cep2=0,.Ceta=0,.CL=0,.sigmaEp=0};
	constants * modelConst= & Const;  
	Setuptest(xi,modelConst);
	double deltaEta = 0.5;
  	Grid grid(true, 1.0, deltaEta);

	if(fabs(Computef0(xi,modelConst,&grid)+46.6666667)>0.0000001)
	{
		cout << Computef0(xi,modelConst,&grid) << endl;
		cout << "FAIL: Compute redistribution at Wall" << endl; 
		return 1; 
	}
	cout <<"PASS: Compute redistribution at Wall" << endl; 
	return 0; 
}
/*
int DeConstructXi_test()
{
	gsl_vector * xi = gsl_vector_calloc(10);
	gsl_vector * U  = gsl_vector_calloc(3);
	gsl_vector * k  = gsl_vector_calloc(3);
	gsl_vector * ep = gsl_vector_calloc(3);
	gsl_vector * v2 = gsl_vector_calloc(3);
	gsl_vector * f  = gsl_vector_calloc (3);
	
	gsl_vector *trueU = gsl_vector_calloc(3); 
	gsl_vector *truek = gsl_vector_calloc(3);
	gsl_vector *trueep = gsl_vector_calloc(3);
	gsl_vector *truev2 = gsl_vector_calloc(3);
	gsl_vector *truef = gsl_vector_calloc(3);


	gsl_vector_set(xi,0,1);
	gsl_vector_set(xi,1,-1);
	gsl_vector_set(xi,2,1);
	gsl_vector_set(xi,3,0);
	gsl_vector_set(xi,4,5);
	gsl_vector_set(xi,5,2);
	gsl_vector_set(xi,6,-2);
	gsl_vector_set(xi,7,1);
	gsl_vector_set(xi,8,0);
	gsl_vector_set(xi,9,6);
	
	gsl_vector_set(trueU,1,1);
	gsl_vector_set(trueU,2,2);

	gsl_vector_set(truek,1,-1);
	gsl_vector_set(truek,2,-2);

	gsl_vector_set(trueep,0,0);
	gsl_vector_set(trueep,1,1);
	gsl_vector_set(trueep,2,1);
	
	gsl_vector_set(truef,0,0);
	gsl_vector_set(truef,1,5);
	gsl_vector_set(truef,2,6);

	if(DeconstructXi(xi,U,k,ep,v2,f))
		return 1;
	if(!gsl_vector_equal(U,trueU))
		return 1; 
	if(!gsl_vector_equal(k,truek))
		return 1;
	if(!gsl_vector_equal(ep,trueep))
		return 1; 
	if(!gsl_vector_equal(v2,truev2))
		return 1;
	if(!gsl_vector_equal(f,truef))
		return 1; 
	return 0; 
}

int ReConstructXi_test()
{

	gsl_vector * xi = gsl_vector_calloc(10);
	gsl_vector * U  = gsl_vector_calloc(3);
	gsl_vector * k  = gsl_vector_calloc(3);
	gsl_vector * ep = gsl_vector_calloc(3);
	gsl_vector * v2 = gsl_vector_calloc(3);
	gsl_vector * f  = gsl_vector_calloc (3);
	
	gsl_vector * truexi = gsl_vector_calloc(10);
	gsl_vector_set(truexi,0,1);
	gsl_vector_set(truexi,1,-1);
	gsl_vector_set(truexi,2,1);
	gsl_vector_set(truexi,3,0);
	gsl_vector_set(truexi,4,5);
	gsl_vector_set(truexi,5,2);
	gsl_vector_set(truexi,6,-2);
	gsl_vector_set(truexi,7,1);
	gsl_vector_set(truexi,8,0);
	gsl_vector_set(truexi,9,6);
	

	gsl_vector_set(U,1,1);
	gsl_vector_set(U,2,2);

	gsl_vector_set(ep,0,0);
	gsl_vector_set(ep,1,1);
	gsl_vector_set(ep,2,1);
	
	
	gsl_vector_set(k,1,-1);
	gsl_vector_set(k,2,-2);
	
	gsl_vector_set(f,0,0);
	gsl_vector_set(f,1,5);
	gsl_vector_set(f,2,6);
	
	if(ReconstructXi(xi,U,k,ep,v2,f))
		return 1; 
	if(!gsl_vector_equal(xi,truexi))
		return 1; 
	return 0; 

}
*/
