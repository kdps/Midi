#include<ctype.h>
#include<io.h>
#include<dos.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<process.h>


extern void settable(unsigned int , unsigned int);
extern void pcmstart(unsigned int , int , int);
extern void pcmstop(int);
extern int  playseq(unsigned int * , unsigned int , int);
extern void displayoff(void);
extern void displayon(void);
extern void waitempty(void);
extern int mykbhit(void);
extern int mygetch(void);
extern void pc98patch(void);
extern void pcmstart98(unsigned int , int);
extern void pcmstop98(void);


unsigned char chvol[16]={		/* ch volume */
	100,100,100,100,100,100,100,100,
	100,100,100,100,100,100,100,100};

unsigned char chexp2[16]={		/* ch expresson */
	127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127};

unsigned char chexp[16]={		/* ch expresson*mastervol */
	127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127};

unsigned long tonebase[48]={	/* tone base table */
	0x20b404a1,0x212dcab9,0x21a95641,0x2226add2,
	0x22a5d81c,0x2326dbe9,0x23a9c01d,0x242e8bb4,
	0x24b545c7,0x253df583,0x25c8a238,0x2655534b,
	0x26e41040,0x2774e0b4,0x2807cc64,0x289cdb27,
	0x293414f2,0x29cd81d7,0x2a692a09,0x2b0715d6,
	0x2ba74dac,0x2c49da18,0x2ceec3c9,0x2d96138c,
	0x2e3fd24f,0x2eec0922,0x2f9ac138,0x304c03e4,
	0x30ffda9c,0x31b64ef9,0x326f6abb,0x332b37c1,
	0x33e9c015,0x34ab0ddf,0x356f2b73,0x36362349,
	0x37000000,0x37cccc5e,0x389c9353,0x396f5ff6,
	0x3a453d88,0x3b1e3774,0x3bfa594f,0x3cd9aed9,
	0x3dbc4400,0x3ea224da,0x3f8b5daf,0x4077faf1};


int pgmtbl[128][4]=		/* program table env,vol/8,sft,pitcpx */
   {{ 0,6,  0,0},{ 0,6,  0,0},{29,8,  0,0},{ 0,8, 0,0}, /* 0-3 */
	{30,6,  0,0},{ 0,5,  0,0},{ 2,5,  0,0},{ 2,9, 0,0}, /* 4-7 */
	{30,6,  0,0},{30,6, 12,0},{ 0,8,  0,0},{30,9, 0,0}, /* 8-11 */
	{ 1,6,  0,0},{ 9,9,  0,0},{ 0,9,  0,0},{29,9, 0,0}, /* 12-15 */
	{ 6,8,  0,0},{ 6,8,-12,0},{ 6,9,-12,0},{ 6,11,0,0}, /* 16-19 */
	{ 6,10, 0,0},{ 5,11, 0,0},{ 5,10, 0,0},{ 5,11,0,0}, /* 20-23 */
	{ 2,10, 0,0},{ 2,8,  0,0},{ 2,8,  0,0},{29,9,  0,0}, /* 24-27 */
	{29,6,  0,0},{ 5,9,  0,0},{ 6,10, 0,0},{ 5,9, 12,0}, /* 28-31 */
	{ 0,9,  0,0},{29,9,  0,0},{30,10, 0,0},{ 5,10, 0,0}, /* 32-35 */
	{29,8,  0,0},{29,9,  0,0},{ 1,10, 0,0},{ 5,10, 0,0}, /* 36-39 */
	{ 3,11, 0,0},{ 3,10, 0,0},{ 3,10, 0,0},{ 3,11, 0,0}, /* 40-43 */
	{ 6,10, 0,0},{ 9,12, 0,0},{ 0,10, 0,0},{ 0,10, 0,0}, /* 44-47 */
	{ 6,10, 0,0},{ 3,10, 0,0},{ 3,8,  0,0},{ 3,8,  0,0}, /* 48-51 */
	{ 6,8,  0,0},{ 5,10, 0,0},{ 6,8,  0,0},{10,12,-12,0}, /* 52-55 */
	{ 5,8,  0,0},{ 5, 9, 0,0},{ 5,10, 0,0},{ 5,10,  0,0}, /* 56-59 */
	{ 5,9,  0,0},{ 5,10, 0,0},{ 3,12, 0,0},{ 3,10,  0,0}, /* 60-63 */
	{ 5,10, 0,0},{ 5,10, 0,0},{ 5,10, 0,0},{ 5,10, 0,0}, /* 64-67 */
	{ 5,11, 0,0},{ 5,10, 0,0},{ 3,10, 0,0},{ 6,10, 0,0}, /* 68-71 */
	{ 3,10, 0,0},{ 6,11, 0,0},{ 6,10, 0,0},{ 3,11, 0,0}, /* 72-75 */
	{ 5,11, 0,0},{ 3,10, 0,0},{ 6,10, 0,0},{ 6,10, 0,0}, /* 76-79 */
	{ 6,10, 0,0},{ 6,11, 0,0},{ 6,10, 0,0},{ 5,10, 0,0}, /* 80-83 */
	{ 6,10, 0,0},{ 6,10, 0,0},{ 6,10,-12,0},{ 5,11, 0,0}, /* 84-87 */
	{ 6,10, 0,0},{ 4,8,  0,0},{ 5,9,  0,0},{ 3,11,  0,0}, /* 88-91 */
	{ 3,8,  0,0},{ 4,8,  0,0},{ 6,9,  0,0},{ 4,6,  0,0}, /* 92-95 */
	{ 0,9,  0,0},{ 3,8,  0,0},{ 1,11, 0,0},{ 0,11, 0,0}, /* 96-99 */
	{ 0,11, 0,0},{ 4,5,  0,0},{ 4,9,  0,0},{30,10, 0,0}, /* 100-103 */
	{30,11, 0,0},{29,11, 0,0},{ 1,10, 0,0},{29,10, 0,0}, /* 104-107 */
	{ 1,10, 0,0},{ 6,12, 0,0},{ 6,12, 0,0},{ 6,12, 0,0}, /* 108-111 */
	{ 1,7, 10,0},{19,10,12,0},{ 9,10, 0,0},{28,6, 26,1}, /* 112-115 */
	{10,12,-55,1},{11,12,-18,1},{12,12, 0,1},{13,12,37,1}, /* 116-119 */
	{14,3,  0,0},{15,3,  0,0},{ 8,0,  0,0},{ 8,0,  0,0}, /* 120-123 */
	{ 8,0,  0,0},{ 8,0,  0,0},{ 8,0,  0,0},{ 2,14, 0,0}};/* 124-127 */ 

char bank126[64]={
	   1,  1,  1,  3,  0,  1,  1,  4,	/*   0 -   7 */
	   4,  5, 25, 25, 25, 28, 28, 36,	/*   8 -  16 */
	  36, 36, 36, 37, 37, 37, 37, 33,   /*  17 -  23 */
	  33, 34, 34, 35, 32, 52, 52, 52,   /*  24 -  31 */
	  52, 49, 48, 50, 50, 16, 16, 16,   /*  32 -  39 */
	  17, 16, 16, 17, 17, 17, 56, 56,   /*  40 -  47 */
	  57, 57, 57, 57, 57, 57, 65, 66,   /*  48 -  55 */
	  67, 65, 61, 61, 61, 61, 61, 55};  /*  56 -  63 */

