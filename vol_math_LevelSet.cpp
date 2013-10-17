#include "vol_math_LevelSet.h"
#include <iostream>
#define pi 3.14
#include<math.h> ;
using namespace std;
PIXTYPE  *p;
PIXTYPE	*q;
int i,j;
/*
LevelSet::LevelSet(void)
{

}
*/


LevelSet::~LevelSet(void)
{
	cout<<"Destructor called"<<endl;


}
//*ImageF Dirac(ImageF x,float sigma);*/
Raw2D& regFunction(ImageF &s,int m,int n);
ImageF& operator+(ImageF &x,ImageF &y);
ImageF& operator+(ImageF &x,float s);
ImageF& cos(ImageF &x);
ImageF& operator *(int p1,ImageF &x);
ImageF& operator * (ImageF &x,ImageF &y);
ImageF* operator * (ImageF &x,ImageF *y);
ImageF& operator / (ImageF &x,float);
ImageF& sin(ImageF &s);
ImageF* Dirac(ImageF *x,float sigma)
{
	ImageF *ret=new ImageF(x);
	Raw2D *f=new Raw2D((sigma)*(cos(pi*((*ret/sigma)+1))));
	//Raw2D *f=new Raw2D(x);
	ImageF b=new Raw2D(regFunction(*ret,(int)-sigma,(int)sigma));
	//*f=*f*b;

	ret=f;
	return ret ;

}

ImageF* gradientx(ImageF *g)
{
	int n=g->getXsize();
	int m=g->getYsize();
	Raw2D* ret=new Raw2D(g);
	int i,j;
	//PIXTYPE *gg;//=g->gety();
	PIXTYPE *p;//=ret->gety();
	for(int i=0;i<n-1;i++)
	{
		for(j=0;j<m-1;j++)
		{
			//*(p+i)=*(p+i-1)-*(p+i);
			//*(g.gety()+i)=*(g.gety()+i-1)-*(g.gety()+i);
			//*(p+j+i*m)=*(gg+j-1+i*m)-*(gg+j+i*m);
			if (g->get(i+1,j)-g->get(i,j)>0)
			{
				ret->put(i,j,(g->get(i+1,j)-g->get(i,j)));
			}
			else if (g->get(i+1,j)-g->get(i,j)<0)
			{
				ret->put(i+1,j,(-g->get(i+1,j)+g->get(i,j)));
			}
			//ret->put(i,j,ret->get(i,j));
		}
	}
	return ret;
}

ImageF* gradienty(ImageF* g )
{
	int n=g->getXsize();
	int m=g->getYsize();
	ImageF* ret=new ImageF(g);
	int i,j;
	//PIXTYPE *gg=(*g).gety();
	//PIXTYPE *p=(*ret).gety();
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<m-1;j++)
		{
			if (g->get(i,j+1)-g->get(i,j)>0)
			{
				ret->put(i,j,(g->get(i,j+1)-g->get(i,j)));
			}
			else if (g->get(i,j+1)-g->get(i,j)<0)
			{
				ret->put(i,j+1,(-g->get(i,j+1)+g->get(i,j)));
			}
		
			

		}
	}
	return ret;

}
//ImageF& operator &(ImageF &x,ImageF &y)
//{
//	int i,j,n,m;
//	ImageF *ret=new ImageF(x);
//	n=x.getXsize();
//	m=x.getYsize();
//	for(i=0;i<m;i++)
//	{
//		for(j=0;j<n;j++)
//		{
//			//*(x.gety()+i*m+j)=*(x.gety()+i*m+j)&(*(y.gety()+i*m+j));
//			ret->put(i,j,x.get(i,j)&y.get(i,j));
//		}
//
//	}
//	return *ret;
//
//}
ImageF& cos(ImageF &xdata)
{
	ImageF *x=new ImageF(xdata);
	int m=x->getXsize();
	int n=x->getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			x->put(i,j,cos(float(x->get(i,j))));
		}

	}
	xdata=*x;
	return xdata;
}

