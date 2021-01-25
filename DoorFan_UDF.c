#include "udf.h"
//MUST BE RUN IN SERIES NOT PARALLEL
//global variable to store average temperature
real tavgTop;
real tavgBot;
int outletIDTop = 39; //outlet ID to get information fromm
int outletIDBot = 6;

/*Function to find the average temperature at the end of each iteration for TOP*/
DEFINE_EXECUTE_AT_END(average_temp_top)
{
	Domain *d;
	face_t f;
	real temper = 0.0;
	real A[ND_ND];
	real area = 0.0;
	real area_tot = 0.0;
	Thread *t;
	tavgTop = 0;
	d = Get_Domain(1);
	t = Lookup_Thread(d,outletIDTop);
	begin_f_loop(f,t)
	{
	F_AREA(A,f,t);
		area = NV_MAG(A);//*2.0*M_PI; /*Since the model is axisymmetric, we have to multiply by 2pi*/
		area_tot += area;
		temper = F_T(f,t); 
		tavgTop += temper*area;
	}
	end_f_loop(f,t)
	tavgTop /= area_tot;
	//printf("Tavg = %g area_tot = %g\n",tavg,area_tot);
}

DEFINE_PROFILE(var_temp_top, t, i)
{
	real x[ND_ND];
	real y;
	face_t f;
	begin_f_loop(f,t)
	{
		F_CENTROID(x, f, t);
		y = x[1];
		F_PROFILE(f, t, i) = tavgTop;
	}
	end_f_loop(f,t);
}

/*Function to find the average temperature at the end of each iteration for bottom fan*/
DEFINE_EXECUTE_AT_END(average_temp_bot)
{
	Domain *d;
	face_t f;
	real temper = 0.0;
	real A[ND_ND];
	real area = 0.0;
	real area_tot = 0.0;
	Thread *t;
	d = Get_Domain(1);
	t = Lookup_Thread(d,outletIDBot);
	tavgBot = 0;
	begin_f_loop(f,t)
	{
		F_AREA(A,f,t);
		area = NV_MAG(A);//*2.0*M_PI; /*Since the model is axisymmetric, we have to multiply by 2pi*/
		area_tot += area;
		temper = F_T(f,t); 
		tavgBot += temper*area;
	}
	end_f_loop(f,t)
	tavgBot /= area_tot;
	//printf("Tavg = %g area_tot = %g\n",tavg,area_tot);
}

DEFINE_PROFILE(var_temp_bot, t, i)
{
	real x[ND_ND];
	real y;
	face_t f;
	begin_f_loop(f,t)
	{
		F_CENTROID(x, f, t);
		y = x[1];
		F_PROFILE(f, t, i) = tavgBot;
	}
	end_f_loop(f,t);
}