char bank127[128]={
	   0,  1,  2,  4,  4,  5,  5,  3,	/*  0 -   7 */
	  16, 17, 18, 16, 19, 19, 19, 21,	/*  8 -  15 */
	 127,  6,  6,  7,  7,  7,  8,  8,   /* 16 -  23 */
	  62, 63, 62, 63, 38, 39, 38, 39,   /* 24 -  31 */
	  88, 92, 90, 88, 97, 99, 99,103,   /* 32 -  39 */
	 102, 96, 68,102, 81, 81, 10, 80,   /* 40 -  47 */
	  48, 48, 48, 45, 40, 40, 42, 42,   /* 48 -  55 */
	  43, 46, 46, 24, 24, 25, 25,104,   /* 56 -  63 */
	  32, 32, 38, 39, 36, 37, 24, 29,   /* 64 -  71 */
	  73, 73, 72, 72, 74, 75, 65, 65,   /* 72 -  79 */
	  65, 65, 71, 71, 68, 69, 70, 22,   /* 80 -  87 */
	  56, 56, 57, 57, 60, 60, 58, 61,   /* 88 -  95 */
	  61, 11, 11, 12,112,  9, 14, 13,   /* 96 -  103 */
	  12,107, 97, 77, 78, 78, 76, 75,   /*104 -  111 */
	  75, 47, -1,118,118,118,116, 12,   /*112 -  119 */
	 115, 11, 55,124,123,126,126,126};  /*120 -  127 */

 int envtbl[]={ /* envelope tablle   flag,vol,delay,vol,delay,...,vol,0,2 */
	1,9, 7,8, 14,6, 28,4, 42,3,84,2,126,1,248,0,-1,	/* 0 :piano 1 */
	1,9, 4,6, 12,3, 24,2, 36,1,72,0,-1,				/* 1 :marimba */
	1,5, 2,8, 14,6, 21,4, 28,3, 35,2,42,1,-1,	/* 2 :guiter */
	0,3,  2,5,  4,7, 13,8,-1,					/* 3 :violin */
	0,3, 13,4,25,5,37,6,50,7, 70,8,-1,			/* 4 :slow */
	0,5,  2,8, 30,7,-1,							/* 5 :trumpet */
	0,6,  2, 8,-1,								/* 6 :organ */
	0,8, -1,									/* 7 :flat */
	0,0, -1,									/* 8 :silent */

	0,8,  2,5,     4,3,     6,2,10,1,15,0,-1,	/* 9:Short Hit */
	0,8,  5,5,    10,3,    15,2,    20,0,-1,	/* 10:Long Hit */
	2,8,  3,6, 0,  6,4,-1,  9,2,-2, 12,0,0,-1,	/* 11:Tom */
	2,8,  3,6,-1,  6,4,-2,  9,2,-3, 12,0,0,-1,	/* 12:Syn Tom */
	0,1,5,2,10,3,15,4,20,5,25,6,30,7,35,8,40,0,-1,	/* 13:rev syn */
	0,2,  2,4,     4,6,    10,3,    12,0,-1,	/* 14:scratch */
	0,5,  2,7,     3,8,     4,5,     5,0,-1,	/* 15:hat-env */
	0,8,  2,5,     3,3,     4,2,     5,0,-1,	/* 16:Short Hit */
	0,6,  2,4,     5,3,     7,2,    10,0,-1,	/* 17:Midium Hit */
	0,6,  5,4,    10,3,    15,2,    20,0,-1,	/* 18:Long Hit */
	0,8,  1,5,     2,3,     3,2,     4,0,-1,	/* 19:Bit Hit */
	0,5,  1,3,     2,2,     3,1,     4,0,-1,	/* 20:Bit Hit Small */
	0,5,  5,3,    10,2,    15,1,    20,0,-1,	/* 21:Long Hit Small */
	2,5,  2,7,12,10,4,12, 12,2,12, 14,0,0,-1,	/* 22: Woo! */
	1,8, 10,5,   30,4,50,2,70,2,120,1,200,0,-1, /* 23: Muted Guter */
	2,6,  1,4,-24, 2,2,-48, 3,1,-72, 4,0,0,-1,	/* 24:Hi Q */
	0,8, 12,5,    20,3,    40,2,   100,0,-1,	/* 25:Crash Hit */
	0,8,  1,0,-1,								/* 26:Click */
	0,5,  2,7,     3,8,    60,3,    65,0,-1,	/* 27:scratch */
	0,7,  1,0,-1,								/* 28:Hyper Bit Hit */

	1,9, 7,7, 14,5, 28,4, 42,3,84,2,126,1,252,0,-1,	/* 29:piano 3 */
	1,7, 2,9, 14,8, 28,7, 42,6, 63,5,84,4,126,3,-1,	/* 30:e.piano 1 */
	-2};

int releaserate=8;

#define ENV9	67		/* pointer of flat enverope */

int drum[61][2]=	/* drum set table {key,env} */
	{{268,24},{ 16,16},{380,14},
	 {360,14},{460,19},{144,19},{164,16},{435,18},{126, 9},
	 {117, 9},{ 16,19},{ 16,10},{ 16,15},{ 16, 9},{182,11}, /* C2 */
	 { 16,20},{198,11},{ 16,20},{210,11},{ 16,21},{228,11},
	 {242,11},{ 16,25},{256,11},{404,17},{ 16,18},{402,18}, /* C3 */
	 {404,16},{ 16,17},{233,19},{ 16,25},{ 16,14},{456,17},
	 {273,19},{265,19},{276,26},{245,16},{225,16},{217, 9}, /* C4 */
	 {197, 9},{356,16},{336,16},{ 16,15},{ 16,19},{392,15},
	 {381,27},{162,15},{128,11},{387,16},{271,19},{251,19}, /* C5 */ 
	 {260,22},{144,22},{374,19},{374,18},{ 16,19},{372,17},
	 {352,12},{364,26},{148,19},{146,10}};

int modtbl[4][8]=	/* pitch modulation depth table */
	{{ 0, 1, 1, 0, 0, 0, 0, 0},
	 { 1, 1, 1, 1, 0, 0, 0, 0},
     { 0, 1, 1, 0, 0,-1,-1, 0},
	 { 0, 1, 1, 1, 0,-1,-1,-1}};