ImageF* NeumannBoundCond(ImageF *img)
{
	int nrow=img->getXsize();
	int ncol=img->getYsize();
	int i,j;
	//PIXTYPE *p=img->gety();
	//g([1 nrow],[1 ncol]) = g([3 nrow-2],[3 ncol-2]);  
	//p[0]=p[3*ncol+2];
	img->putXY(0,img->getXY(2*ncol+2));
	//p[ncol-1]=p[3*ncol+ncol-3];
	img->putXY(ncol-1,img->getXY(2*ncol+ncol-3));
	//p[nrow*(ncol-1)]=p[(ncol-2)*ncol+2];
	img->putXY(nrow*(ncol-1)-1,img->getXY((nrow-3)*ncol+2));
	//p[nrow*ncol-1]=p[(nrow-2)*(ncol-2)-1];
	img->putXY(nrow*ncol-1,img->getXY(nrow-2)*ncol-3-1);
	//g([1 nrow],2:end-1) = g([3 nrow-2],2:end-1);  
	for(i=1;i<nrow-2;i++)
	{
		//p[i]=p[3*ncol+i];
		img->putXY(i,img->getXY(3*ncol+i));
		//p[(ncol-1)*nrow+i-1]=p[(ncol-2)*nrow+i];//
		img->putXY((ncol-1)*nrow+i-1,img->getXY((ncol-2)*nrow+i));
	
	}
	//g(2:end-1,[1 ncol]) = g(2:end-1,[3 ncol-2]);  	
	for(j=2;j<ncol-2;j++)
	{
		//p[ncol*(j-1)+1]=p[nrow*(j-1)+1];
		img->putXY(nrow*j-1,img->getXY(nrow*j-2));
		//p[ncol*(j-1)+nrow-1]=p[nrow*(j-1)+nrow-1];
		img->putXY(nrow*j+nrow-2,img->getXY(nrow*j+nrow-1));
	}
	for (i=0;i<nrow;i++)
	{
		for(j=0;j<ncol;j++)
		{
			img->put(i,j,img->get(i,j));
		}
	}
	return img;

}

ImageF& ImageFSqrt(ImageF &x,ImageF &y)
{
	// 	ImageF x=xdata;
	// 	ImageF y=ydata;
	//p=x.gety();
	//q=y.gety();
	ImageF  *ret=new ImageF (x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=sqrt();
			ret->put(i,j,sqrt(float(x.get(i,j)*x.get(i,j)+y.get(i,j)*y.get(i,j))));
		}

	}
	return *ret;


}
ImageF& operator *(int p1,ImageF &x)
{

	//p=x.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)*p1;
			ret->put(i,j,x.get(i,j)*p1);
		}

	}
	return *ret;

}
ImageF& operator * (ImageF &x,ImageF &y)
{
	//PIXTYPE *p=x.gety();
	//PIXTYPE *q=y.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			ret->put(i,j,ret->get(i,j)*y.get(i,j));
		}

	}
	return *ret;

}
ImageF* operator * (ImageF &x,ImageF *y)
{

	//PIXTYPE *p=x.gety();
	//PIXTYPE *q=y->gety();
	ImageF *ret=new ImageF(x);
	int n=x.getXsize();
	int m=x.getYsize();
	for (int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			//q[i*m+j]*=p[i*m+j];
			ret->put(i,j,x.get(i,j)*y->get(i,j));
		}

	}
	return ret;

}
ImageF& operator/(ImageF &xdata,ImageF& ydata)
{
	/*array the corresponding position*/
	//p=xdata.gety();
	//q=ydata.gety();
	ImageF *ret=new ImageF(xdata);
	int m=xdata.getXsize();
	int n=xdata.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			/*if(*(q+i*n+j)!=0)
				*(p+i*n+j)=*(p+i*n+j)/(*(q+i*n+j));*/
			if (ydata.get(i,j)!=0)
			{
				ret->put(i,j,xdata.get(i,j)/ydata.get(i,j));

			}
		}

	}
	return *ret;

}

ImageF& operator/(ImageF &x,float t)

{
	//p=x.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	PIXTYPE temp;
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)/(t+0.001);
			temp=x.get(i,j)/t;
			if(temp=0)
			ret->put(i,j,temp);
		}

	}
	return *ret;

}
ImageF& operator/(float t,ImageF &x)

