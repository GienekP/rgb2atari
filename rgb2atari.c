/*--------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
/*--------------------------------------------------------------------*/
unsigned char convert(unsigned char r, unsigned char g, unsigned char b, unsigned char lvl)
{
	const double tangle[16]={0.0,135.0,112.5,90.0,67.5,45.0,22.5,337.5,315.0,292.5,270.0,225.0,202.5,180.0,157.5,135.0};
	unsigned char i,c=0,ret=0;
	double pi,rr,gg,bb,ll,Y,U,V,angle;
	pi=acos(-1);
	rr=((double)(r)/255.0);
	gg=((double)(g)/255.0);
	bb=((double)(b)/255.0);	
	ll=((double)(lvl)/500.0);	
	Y=0.299*rr+0.587*gg+0.114*bb;
	U=0.492*(bb-Y);
	V=0.877*(rr-Y);
	angle=atan2(V,U)*180.0/pi;
	if (angle<0) {angle=(360+angle);};
	if ((U*U+V*V)>(ll*ll))
	{
		double dist=360.0*360.0;
		for (i=1; i<16; i++)
		{
			double r=(tangle[i]-angle);
		    	double d=r*r;
		    	if (d<dist)
		    	{
				dist=d;
				c=i;
			};
		};
	};
	Y=(Y*255.0)/16.0;
	ret=(unsigned char)(Y);
	c<<=4;
	ret|=c;
	return ret;
}
/*--------------------------------------------------------------------*/
void rgb2atari(const char *fin, const char *fout, unsigned char lvl)
{
	unsigned int i,j;
    	FILE *fi,*fo;
    	fi=fopen(fin,"rb");
    	if (fi)
    	{
		fseek(fi,0,SEEK_END);
		j=ftell(fi);
		fseek(fi,0,0);
		fo=fopen(fout,"wb");
		if (fo)
		{
			for (i=0; i<(j/3); i++)
			{
				unsigned char r,g,b,p;
				r=getc(fi);
				g=getc(fi);
				b=getc(fi);
				p=convert(r,g,b,lvl);
				putc(p,fo);			
			};
			fclose(fo);
		};
		fclose(fi);
	};
}
/*--------------------------------------------------------------------*/
unsigned char graylvl(const char *txt)
{
	unsigned char a,b;
	a=txt[0];
	a-='0';
	a%=10;
	b=txt[1];
	if (b)
	{
		b-='0';
		b%=10;
		a*=10;
	};
	return (a+b);
}
/*--------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	switch (argc)
	{
		case 3:
		{
			rgb2atari(argv[1],argv[2],50);
		} break;
		case 4:
		{
			rgb2atari(argv[1],argv[2],graylvl(argv[3]));
		} break;
		default:
		{
			printf("RGB2ATARI - Convert RGB-RAW to ATARI-PAL - (c)GienekP\n");
			printf("use:\n");
			printf("   rgb2atari filename filename [gray threshold 0-99%%]\n");
			printf("   rgb2atari filein.rgb fileout.atari\n");		
			printf("   rgb2atari filein.rgb fileout.atari 50\n");		
		} break;
	};
	return 0;
}
/*--------------------------------------------------------------------*/