int	env[16]		 	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char bendrange[16]	= {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
char lastkey[16]	 	= {60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60};
char hold[16]	 	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char pitbend[16] 	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char portament[16]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char shift[16]		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char pitcpx[16]		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char chmodl[16]		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char pgmvol[16]		= {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
char drumpart[16]	= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char bank[16]		= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char mask[17]={"                "};	/* MIDI channel mask */

unsigned char portmap[16][6];		/* port mapping reg. */
									/* ch,tone,truetone,seq,vol,truevol */
int timing[256][4];					/* timing stack:delay,port,tone,vol */
unsigned char holdflg[16][128];
unsigned int  workseg[10];

char volmap[16];

unsigned long basefreq=2000000;	/* master clock (18MHz) */
unsigned long tempo   =500000;	/* tempo=500ms/quarter */
unsigned int intperiod=95;		/* interrupt period */
int	tmpadj=100;					/* tempo adjust (%) */
int	keyadj=0;					/* key adjust (tone) */
int	drumadj=100;				/* drum vol adjust (%) */
int	mastervol=127;				/* master volume */

int	timptr=0;						/* timing stack */

int	portmax=11;
int	melodymin=0;

int	envoff=0;
int	segnmax,segn;
unsigned char far *datap;
unsigned long convrate;
unsigned int vol;
unsigned char seqhis=0;
int chmodpor=0,lfocnt=0,lfo=0,lfoerr=0,volsmp=16,peakvol=1,nowvol=0;
int useems=0;

int emmha=-1;
int emsmaxblock=0;
unsigned int emmseg;


void resetenv(int);
int pbconv(int pbend,int *cmd);

/*** EMS related function ***/
int chkemm(void){
	union REGS regs;
	struct SREGS sregs;
	char far *emmtest;
	char emm[]="EMMXXXX0";
	int	i;
	unsigned int dseg;
	regs.x.ax=0x3567;
	int86x(0x21, &regs, &regs, &sregs);
	dseg=sregs.es;
	emmtest = (char far*)( ( (long)(dseg) <<16)+0x0a );
	for (i = 0;i < 8; i++){
		if (*(emmtest+i) != emm[i]) return -1;
	}
	return 0;
}

int getseg(void){
	unsigned int pageseg;
	unsigned char status;
	union REGS regs;
	regs.h.ah=0x41;
	int86(0x67, &regs, &regs);
	pageseg=regs.x.bx;
	status=regs.h.ah;
	if (status != 0) {
		return -1;
	}else{
		return pageseg;
	}
}

int	chkfreepage(void){
	unsigned int pagenum;
	unsigned char status;
	union REGS regs;
	regs.h.ah=0x42;
	int86(0x67, &regs, &regs);
	pagenum=regs.x.bx;
	status=regs.h.ah;
	if (status != 0) {
		return 0;
	}else{
		return pagenum;
	}
}

int	pageget(int	page){
	int	ha;
	unsigned char status;
	union REGS regs;
	regs.h.ah=0x43;
	regs.x.bx=page;
	int86(0x67, &regs, &regs);
	ha=regs.x.dx;
	status=regs.h.ah;
	if (status != 0) {
		return -1;
	}else{
		return ha;
	}
}

int	fourpagemap(int	ha,int	logpage){
	unsigned char status;
	union REGS regs;
	int	i;
	char	phipage=0;
	for(i=0;i<4;i++){
		regs.x.dx=ha;
		regs.h.ah=0x44;
		regs.h.al=phipage;
		regs.x.bx=logpage;
		int86(0x67, &regs, &regs);
		status=regs.h.ah;
		if (status != 0) {
			return -1;
		}
		phipage++;
		logpage++;
	}
	return 0;
}

int emm_free(int	ha){
	unsigned char status;
	union REGS regs;
	regs.h.ah=0x45;
	regs.x.dx=ha;
	int86(0x67, &regs, &regs);
	status=regs.h.ah;
	return status; 
}

/* get heap memory */

void	getmem(void){
	segnmax=0;
	while ((allocmem(4096,&workseg[segnmax]))==-1)	segnmax++;
	workseg[segnmax]=0;
}


void	getems(void){
	emsmaxblock=chkfreepage()/4;
	if (emsmaxblock>0){
		if ((emmha=pageget(emsmaxblock*4)) == -1) emsmaxblock=0;
	}
}


/* release heap memory */

void relmem(void){
	int	i=0;
	while(workseg[i]!=0){
		freemem(workseg[i]);
		i++;
	}
	if (emmha>=0){
		emm_free(emmha);
	}
}


int nextseg(void){
	if (segn < segnmax+emsmaxblock-1){
		segn++;
		if (segn < segnmax){
			datap=(unsigned char far*)((unsigned long)(workseg[segn])<<16);
		}else{
			fourpagemap(emmha,(segn-segnmax)*4);
			datap=(unsigned char far*)((unsigned long)(emmseg)<<16);
		}
		return 0;
	}
	datap=datap-FP_OFF(datap)+0Xffef;
	return 1;
}

/* send data to seq buf */

int	send0(int deltatime,int port,int key,int volm){
	unsigned int i;
	while (1){
		if (key<=3)	break;
		if (key==127){
			if ((port==7)&&(volm==63)) break;
			if ((port==0)&&(volm==32)) break;
		}
		nowvol+=(char)volm-volmap[port];
		volmap[port]=(char)volm;
		if (peakvol<nowvol) peakvol=nowvol;
		break;
	}
	deltatime=(deltatime<<1)|(port>>3);
	if ( deltatime >= 0x80 ){
		*datap=(unsigned char) (deltatime | 0x80);
		datap++;
		*datap=(unsigned char) (deltatime >> 7);
		datap++;
	}else{
		*datap=(unsigned char) deltatime;
		datap++;
	}
	*(unsigned int far *)datap=(unsigned int)((key<<9)|(volm<<3)|(port&7));
	datap+=2;

	if ( FP_OFF(datap) <= 0xfff0) return 0;

	*datap=0x00;datap++;
	*datap=0xff;datap++;
	*datap=0xff;
	while (FP_OFF(datap)<0xffff){
		datap++;*datap=0xff;
	}
	return nextseg();
}


/* send data with pitch modulate execution */

int	send(int deltatime,int port,int key,int volm)
{
	int i,j,k,l,x,y,pbend,pb,cmd,f=0;
	while ((j=deltatime-lfocnt)>=0){
		for (i=0;i<=portmax;i++){
			if ((k=portmap[i][0])>0){
				f=1;				
				if ((l=chmodl[k-1])>0){
					pbend=pitbend[k-1]+modtbl[min(l-1,3)][chmodpor];
					pb=pbconv(pbend,&cmd);
					send0(lfocnt,i,cmd,pb);
					deltatime=j;
					lfocnt=0;
				}
			}
		}
		lfocnt+=lfo>>4;
		lfoerr+=lfo&15;
		if (lfoerr>16){
			lfoerr-=16;
			lfocnt++;
		}
		if ((chmodpor++)>7) chmodpor=0;
		if (f==0) chmodpor=0;
	}
	lfocnt-=deltatime;
	if (volm>63) volm=63;
	return send0(deltatime,port,key,volm);
}


/* send data with expression execution*/

int	send1(int deltatime,int port,int key,int volm){
	int v=0;
	if (volm!=0) v=((int)volm*chexp[portmap[port][0]-1]+64)>>7;
	return send(deltatime,port,key,v);
}


/* envelope event execution */

int	execenv(int deltatime){
	int	i,j,port,truekey,volm;
	int *ptr;
	char rel[16];
	if (deltatime>0){
		for(i=0;i<=portmax;i++) rel[i]=0;
		for(i=timptr-1;i>=0;i--){
			ptr=(int *)timing[i];
			if ((j=deltatime-*ptr)<0){
				*ptr-=deltatime;
				break;
			}
			deltatime=j;
			port=*(ptr+1);
			truekey=*(ptr+2);
			volm=*(ptr+3);
			if (truekey<4){
				send(*ptr,port,truekey,volm);
			}else{
				if (volm==0){
					if (portmap[port][5]>0){
						truekey=127;
						rel[port]=1;
					}
				}
				send1(*ptr,port,truekey,volm);
				portmap[port][2]=truekey;
				portmap[port][5]=volm;
			}
		}
		timptr=i+1;
		for(i=0;i<=portmax;i++){
			if	(rel[i]){
				portmap[i][0]=0;
				resetenv(i);
			}
		}
	}
	return deltatime;
}


/* Read Variable-length-value (SMF Numeral Format) */

unsigned long ReadVarLen (FILE *fp)
{
	unsigned long value=0;
	unsigned char c;
	int	j;
	if ((value = getc(fp)) & 0x80){
		value &= 0x7f;
		for(j=0;j<3;j++){
			value = (value << 7) + ((c = getc(fp)) & 0x7f);
			if ((c & 0x80)==0) break;
		}
	}
	return (value);
}


/* Read 4bytes (little endian) */

unsigned long ReadLong (FILE *fp)
{
	unsigned long value;
	value=(unsigned long)getc(fp);
	value=(value<<8)+(unsigned long)getc(fp);
	value=(value<<8)+(unsigned long)getc(fp);
	value=(value<<8)+(unsigned long)getc(fp);
	return (value);
}


/* Read 3bytes (little endian) */

unsigned long ReadSemiLong (FILE *fp)
{
	unsigned long value;
	value=(unsigned long)getc(fp);
	value=(value<<8)+(unsigned long)getc(fp);
	value=(value<<8)+(unsigned long)getc(fp);
	return (value);
}


/* Read 2bytes (little endian) */

unsigned int ReadWord (FILE *fp)
{
	unsigned int value;
	value=(unsigned int)getc(fp);
	value=(value<<8)+(unsigned int)getc(fp);
	return (value);
}

/* Write 4bytes (little endian) */

int	WriteLong (unsigned long dt,FILE *fp)
{
	int	i;
	for (i=0;i<4;i++){
		if (putc((unsigned char)(dt>>24),fp)==EOF) return 1;
		dt=dt<<8;
	}
	return 0;
}


/* Write 2bytes (little endian) */

int	WriteWord (unsigned int dt,FILE *fp)
{
	int	i;
	for (i=0;i<2;i++){
		if (putc((unsigned char)(dt>>8),fp)==EOF) return 1;
		dt=dt<<8;
	}
	return 0;
}


/* calculate tempo convertion ratio (convrate/0xfff) */

void temposet(unsigned long tmp,unsigned int div)
{
	convrate=(unsigned long)(
			((tmp>>6)*100/tmpadj) * (((unsigned long)(basefreq)<<10)/1000000)
			/ ((unsigned long)(div)*intperiod) );
}


/* insert to enverope order stack */

void setenv(int deltatime,int port,int key,int volume){
	int	i,j;
	int	*ptr;
	if (deltatime==0) return;
	for(i=timptr-1;i>=0;i--){
		if ((j=deltatime-timing[i][0])<0){
			timing[i][0]-=deltatime;
			break;
		}
		deltatime=j;
		ptr=(int *)timing[i];
		*(ptr+4)=*ptr;
		*(ptr+5)=*(ptr+1);
		*(ptr+6)=*(ptr+2);
		*(ptr+7)=*(ptr+3);
	}
	ptr=(int *)timing[i+1];
	*ptr=deltatime;
	*(ptr+1)=port;
	*(ptr+2)=key;
	*(ptr+3)=volume;
	timptr++;


	if (timptr>255) {
		timptr=255;
		printf("Envelope stack overflow!\n");
	}
}


/* delete from enverope order stack */

void resetenv(int port){
	int	i,j;
	int *ptr;
	i=0;
	while (i<timptr){
		if (timing[i][1]==port){
			if (i!=0) timing[i-1][0]+=timing[i][0];
			ptr=(int *)timing[i];
			for (j=i;j<timptr-1;j++){
				*ptr    =*(ptr+4);
				*(ptr+1)=*(ptr+5);
				*(ptr+2)=*(ptr+6);
				*(ptr+3)=*(ptr+7);
				ptr+=4;
			}
			timptr--;
		}else{
			i++;
		}
	}
}


/* make pitch bend command */

int pbconv(int pbend,int *cmd){
	int	res;
	if (pbend>=64){
		*cmd=1;
		res=pbend-64;
	}else if (pbend>=0){
		*cmd=0;
		res=pbend;
	}else if (pbend>-64){
		*cmd=2;
		res=-pbend;
	}else{
		*cmd=3;
		res=-pbend+64;
	}
	if (res>63) res=63;
	return(res);
}


/* key on */

void keyon(int delta,int port,int truekey,int volume,int envptr,int ch){	
	int p,r,cmd,pb,i,j,k,l,m,pbend,por,lstkey,v;
	pbend=pitbend[ch]; /* pitch bend flag */
	por=portament[ch]; /* portament flag */
	lstkey=lastkey[ch]; /* last note */

	v=volume*envtbl[envptr+1];
	v=(v+4)>>3;
	if (portmap[port][5]>0){
		send(delta,port,127,0); /* key off */
		delta=0;
	}
	resetenv(port);
	portmap[port][5]=v;
	send1(delta,port,truekey,v); /* key on */

	if (pbend){
		pb=pbconv(pbend,&cmd);
		send(0,port,cmd,pb); /* pitch bend */
	}
	if (por) {
		k=(lstkey-truekey)*12/16+pbend;
		m=0;
		pb=pbconv(k,&cmd);
		send(0,port,cmd,pb);
		for (i=1;i<10;i++){
			j=(lstkey-truekey)*(9-i)/16+pbend;
			l=por*i/9;
			if ((j!=k)&&(l!=m)) {
				pb=pbconv(j,&cmd);
				setenv(l,port,cmd,pb);
				k=j;
				m=l;
			}
		}
	}
	switch (envtbl[envptr]){
	  case 1:
		r=truekey/24;
		envptr+=2;
		while ((p=envtbl[envptr])>=0){
			p=((unsigned int)p<<2)>>r;
			v=volume*envtbl[envptr+1];
			if (v!=0) v=(v+4)>>3;
				if (p!=0){
				setenv(p,port,truekey,v);
			}else{
				portmap[port][5]=v;
				send1(0,port,truekey,v);
			}
			envptr+=2;
			if (v==0) break;
		}
		break;
	  case 2:
		envptr+=2;
		while ((p=envtbl[envptr])>=0){
			v=volume*envtbl[envptr+1];
			if (v!=0) v=(v+4)>>3;
			if (p!=0){
				setenv(p,port,truekey+envtbl[envptr+2],v);
			}else{
				portmap[port][5]=v;
				send1(0,port,truekey+envtbl[envptr+2],v);
			}
			envptr+=3;
			if (v==0) break;
		}
		break;
	  default:
		envptr+=2;
		while ((p=envtbl[envptr])>=0){
			v=volume*envtbl[envptr+1];
			if (v!=0) v=(v+4)>>3;
			if (p!=0){
				setenv(p,port,truekey,v);
			}else{
				portmap[port][5]=v;
				send1(0,port,truekey,v);
			}
			envptr+=2;
			if (v==0) break;
		}
	}
}

/* beep sound port assign */

int	portassign(int deltatime,char ch,char key,char truekey,int volume){
	int i,j,same=-1,free=-1,free2=-1,envptr,age=0;
	int mostold=0;
	envptr=env[ch];
	/* 空きch検索 */
	if (ch==9){
		same=0;
		melodymin=1;
	}else{
		for (i=melodymin;i<=portmax;i++){
			if (portmap[i][0]==ch+1){
				if (portmap[i][2]==truekey) {
					same=i; /* 同じchで同じ音程 */
					break;
				}
			}
			if (portmap[i][0]==0){
				free=i; /* 空きポートでiが最大のものを捜す */
				if (portmap[i][5]==0) free2=free;
			}
		}
	}
	if (free2>=0) free=free2;	 /* release phase中でないポート優先 */
	if (same>=0){		/* 同一chで同一key */
		if (ch==9){
			if ((portmap[same][0]>0)&&(portmap[same][2]==4)&&(deltatime==0)){
				if (truekey!=4) return deltatime;
				if (portmap[same][4]>volume) return deltatime;
			}
		}
		portmap[same][0]=ch+1;
		portmap[same][1]=key;
		portmap[same][2]=truekey;
		if (deltatime!=0) seqhis++;
		portmap[same][3]=seqhis;
		portmap[same][4]=volume;
		send(deltatime,same,127,     0); /* key off */
		resetenv(same);
		
		keyon(0,same,truekey,volume,envptr,ch); /* key on */
		deltatime=0;
		return deltatime;
	}
	if (free>=0){  /* 空きポートあり */
		portmap[free][0]=ch+1;
		portmap[free][1]=key;
		portmap[free][2]=truekey;
		if (deltatime!=0) seqhis++;
		portmap[free][3]=seqhis;
		portmap[free][4]=volume;
		keyon(deltatime,free,truekey,volume,envptr,ch); /* key on */
		deltatime=0;
		return deltatime;
	}

	/* 空きポートなし */
	/* 一番消しても影響の少なそうな音を探す */
	j=portmax;
	for (i=melodymin;i<=portmax;i++){
		age=min((unsigned char)(seqhis-portmap[i][3]-1),8);
		age+=(127-portmap[i][2])>>4;	/* tone  influence:8 */
		age+=32-(portmap[i][4]<<5)/vol;	/* vol influence:32 */
		if (portmap[i][2]==truekey) age+=16;
		if (portmap[i][0]==ch+1)	age+=20;
		if (age>=mostold){
			mostold=age;
			j=i;
		}
	}
	portmap[j][0]=ch+1;
	portmap[j][1]=key;
	portmap[j][2]=truekey;
	if (deltatime!=0) seqhis++;
	portmap[j][3]=seqhis;
	portmap[j][4]=volume;
	send(deltatime,j,127,     0); /* key off */
	deltatime=0;
	resetenv(j);
	keyon(0        ,j,truekey,volume,envptr,ch); /* key on */
	return deltatime;
}


/* beep sound port release */

int	portfree(int deltatime,char ch,char key){
	int i;
	for (i=0;i<=portmax;i++){
		if (portmap[i][1]==key){
			if (portmap[i][0]==ch+1){
				if (pitbend[ch]==0){
					portmap[i][5]>>=1;
				}else{
					portmap[i][5]=0;
				}
				if (portmap[i][5]==0){
					send(deltatime,i,127,0);
				}else{
					send1(deltatime,i,portmap[i][2],portmap[i][5]);
				}
				portmap[i][0]=0;
				deltatime=0;
				resetenv(i);
				if (portmap[i][5]>0) setenv(releaserate,i,127,0);
				break;
			}
		}
	}
	return deltatime;
}


/* all note off */

int	allnoteoff(int deltatime,char ch){
	int i;
	for (i=0;i<=portmax;i++){
		if (portmap[i][0]==ch+1){
			portmap[i][0]=0;
			portmap[i][5]=0;
			send(deltatime,i,127,0); /* key off */
			resetenv(i);
			deltatime=0;
		}
	}
	return deltatime;
}


/* real time pitch bend execution */

int	realbend(int deltatime,char ch,int pbend){
	int i,cmd,pb;
	for (i=0;i<=portmax;i++){
		if (portmap[i][0]==ch+1){
			pb=pbconv(pbend,&cmd);
			send(deltatime,i,cmd,pb); /* pitch bend */
			deltatime=0;
		}
	}
	return deltatime;
}


/* execute real time expression */

int	realexp(int deltatime,char ch){
	int	i;
	for (i=0;i<=portmax;i++){
		if (portmap[i][0]==ch+1){
			send1(deltatime,i,portmap[i][2],portmap[i][5]);
			deltatime=0;
		}
	}
	return deltatime;
}

/* change master volume */
int	setmastervol(int dt)
{
	int ch;
	for(ch=0;ch<16;ch++){
		chexp[ch]=(long)chexp2[ch]*mastervol/127;
		dt=realexp(dt,ch);
	}
	return dt;
}


/* note off */

int noteoff(int deltatime,char ch,char key){
	if (hold[ch]){
		holdflg[ch][key]=1;
	}else{
		deltatime=portfree(deltatime,ch,key);
	}
	return deltatime;
}

/* dumper off */
int dumperoff(int deltatime,char ch){
	int i;
	for(i=0;i<128;i++){
		if (holdflg[ch][i]){
			deltatime=portfree(deltatime,ch,i);
			holdflg[ch][i]=0;
		}
	}
	return deltatime;
}

/* save LXM file */
int	savelxm(FILE *fp){
	unsigned long	fsz,lc;
	unsigned int	chksum=0,dt,seg=0;
	unsigned char far *ptr;
	ptr=0;
	fsz=((unsigned long)(segn)<<16) + FP_OFF(datap);
	putc('L',fp);
	putc('X',fp);
	putc('M',fp);
	putc('0',fp);
	WriteLong(basefreq,fp);
	WriteWord(intperiod,fp);
	WriteWord(portmax,fp);
	WriteLong(fsz,fp);
	for (lc=0;lc<fsz;lc++){
		if (FP_OFF(ptr)==0){
			if (seg<segnmax){
				ptr=(unsigned char far*)((unsigned long)(workseg[seg])<<16);
			}else{
				fourpagemap(emmha,(seg-segnmax)*4);
				ptr=(unsigned char far*)((unsigned long)(emmseg)<<16);
			}
			seg++;
		}
		dt=*ptr;
		ptr++;
		chksum+=dt;
		if ((putc(dt,fp))==EOF) return 1;
	}
	if (WriteWord(chksum,fp)==EOF) return 1;
	return 0;
}

/* load LXM file */
int	loadlxm(FILE *fp){
	unsigned long	fsz,lc;
	unsigned int	chksum=0;
	int	dt;
	basefreq=ReadLong(fp);
	intperiod=ReadWord(fp);
	portmax=ReadWord(fp);
	fsz=ReadLong(fp);
	for (lc=0;lc<fsz;lc++){
		if ((dt=getc(fp))==EOF) return 5;
		chksum+=dt;
		*datap=(unsigned char)dt;
		datap++;
		if (FP_OFF(datap) == 0){
			if (nextseg()>0) return 3;
		}
	}
	if (chksum!=ReadWord(fp)) return 5;
	return 100;
}

/* analize SMF file and convert to beep seq. data */

int	smfconv(FILE *fp){
	unsigned char strbuf[256];
	int d;
	unsigned int div,cnt=0;
	int	deltatime=0;
	unsigned long delta,time=0,size,v1,lngs;
	char pgm,key,truekey,vel,ch,cmd,arg,cmdhis=0x80;
	int	i,j,pbh,pbl,pb,f;
	int	endmark=0,volume,portamenttime=0;
	int	rpch[16],rpcl[16],envno[64];

	/* check SMF Header */
	fgets(strbuf,5,fp);
	if (strcmp(strbuf,"LXM0")==0)	return loadlxm(fp);
	if (strcmp(strbuf,"MThd")!=0)	return 1;
	if (ReadLong(fp)!=6)		return 1;
	if (ReadLong(fp)!=1)		return 2;
	div=ReadWord(fp);
	temposet(tempo,div);
	fgets(strbuf,5,fp);

	/* check SMF Data */
	if (strcmp(strbuf,"MTrk")!=0)	return 1;
	ReadLong(fp);	/* ignore track length */

	/* recalc & headsearch envelope table */
	i=0;
	j=0;
	while ((f=envtbl[i])!=-2){
		envno[j++]=i;
		i+=2;
		while (envtbl[i]!=-1){
			envtbl[i]=((((long)(envtbl[i])*(basefreq/intperiod))>>13)+1)>>1;
			if (f==2) i+=3; else i+=2;
		}
		i++;
	}
	releaserate=((((long)(releaserate)*(basefreq/intperiod))>>13)+1)>>1;
	if (releaserate==0) releaserate=1;

/* start analisys */
	while(endmark==0){
		if ((cnt++)>=256){
			cnt=0;
			putchar('.');
			if (mykbhit()) return 4; /* use break */
		}
		delta=ReadVarLen(fp);
		time+=delta*convrate;
		deltatime=min(0x3fff,(unsigned int)deltatime+(unsigned int)(time>>12));
		time=time & 0xfff;

		deltatime=execenv(deltatime); /* envelope operation */

		d=getc(fp);
		if (d == EOF) return 5;
		if (d < 0x80){	/* running sequence */
			key=d;
			d=cmdhis;
		}else{
			key=getc(fp);
			if (key<0) key=0;
			if ((d != 0xf0) && (d != 0xf7) && (d != 0xff)) cmdhis=d;
		}
		ch=d & 0x0f; /* MIDI ch 0-15 */
		if (((d & 0xf0)!=0xf0)&&(drumpart[ch]!=0)) ch=9;
		switch (d & 0xf0){

		  case 0x80:	/* key off */
			vel=getc(fp);
			if (mask[ch]=='M') break;
			if (ch==9) break;
			if ((key<5)||(key>126)) break;
			deltatime=noteoff(deltatime,ch,key);
			break;

		  case 0x90:	/* key on */
			vel=getc(fp);
			if (vel<0){
				printf("Illegal velocity (%d) appeared on ch.%d\n",vel,ch);
				vel=0;
			}
			if ((key<5)||(key>126)) break;
			if (mask[ch]=='M') break;
			v1=((unsigned long)(vel)*chvol[ch]*pgmvol[ch])>>3;
			truekey=key;
			if (ch==9){
				if ((key>=27)&&(key<=87)) {
					env[ch]=envno[drum[key-27][1]];
					truekey=drum[key-27][0]>>2;
					pitbend[ch]=drum[key-27][0]&3;
					v1=v1*drumadj/100;
				}else{
					break;
				}
			}
			if ((v1==0)||(envtbl[env[ch]+1]==0)){
				if (ch==9) break;
				deltatime=noteoff(deltatime,ch,key);
			}else{
				truekey+=shift[ch];
				if (pitcpx[ch]){
					i=pitbend[ch];
					j=truekey-60;
					j=j*24/13;
					pitbend[ch]+=(j&3);
					truekey=(j>>2)+60;
				}
				if ((truekey>=4)&&(truekey<=126)){
					volume=(((v1*vol)>>13)+1)>>1;
					holdflg[ch][key]=0;
					deltatime=portassign(deltatime,ch,key,truekey,volume);
					lastkey[ch]=truekey;
				}
				if (pitcpx[ch]) pitbend[ch]=i;
			}
			break;

		  case 0xa0:	/* key pressure */
			getc(fp);	/* ignore */
			break;

		  case 0xb0:	/* control change */
			cmd=key;
			arg=getc(fp);
			if (arg<0) break;
			if (mask[ch]=='M') break;
			switch (cmd){
			  case 0x00: /* bank select MSB */
				bank[ch]=arg;
				break;
			  case 0x01: /* pitch modulation */
				chmodl[ch]=(arg+16)>>5;
				break;
			  case 0x05: /* portament time */
				portamenttime=arg*(basefreq/(intperiod*256))/160;
				break;
			  case 0x06: /* data entry */
				if ((rpch[ch]==0)&&(rpcl[ch]==0)){ /* pitch bend depth */
					bendrange[ch]=min(arg,24)*4;
				}
				break;
			  case 0x07: /* channel volume */
				chvol[ch]=arg;
				break;
			  case 0x0b: /* channel expression */
				chexp2[ch]=arg;
				chexp[ch]=(long)arg*mastervol/127;
				deltatime=realexp(deltatime,ch);
				break;
			  case 0x40: /* hold 1 (dumper pedal) */
				if (arg<0x40){
					hold[ch]=0;
					deltatime=dumperoff(deltatime,ch);
				}else{
					hold[ch]=1;
				}
				break;
			  case 0x41: /* portament on/off */
				if (arg<0x40){
					portament[ch]=0;
				}else{
					portament[ch]=portamenttime;
				}
				break;
			  case 0x62: /* NRPN LSB */
				rpcl[ch]=-1;
				break;
			  case 0x63: /* NRPN MSB */
				rpch[ch]=-1;
				break;
			  case 0x64: /* RPN LSB */
				rpcl[ch]=arg;
				break;
			  case 0x65: /* RPN MSB */
				rpch[ch]=arg;
				break;
			  case 0x79: /* channel reset */
				portament[ch]=0;
				chmodl[ch]=0;
				chexp[ch]=127;
				deltatime=realexp(deltatime,ch);
				hold[ch]=0;
				deltatime=dumperoff(deltatime,ch);
				pitbend[ch]=0;
				deltatime=realbend(deltatime,ch,0);
				bank[ch]=0;
				break;
			  case 0x78: /* all note off */
			  case 0x7b:
			  case 0x7c:
			  case 0x7d:
			  case 0x7e:
			  case 0x7f:
				deltatime=allnoteoff(deltatime,ch);
				break;
			}
			break;

		  case 0xc0: /* program change */
			pgm=key;
			if (bank[ch]==126){
				if (pgm<64){
					pgm=bank126[pgm];
				}else{
					pgm=0;
				}
			}else if (bank[ch]==127){
				pgm=bank127[pgm];
			}else if (pgm>126) pgm=126;
			if (envoff) break;
			if (ch==9) break;
			if (pgm==-1){
				env[ch]=envno[pgmtbl[11][0]];
				pgmvol[ch]=7;
				shift[ch]=0;
				pitcpx[ch]=0;
			}else{
				env[ch]=envno[pgmtbl[pgm][0]];
				pgmvol[ch]=pgmtbl[pgm][1];
				shift[ch]=pgmtbl[pgm][2];
				pitcpx[ch]=pgmtbl[pgm][3];
			}
			break;

		  case 0xd0:
			break;

		  case 0xe0: /* pitch bend */
			pbl=key;
			pbh=getc(fp);
			if (pbh<0) break;
			if (ch==9) break;
			pb=(pbh<<7)+pbl-8192;
			if (pb>=0) {
				pb=((long)(pb)*bendrange[ch]/4096+1)/2;
			}else{	
				pb=-(((long)(-pb)*bendrange[ch]/4096+1)/2);
			}
			if (pb!=pitbend[ch]){
				pitbend[ch]=pb;
				deltatime=realbend(deltatime,ch,pb);
			}
			break;

		  case 0xF0: /* meta event etc. */
			switch(ch){
			  case 0x0F:	/* 0xFF:metaevent */
				switch (key){
				  case 0x02:
				  case 0x03:
					size=ReadVarLen(fp);
					for (i=0;i<size;i++){
						if (i<255) strbuf[i]=getc(fp);
					}
					strbuf[min(size,255)]=0;
					printf("%s\n",strbuf);
					cnt=0;
					break;
				  case 0x2F: /* end of file */
					fseek(fp,ReadVarLen(fp),SEEK_CUR);
					endmark=1;
					break;
				  case 0x51: /* tempo set */
					if ((size=ReadVarLen(fp))==3){
						send(deltatime,0,127,32);
						deltatime=0;
						tempo=ReadSemiLong(fp);
						temposet(tempo,div);
					}else{
						fseek(fp,size,SEEK_CUR);
					}
					break;
				  default:
					fseek(fp,ReadVarLen(fp),SEEK_CUR);
					break;
				}
				break;
			  case 0x07:	; /* 0xF7 */
			  case 0x00:	; /* 0xF0 exclusive message */
				fseek(fp,-1,SEEK_CUR);
				lngs=ReadVarLen(fp);
				if (lngs == 0) break;
				if (d == 0xf7){
					lngs-=1;
					if (getc(fp)!=0xf0){
						fseek(fp,lngs,SEEK_CUR);
						break;
					}
				}
				if ((lngs==10)||(lngs==8)){
					for (i=0;i<lngs;i++){
						strbuf[i]=getc(fp);
					}
					if (strbuf[lngs-1]!=0xf7) break;
					if (lngs==8){
						if ((strbuf[0]==0x7f)&&(strbuf[1]==0x7f)&&
							(strbuf[2]==0x04)&&(strbuf[3]==0x01)){
							/* grobal master volume */
							mastervol=strbuf[5]&0x7f;
							deltatime=setmastervol(deltatime);
						}
					}else{
						if ((strbuf[0]!=0x41)||(strbuf[2]!=0x42)) break;
						if (strbuf[3]!=0x12) break;
						if (strbuf[4]!=0x40) break;
						if ((strbuf[5]==0x00)&&(strbuf[6]==0x04)){
							/* GS master volume */
							mastervol=strbuf[7]&0x7f;
							deltatime=setmastervol(deltatime);
						}else if (((strbuf[5]&0xf0)==0x10)&&(strbuf[6]==0x15)){
							/* GS drum part assign */
							i=strbuf[5]&0x0f;
							if (i==0){
								i=9;
							}else if (i<10){
								i--;
							}
							drumpart[i]=strbuf[7]&0x03;
						}
					}
				}else{
					fseek(fp,lngs,SEEK_CUR);
				}
				break;
			  default:
				fseek(fp,-1,SEEK_CUR);
			}
			break;
		}
		if (deltatime>=128){
			if (send(deltatime,0,127,32)!=0) return 3;
			deltatime=0;
		}
	}
	if (send(deltatime+16,7,127,63)!=0) return 3;
	if ( FP_OFF(datap) == 0) segn--;
	return 0;
}


/* make tone conversion table */

void maketonetbl(int intper){
	unsigned int	i;
	unsigned long	toneb[48];
	for (i=0;i<48;i++){
		toneb[i]=tonebase[i]/(basefreq/intper);
	}
	for (i=0;i<512;i++){
			settable(i,(unsigned int) (toneb[(i+keyadj*4)%48]>>(10-((i+keyadj*4)/48))));
	}
}


/* adjust system timer */

void	timeadjust(void){
	unsigned long tm,tm2;

	union {
		union REGS u;
		struct  {
			struct {
				unsigned int l: 4;
				unsigned int h: 4;
			} al,ah,bl,bh,cl,ch,dl,dh;
		} s;
	} r;
	
	do {
		r.u.h.ah = 0x02;
		int86(0x1A, &r.u, &r.u);
	} while(r.u.x.cflag != 0);
	
	tm  =  (unsigned long)(r.s.ch.h) * 36000 +
	       (unsigned long)(r.s.ch.l) * 3600 +
		   (unsigned long)(r.s.cl.h) *  600 +
		   (unsigned long)(r.s.cl.l) *   60 +
		   (unsigned long)(r.s.dh.h) *   10 +
		   (unsigned long)(r.s.dh.l);
	tm   = tm * 1147 / 63;

	tm2 = * (unsigned long far*)(0x0040006cL);
	*(unsigned long far*)(0x0040006cL)=tm;
	if (tm2>tm){
		*(unsigned char far*)(0x00400070L)=1;
		(*(unsigned int far*)(0x004000b2L))++;
	}
}


/* renew the volume of seq. data buf */

void optvol(int opvol){
	int	key,volm,ch,h,l,i,j;
	unsigned char far *ptr;
	for(i=0;i <= segn;i++){
		if (i<segnmax){
			ptr=(unsigned char far*)((unsigned long)(workseg[i])<<16);
		}else{
			fourpagemap(emmha,(i-segnmax)*4);
			ptr=(unsigned char far*)((unsigned long)(emmseg)<<16);
		}
		while(1){
			if (((*ptr)&0x80)!=0) ptr++;
			ptr++;
			l=*ptr;
			h=*(ptr+1);
			if ((h==0xff)&&(l==0xff)) break;
			if ((h!=0xff)||(l!=0x00)){
				key=h>>1;
				if (key>=4){
					volm=((h&1)<<5)+(l>>3);
					ch=l&7;
					if (volm!=0) volm=min((volm*opvol+160)/320,63);
					*ptr=(unsigned char)((volm<<3)+ch);
					*(ptr+1)=(unsigned char)((key<<1)+(volm>>5));
				}
			}
			ptr+=2;
		}
	}
}


/* main routione */

void main(int argc,char *argv[]){
	int	i,j,k,l,opt,result,volnum=0,notlx=0,revlev=2;
	int	helpflg=0,parerr=0,opterr=0,keywait=0,pkey,dspoff=0;
	int timadj=1,ispc98=0,intperiodchg=0,savelxmflg=0;
	unsigned int playadr,playseg;
	int	optlev=100;					/* optimized volume level (%) */
	FILE *fp;
	char filename[80];
	char filename2[80];
	printf("Beep MIDI Player LXMIDI ver 1.71\n");
	printf("Copyright (C) 1996,1997,1998 HARUYA  all right reserved.\n\n");

	if	(argc==1) helpflg=1;	/* no option */
	for	(opt=1;opt<argc;opt++){
		if ((argv[opt][0]!='-')&&(argv[opt][0]!='/')){
			if (opt!=argc-1) parerr=1;
		}else{
			if (opt==argc-1){
				parerr=1;
				break;
			}
			switch (toupper(argv[opt][1])){
			  case 'A':
				optlev=(unsigned)atol(&argv[opt][2]);
				if ((optlev<10)||(optlev>200)) parerr=1;
				break;
			  case 'C':
				intperiodchg=(unsigned)atol(&argv[opt][2]);
				if ((intperiodchg<40)||(intperiodchg>255)) parerr=1;
				break;
			  case 'K':
				keywait=1;
				break;
			  case 'S':
				useems=1;
				break;
			  case 'W':
				savelxmflg=1;
				break;
			  case 'X':
				dspoff=1;
				break;
			  case 'O':
				portmax=(unsigned)atol(&argv[opt][2])-1;
				if ((portmax!=3)&&(portmax!=7)&&(portmax!=11)&&(portmax!=15)) parerr=1;
				break;
			  case 'R':
				keyadj=(unsigned)atol(&argv[opt][2]);
				if ((keyadj<-24)||(keyadj>24)) parerr=1;
				break;
			  case 'E':
				revlev=(unsigned)atol(&argv[opt][2]);
				if ((revlev<1)||(revlev>6)) parerr=1;
				break;
			  case 'T':
				tmpadj=(unsigned)atol(&argv[opt][2]);
				if ((tmpadj<10)||(tmpadj>1000)) parerr=1;
				break;
			  case 'P':
				envoff=1;
				for (i=0;i<16;i++){
					env[i]=ENV9;	/* flat envelope */
				}
				break;
			  case 'V':
				volnum=(unsigned)atol(&argv[opt][2]);
				if ((volnum<0)||(volnum>100)) parerr=1;
				break;
			  case 'D':
				drumadj=(unsigned)atol(&argv[opt][2]);
				if ((drumadj<0)||(drumadj>400)) parerr=1;
				break;
			  case 'N':
				switch (atol(&argv[opt][2])){
				  case  9:		/* for PC-98 */
					ispc98=1;
					timadj=0;
					notlx=1;
					if ((*(char far *)(0x00000501L))&0x80){
						basefreq=1996800;
						intperiod=90;
					}else{
						basefreq=2457600;
						intperiod=111;
					}
					break;
				  case	4:		/* for 40MHz LX */
					basefreq=1506847;
					intperiod=70;
					break;
				  case	3:		/* for 32MHz LX */
					basefreq=1205480;
					intperiod=64;
					break;
				  case	2:		/* for IBM-PC (Clock adjust disable */
				    timadj=0;
				  case  5:		/* for IBMPC (Clock adjust enable) */
					notlx=1;
				  case	1:		/* for 31MHz & 15MHz(Normal) LX */
					basefreq=1193180;
					intperiod=64;
					break;
				  case	0:		/* for 36MHz LX */
					break;
				  default:
					parerr=1;
				}
				break;
			  case 'M':
				for(i=0;i<16;i++){
					mask[i]=' ';
				}
				i=2;
				while((j=toupper(argv[opt][i]))!=0){
					if ((j<'0')||(j>'F')) parerr=1;
					if ((j>'9')&&(j<'A')) parerr=1;
					if (j>='A') j=j-'A'+'9'+1;
					j=(j-'0')&0x0f;
					mask[j]='M';
					i++;
				}
				break;
			  default:
				opterr=1;
			}
		}

	}
	opt--;
	if (opterr!=0){
		printf("Illegal option!\n");
		helpflg=1;
	}
	if (parerr!=0){
		printf("Illegal option parameter!\n");
		helpflg=1;
	}
	if (helpflg!=0){
		printf("Usage:LXMIDI [option(s)] filename[.MID]\n");
		printf("options:\n");
		printf("  -N[0|1|2|9]    machine type  0(default):LX(x2.27) 1:LX(x2) 2:IBMPC 9:PC98\n");
		printf("  -C[40 ~ 255]   interrupt cycle (default:95(-N0) 64(-N1,-N2) 90/111(-N9))\n");
		printf("  -D[0 ~ 400]    percussion volume ratio(%)\n");
		printf("  -E[1 ~ 6]      reverb level (-6*n dB)\n");
		printf("  -K             wait keyin before playing\n");
		printf("  -M[012...DEF]  mask MIDI channel\n");
		printf("  -O[4|8|12|16]  number of maximum polyphony \n");
		printf("  -P             ignore MIDI program change \n");
		printf("  -R[-24 ~ 24]   transpose (unit:half tone)\n");
		printf("  -S             use EMS memory \n");
		printf("  -T[10 ~ 1000]  adjust tempo(%)\n");
		printf("  -V[0 ~ 100]    volume (0:auto optimize)\n");
		printf("  -A[10 ~ 200]   volume ratio (%) (valid in auto optimize mode)\n");
		printf("  -W             write LXM object file \n");
		printf("  -X             turn off LCD display \n");
		printf("\ndefault option:-N0 -O12 -C95 -D100 -E2 -R0 -T100 -V0 -A100\n");
		exit(-1);
	}
	
	if (intperiodchg) intperiod=intperiodchg;
	
	if (notlx) dspoff=0;
	i=0;
	k=1;
	while (((j=argv[opt][i])!=0)&&(i<70)){
		filename[i++]=j;
		if (j=='.'){
			k=0;
		}
	}
	if (k){
		filename[i++]='.';
		filename[i++]='M';
		filename[i++]='I';
		filename[i++]='D';
	}
	filename[i]=0;

	i=0;
	while (((j=argv[opt][i])!=0)&&(i<70)){
		if (j=='.') break;
		filename2[i++]=j;
	}
	filename2[i++]='.';
	filename2[i++]='L';
	filename2[i++]='X';
	filename2[i++]='M';
	filename2[i]=0;



	if ((fp=fopen(filename,"rb"))==NULL){
		printf("Can't open %s\n",filename);
		exit(-1);
	}
	
	emsmaxblock=0;
	segnmax=0;
	if (useems){
		if (chkemm()!=-1){
			emmseg=getseg();
			getems();
			if (emsmaxblock > 0){
				printf("%dkbytes EMS memory were allocated.\n",64*emsmaxblock);
			}
		}else{
			printf ("EMM driver not found\n");
		}
	}
	getmem();
	if (segnmax > 0){
		printf("%dkbytes conventional memory were allocated.\n",64*segnmax);
	}
	if ((segnmax+emsmaxblock)==0){
		printf("Insufficient memory\n");
		exit(-1);
	}

	printf("Now loading & analyzing %s...\n",filename);
	
	if (volnum==0){
		vol=32;
	}else{
		vol=(long)(intperiod)*volnum/100;
	}
	lfo=(basefreq/intperiod)/128/7;

	segn=0;
	if (segn < segnmax){
		datap=(unsigned char far*)((unsigned long)(workseg[0])<<16);
	}else{
		fourpagemap(emmha,(segn-segnmax)*4);
		datap=(unsigned char far*)((unsigned long)(emmseg)<<16);
	}
	result=smfconv(fp);
	fclose(fp);

	switch(result){
		case 0:
			printf("\nMIDI data analysis completed.\n");
			printf("Internal data size:%lu bytes.\n",(unsigned long)(segn)*65523+FP_OFF(datap));
			if (volnum==0){
				printf("Optimizing volume now...");
				k=1<<revlev;
				l=min( ((long)(intperiod)*320*k*optlev)/((long)(k+1)*peakvol*100),630);
				optvol(l);
				printf("done! The optimized volume is %d.\n",l*10/intperiod);
			}
			if (savelxmflg){
				if ((fp=fopen(filename2,"wb"))==NULL){
					printf("Can't create %s\n",filename2);
					break;
				}
				printf("Now writing object file %s.\n",filename2);
				savelxm(fp);
				fclose(fp);
				break;
			}
		case 100:
			if (keywait){
				printf("Push any key to start playing.\r");
				while(mykbhit()==0);
				mygetch();
			}
			printf("Start playing...\n");
			maketonetbl(intperiod);
			if (ispc98) pc98patch();
			playadr=0x0000;
			i=0;
			while(1){
				printf("[ALT]:Pause [SHIFT]:Feed Forward [CTRL]:Stop\r");
				if (dspoff) displayoff();
				if (ispc98){
					pcmstart98(intperiod,portmax+1);
				}else{
					pcmstart(intperiod,notlx,portmax+1);
				}
				while(i <= segn){
					if (i < segnmax){
						playseg=workseg[i];
					}else{
						fourpagemap(emmha,(i-segnmax)*4);
						playseg=emmseg;
					}
					if ((pkey=playseq(&playadr,playseg,revlev))!=0) break;
					playadr=0x0000;
					i++;
				}
				waitempty();
				if (ispc98){
					pcmstop98();
				}else{
					pcmstop(notlx);
				}
				if (dspoff) displayon();
				if ((pkey&8)==0) break;
				while(mykbhit())	mygetch();
				printf("Paused.  Hit any key to restart playing!       \r");
				while(mykbhit()==0);
				mygetch();
			}
			printf("\n");
			if (timadj) timeadjust();
			break;
		case 1:
			printf("Not standard MIDI file!\n");
			break;
		case 2:
			printf("Not format0 MIDI file!  Please convert to format0.\n");
			break;
		case 3:
			printf("Memory full!\n");
			break;
		case 4:
			break;
		case 5:
			printf("Broken file! (EOF runaway?)\n");
			break;
		}
	relmem();
	exit(result);
}