{
	//p=x.gety();
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)/(t+0.001);
			x.put(i,j,t/x.get(i,j)+0.001);
		}

	}
	return x;

}
ImageF&  operator+( ImageF  &x,float s)
{
	//p=x.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)+s;
			ret->put(i,j,x.get(i,j)+s);
		}

	}
	return *ret;

}

ImageF& div(ImageF &x,ImageF &y)
{
	ImageF *ret=new ImageF(x);
	*ret=-1*(*gradientx(&x)+*gradienty(&y))+255;
	return *ret;
}
ImageF& operator+(ImageF &x,ImageF &y)
{
	//p=x.gety();
	//q=y.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)+(*(q+i*n+j));
			ret->put(i,j,x.get(i,j)+y.get(i,j));
		}

	}
	return *ret;
}

ImageF& operator -(ImageF &x,ImageF &y)
{
	//p=x.gety();
	//q=y.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)-(*(q+i*n+j));
			ret->put(i,j,x.get(i,j)-y.get(i,j));
		}

	}
	return *ret;
}

ImageF& operator -(ImageF &x,int s)
{
	//p=x.gety();
	ImageF *ret=new ImageF(x);
	int m=x.getXsize();
	int n=x.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//*(p+i*n+j)=*(p+i*n+j)-s;
			ret->put(i,j,x.get(i,j)-s);
		}

	}
	return *ret;
}
ImageF* del2(ImageF *phi)
{

	//p=phi->gety();
	Raw2D *ret2=new Raw2D(phi);
	
	int m=phi->getXsize();
	int n=phi->getYsize();
	Raw2D *ret=new Raw2D(phi);
	//PIXTYPE *pt=ret->gety();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			if(i+1<m&&j+1<n&&i-1>=0&&j-1>=0)
			//*(p+i*n+j)=*(pt+i*n+j-1)+*(pt+(i-1)*n+j)+*(pt+i*n+j+1)+*(pt+(i+1)*n+j)-4**(pt+i*n+j);
			ret2->put(i,j,ret->get(i+1,j)+ret->get(i-1,j)+ret->get(i,j+1)+ret->get(i,j-1)-4*ret->get(i,j));

		}

	}
	//ImageF phi_x=gradientx(phi);
	//ImageF phi_y=gradienty(phi);
	/*
	for (i=0;i<m;i++)
	{
	for(j=0;j<n;j++)
	{
	*(pt+i*n+j)=*(p+i*n+j-1)+*(p+(i-1)*n+j)+*(p+i*n+j+1)+*(p+(i+1)*n+j)-4**(p+i*n+j);
	}

	}*/

	//delete []pt;
	return ret2;
}
ImageF& regFunction(ImageF &s,int m,int n)
{
	ImageF *ss=new ImageF(s);
	//p=s.gety();
	int l=ss->getXsize();
	int k=ss->getYsize();
	for (i=0;i<l;i++)
	{
		for(j=0;j<k;j++)
		{

			if(ss->get(i,j)>=m&&ss->get(i,j)<=n)
				ss->put(i,j,1);
			else if(ss->get(i,j)==m||ss->get(i,j)==n)
				ss->put(i,j,1);
			else ss->put(i,j,0);
		}

	}

	return *ss;

}
ImageF* distReg_p2(ImageF *phi)
{
	int m=phi->getXsize();
	int n=phi->getYsize();
	Raw2D *phi_x=new Raw2D(m,n);
	ImageF *phi_y=new ImageF(m,n);
	phi_x=gradientx(phi);
	phi_y=gradienty(phi);
	//ImageF s=ImageFSqrt(((*phi_x)*(*phi_x)) + ((*phi_y)*(*phi_y)));
	ImageF  s=ImageFSqrt(*phi_x,*phi_y);
	ImageF a=regFunction(s,0,1);
	ImageF b=regFunction(s,0,1);//need to be changed.
	ImageF ps=a*sin(2*s)/(2*pi)+b*(s-1);
	ImageF dps=regFunction(ps,0,0)*ps+regFunction(ps,0,0)/(regFunction(s,0,0)+regFunction(s,0,0));
	ImageF f=div(dps**phi_x-*phi_x,dps**phi_y-*phi_y)+4**del2(phi);
	*phi=f;
	//ImageF b=(s>1);
	return phi;
}
ImageF& sin(ImageF &s)
{
	//overload sin
	//p=s.gety();
	ImageF *ss=new ImageF(s);
	int m=s.getXsize();
	int n=s.getYsize();
	for (i=0;i<m;i++)
	{
		for(j=0;j<n;j++)
		{
			//(*(p+i*n+j))=(float)sin(float(*(p+i*n+j)));
			ss->put(i,j,(float)sin(float(s.get(i,j))));
		}

	}
	return *ss;
}
void write(Raw2D &destImg)
{
	//FILE *p=fopen("F:\\ls1.raw","ab+");
//	fwrite(destImg.gety(),sizeof(PIXTYPE),destImg.getXsize()*destImg.getYsize(),p);
	//fclose(p);
}
ImageF& LevelSet::initialg(ImageF &g)
{
	ImageF *ret=new ImageF(&g);
	ImageF *gx=gradientx(ret);
	ImageF *gy=gradienty(ret);
	//g=*ret;
	g=*gx**gx+*gy**gy;
	//g=(255/(g+1));//or  
	g=-255/(g+1)+255;
	delete ret;
	return g;
}
ImageF & pow(PIXTYPE e,ImageF &x)
{
	int m=x.getXsize();
	int n=x.getYsize();
	ImageF * ret=new ImageF(x);
	for (int i=0;i<m;i++)
	{
		for (int j=0;j<n;j++)
		{
			ret->put(i,j,pow((float)e,x.get(i,j)));

		}
	}
	return *ret;

}
ImageF& LevelSet::drlse_edge(ImageF &phi_0,ImageF &g,float lambda,float mu,float alfa,float epsilon,int timestep, int iter,const char * potentialFunction)
{
	ImageF *phi=new ImageF(&phi_0);
	int m=g.getXsize();
	int n=g.getYsize();
	ImageF *vx=gradientx(&g);
	ImageF *vy=gradienty(&g);
	for(int i=0;i<iter;i++)
	{
		NeumannBoundCond(phi);


		ImageF &phi_x=*gradientx(phi);
		ImageF &phi_y=*gradienty(phi);
		Raw2D *s=new Raw2D(m,n);

		*s=ImageFSqrt(phi_x,phi_y);
		float smallNumber=1e-10;
		ImageF *Nx=new ImageF(phi_x);
		ImageF *Ny=new ImageF(phi_y);

		*Nx=pow(255,phi_x/(*s+smallNumber));
		*Ny=pow(255,phi_y/(*s+smallNumber));
		ImageF * curvature=new ImageF(m,n);
		*curvature=div(*Nx,*Ny);
		ImageF *distRegTerm=new ImageF(m,n);
		//const string p1("single-well");
		//const char *p1=potentialFunction;
		char *p1="single-well";
		if (strcmp(potentialFunction,p1)+1)
		{/*
		 compute distance regularization term in equation (13) 
		 with the single-well potential p1.
		 */
			*distRegTerm= 4**del2(phi)-*curvature;
			//printf("");
		}

		else if (strcmp(potentialFunction,"double-well")+1)
		{
			*distRegTerm=distReg_p2(phi);  // compute the distance regularization term in eqaution (13) with the double-well potential p2.

			printf("asda");

		}
		else printf("EEROR");

		//float eplsion=0.1;
		Raw2D *diracPhi=new Raw2D(Dirac(phi,epsilon));
		Raw2D* areaTerm=new Raw2D((g*(*diracPhi))); 
		ImageF  *edgeTerm=new ImageF(m,n);
		*edgeTerm=*diracPhi*(*vx**Nx+*vy**Ny) + *diracPhi*g*(*curvature);
		phi_0=4**del2(phi);
		//phi_0=*phi;
		*phi=*phi + timestep*(mu**distRegTerm +lambda**edgeTerm + alfa**areaTerm);
		//phi_0=*curvature;

		

		//phi_0=*phi;
		//}
	}	
	//phi_0=*phi_x;
	return phi_0; 
}
void LevelSet::testout(Raw2D *ret){
	//PIXTYPE *test=ret->gety();
	//memcpy(test,ret->gety(),100);

int i=0;

while (i<100)
{
	//PIXTYPE test = p[i];
	i++;
	cout<<"no====>>:"<<i<<"p[i]="<<ret->getXY(i)<<endl;
}
}