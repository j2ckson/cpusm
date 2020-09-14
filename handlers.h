#ifndef HANDLERS_H
#define HANDLERS_H

FILE *fo;

typedef unsigned long long cnt_nsec_t;
cnt_nsec_t exptime, cnttime;
pthread_mutex_t lock;
struct timespec tstart= {0,0}, tend= {0,0};

typedef struct timeZ
{
	int idurm, idurh, idurd, idurms, idurhs, idurds;
	double idurs;
	double idurss;
	double iduri;
} timeZ;

typedef struct cpuZ
{
	char ch;
	char disk[8][255];
	char *ndev;
	double cStat[32];
	double vStat[65];
	double fStat[65];
	double tStat[65];
	double loadavg[3];
	int cndcnt, ndfcnt; 
	int cpucorecnt;
	double zecdecz;
	double freqs[9];
	double ctemp;
	double dekvar;
	int rollintval;
	int flop;
	int diskcnt;
	int netdiz;
	int verb;
	int multistat;
	int slip;
	int optalg;
	int core;
	int thermalzones;
	double memstat[5];
	int scroll;
	char procsS[9];
	double freqmax;
} cpuZ;

typedef struct timeD
{
    char appstart[255], dev[255];
	double cpuspin;
	int samples, sig, scroll, netdis, colour, ppid, multistat, cpun, dsize;
	double ttstart;
	int slat, step;
	int diskcnt;
	int corecnt;
	double idur, idurv;
	double minfreq, maxfreq;
	char *sysid;
	char *disk[8];
	char *cpuol;
	char *model;
	char *version;
	char *uname;
	char *freqgov;
	int rcnt;
	int mmstat, rate, actlog;
	double secdecs, cntsum;
} timeD;

typedef struct prntsctrl
{
	int scnt, sthis, sthat;
	int colour;
	int idurh;
	int idurm;
	int focus;
	double idurs;
	double zecdecz;
	double gen_stat[4][16];
	char runtime[255];
} prntsctrl;

typedef struct psv0
{
	int verb, sthis, sthat;
	int type;
	int colour;
	int core;
	int corecnt;
	int stat_count;
	int counter;
	int focus;
	int multistat;
	int thermalzones;
	double stat[192];
	double max[5][64];
	double min[5][64];
	double mean[5][64];
	double threshold;
	
} psv0;

typedef struct psheader
{
	int roll;
	int xcnt;
	int netdis;
	int colour;
	int focus;
	int pri;
	int ppid;
	int nice;
	int sched;
	int cpun;
	int verb;
	int corecnt;
	int stat_count;
	int scroll;
	int thermalzones;
	int multistat;
	char *dev;
} psheader;

void printcc(char* term, int colour, int basecolour);
double seconds_handler(int dhm[], double seconds);
int rng(const int nMin, const int nMax, const int  nN);
double derive_interval(int qual, int rate, double secdecs, int *slot, int *scroll);
void sig_handler(int signo);
char *getNetDev(void);
int termsize (int colour);
static struct termios g_old_kbd_mode;
int set_ppid(int colour);
int set_priority(int sched[]);
int set_cpuAffinity(int cpuN, int rcnt);
void check_60(void * t, double secdecs);
static cnt_nsec_t get_time_nsec(void);
static void old_attr(void);
static void keyboard_attr(void);
int getfreqlimits( double *freqmin, double *freqmax, double *maxfreq);
int getsystype(timeD *xx);
int getmem(int *mmstat);
int gethdd(timeD *xx, cpuZ *g);
int getthermalzones(int *gtz, int *vtz, int *ptz);
int getcpuonline(timeD *xx);
int getsysmodel(timeD *xx);
int getsysversion(timeD *xx);
int getsysname(timeD *xx);
int getsysfreqgov(timeD *xx);
char *commaprint(unsigned long long n);
void timeFormat(char runtime[], double dursofar);

#endif
