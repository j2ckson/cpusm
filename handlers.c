#include "./handlers.h"


void printcc(char* term, int colour, int basecolour)
{
	if ( basecolour != 0 ) {
		if ( colour == 1 ) printf(ILOADLOW);
		if ( colour == 2 ) printf(CLOADLOW);
		if ( colour == 3 ) printf(VALMAX);
	}
	printf("%s", term);
	return;
}
double seconds_handler(int dhm[], double seconds)
{
	dhm[0] = (int)seconds/86400;
	seconds = fmod(seconds, 86400);
	dhm[1] = (int)seconds/3600;
	seconds = fmod(seconds, 3600);
	dhm[2] = (int)seconds/60;
	return fmod(seconds, 60);
}
int rng(const int nMin, const int nMax, const int  nN)
{
	int nR = 0;
	for (int i = 0; i < nN; i++) {
		nR = rand()%(nMax-nMin) + nMin;
	}
	return nR;
}
double derive_interval(int qual, int rate, double secdecs, int *slot, int *scroll)
{
	double decvar = 0;
//	double cnttime = 0;
	if ( qual == 1 && *slot != 11) {
		cnttime = get_time_nsec();
		if ( *slot == 19 && *scroll != 0 ) {
			exptime += (rate*secdecs*NSECS*HSLEEP);
			exptime += NSECS * secdecs;
			*slot = 1;
		} else {
			exptime += NSECS * secdecs;
		}
		if (cnttime < exptime && *slot != 2) {
			decvar = (exptime - cnttime);
			decvar /= NSECS;
			*slot=*slot==4?5:4;
		} else if (cnttime > exptime && *slot != 4) {
			if ( cnttime - exptime <= secdecs  || *slot ==1 ) {
				decvar = cnttime - exptime;
				decvar /= NSECS;
				*slot=*slot==2?3:2;
			}
		} else {
			decvar = secdecs;
			*slot=7;
		}
	} else {
		decvar = secdecs;
		*slot = *slot==11?0:*slot;
	}
	return decvar;
}
void sig_handler(int signo)
{
	if (signo == SIGINT) {
		return;
	} else if (signo == SIGTRAP) {
		return;
	} else if (signo == SIGQUIT) {
		return;
	}
	return;
}
char *getNetDev(void)
{
	char netdevcmd[75];
	char *devdev;
	devdev = NULL;
	strcpy(netdevcmd, DEFDEV);
	FILE *gndp = popen(netdevcmd, "r");
	size_t linesiz=0;
	getline(&devdev, &linesiz, gndp);
	pclose(gndp);
	return strtok(devdev, "\n");
}
int termsize (int colour)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if ( colour != 0 ) printf(CLOADLOW);
    printf ("terminal geometry: %i rows x %i columns\n", w.ws_row, w.ws_col);
    return w.ws_col;
}
static void old_attr(void)
{
	tcsetattr(0, TCSANOW, &g_old_kbd_mode);
	return;
}
static void keyboard_attr(void)
{
	struct termios new_kbd_mode;
	tcgetattr(0, &g_old_kbd_mode);
	memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
	new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
	new_kbd_mode.c_cc[VTIME] = 0;
	new_kbd_mode.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_kbd_mode);
	atexit(old_attr);
	return;
}
static cnt_nsec_t get_time_nsec(void)
{
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	return NSECS *(double)tstart.tv_sec + tstart.tv_nsec;
}
int getfreqlimits( double *freqmin, double *freqmax, double *maxfreq)
{
	int erresult = 0;
	char freqcmd[128];
	*freqmin = *freqmax = *maxfreq = 0;
	strcpy(freqcmd, FREQ);
	FILE *fp;
	fp = popen(freqcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			sscanf(linebuf, "%lf,%lf", &*freqmin, &*freqmax);
			*maxfreq = *freqmax;
		}
		free(linebuf);
		linebuf=NULL;
	}
	pclose(fp);
	return erresult;
}
int getsystype(timeD *xx)
{
	int erresult = 0;
	char sysidcmd[128];
	char sysid[128];
	char na[4] = "n/a";
	xx->sysid = strdup(na);
	strcpy(sysidcmd, ID);
	FILE *fp;
	fp = popen(sysidcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			if (strcmp(strtok(linebuf, " \n"), "Linux") == 0) {
				strcpy(sysid, "linux");
			}
			if (strcmp(strtok(linebuf, " \n"), "Darwin") == 0) {
				strcpy(sysid, "mac-os");
			}
			xx->sysid = strdup(sysid);
		}
		free(linebuf);
		linebuf=NULL;
	}else{
		xx->sysid = strdup(na);
	}
	pclose(fp);
	return erresult;
}
int getmem(int *mmstat)
{
	int erresult = 0;
	if( access(MSTAT, F_OK ) != -1 ) {
		FILE *fp;
		fp = fopen(MSTAT, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				strtok(linebuf," ");
				*mmstat = atoi(strtok(NULL, " "));
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			*mmstat = -1;
		}
		fclose(fp);
	}else{
		*mmstat = -1;
	}
	return erresult;
}
int gethdd(timeD *xx, cpuZ *g)
{
	int erresult = 0;
	char hddcmd[61];
	char na[4] = "n/a";
	xx->diskcnt = g->diskcnt = 0;
	xx->disk[0] = strdup(na);
	strcpy(hddcmd, DEFDSK);
	FILE *fp;
	fp = popen(hddcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		int az = 0;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		ssize_t linelen=0;
		while ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
			sprintf(g->disk[az], "%s", strtok(linebuf, " \n"));
			xx->disk[az] = strdup(g->disk[az]);
			az++;
		}
		xx->diskcnt = g->diskcnt = az;
		free(linebuf);
		linebuf=NULL;
	}else{
		xx->diskcnt = g->diskcnt = 0;
		xx->disk[0] = strdup(na);
	}
	pclose(fp);
	return erresult;
}
int getcpuonline(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char cpuol[32];
	char na[4] = "n/a";
	if( access(AFFCPU, F_OK ) != -1 ) {
		fp = fopen(AFFCPU, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(cpuol, "%s", (strtok(linebuf, "\n")));
				xx->cpuol = strdup(cpuol);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->cpuol = strdup(na);
		}
		fclose(fp);
	}else{
		xx->cpuol = strdup(na);
	}
	return erresult;
}
int getsysversion(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char version[128];
	char na[4] = "n/a";
	if( access(VER, F_OK ) != -1 ) {
		fp = fopen(VER, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(version, "%s", (strtok(linebuf, " ")));
				for (int i=0; i<2; i++) {
					sprintf(version, "%s", (strtok(NULL, " ")));
				}
				xx->version = strdup(version);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->version = strdup(na);
		}
		fclose(fp);
	}else{
		xx->version = strdup(na);
	}
	return erresult;
}
int getsysname(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char uname[255];
	char na[4] = "n/a";
	if( access(HN, F_OK ) != -1 ) {
		fp = fopen(HN, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(uname, "%s", (strtok(linebuf, "\n")));
				xx->uname = strdup(uname);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->uname = strdup(na);
		}
		fclose(fp);
	}else{
		xx->uname = strdup(na);
	}
	return erresult;
}
int getsysfreqgov(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char freqgov[255];
	char na[4] = "n/a";
	if( access(GOV, F_OK ) != -1 ) {
		fp = fopen(GOV, "r");
		if ( fp != NULL ) {
			erresult = 1;	
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			if ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sscanf(linebuf, "%s", freqgov);
				xx->freqgov = strdup(freqgov);
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->freqgov = strdup(na);
		}
		fclose(fp);
	}else{
		xx->freqgov = strdup(na);
	}
	return erresult;
}
int getsysmodel(timeD *xx)
{
	int erresult = 0;
	FILE *fp;
	char model[2][255];
	char na[4] = "n/a";
	if( access(CPUI, F_OK ) != -1 ) {
		fp = fopen(CPUI, "r");
		if ( fp != NULL ) {
			erresult = 1;
			size_t linesiz=0;
			char* linebuf;
			linebuf=NULL;
			ssize_t linelen=0;
			while ( (linelen=getline(&linebuf, &linesiz, fp)) > 0 ) {
				sprintf(model[0], "%s", (strtok(linebuf, ":")));
				sprintf(model[1], "%s", (strtok(NULL, ":\n")));
				if (strcmp(model[0], "model name\t") == 0) {
					xx->model = strdup(model[1]);
					break;
				}
			}
			free(linebuf);
			linebuf=NULL;
		}else{
			xx->model = strdup(na);
		}
		fclose(fp);
	}else{
		xx->model = strdup(na);
	}
	return erresult;
}
int getthermalzones(int *gtz, int *vtz, int *ptz)
{
	int erresult = 0;
	FILE *fp;
	char thermalcmd[64];
	strcpy(thermalcmd, TEMPNUM);
	*gtz = *vtz = *ptz = 0;
	fp = popen(thermalcmd, "r");
	if ( fp != NULL ) {
		erresult = 1;
		size_t linesiz=0;
		char* linebuf;
		linebuf=NULL;
		getline(&linebuf, &linesiz, fp);
		*gtz = *vtz = *ptz = atoi(linebuf);
		free(linebuf);
		linebuf=NULL;
	}else{
		*gtz = *vtz = *ptz = -1;
	}
	pclose(fp);
	return erresult;
}
void check_60(void * t, double secdecs)
{
	timeZ *h;
	h=(timeZ*)t;
	int zeczint;
	char secheck[7];
	sprintf(secheck, "%.0lf", h->idurs);
	zeczint = atoi(secheck);
	if ( zeczint == 60 ) {
		h->idurm++;
		if ( h->idurm == 60 ) {
			h->idurh++;
			h->idurm = 0;
		}
		h->idurs = 0;
	}
	return;
}
int set_ppid(int colour)
{
	pid_t ppid;
	ppid = getpid();
	//~ FILE *fp = fopen(PPMYD, "w");
	//~ if ( colour != 0 ) printf(CLOADLOW);
	//~ if ( fp != NULL) {
		//~ fprintf (fp, "%i\n", ppid);
		//~ printf("\nprocess %i written to %s\n", ppid, PPMYD);
	//~ } else {
		//~ printf("\nprocess %i: write failure to %s\n", ppid, PPMYD);
	//~ }
	//~ fclose(fp);
	return ppid;
}
int set_priority(int sched[])
{
	int setret[3] = {};
	int getret[3] = {};
	int process = PRIO_PROCESS;
	struct sched_param param ={0};
	param.sched_priority = sched[2];
	setret[0] = setpriority(process, sched[0], sched[1]);
	if ( setret[0] == -1 ) printf("\nfailed to set process priority %i\n", sched[2]);
	setret[1] = sched_setscheduler( 0, sched[3], &param );
	setret[2] = sched_setparam( 0, &param );
	getret[1] = sched_getscheduler(sched[0]);
	getret[2] = sched_getparam(sched[0], &param);
	sched[3] = getret[1];
	sched[2] = param.sched_priority;
	printf("scheduler: %i priority: %i\n", getret[1], param.sched_priority);
	return getret[0];
}
int set_cpuAffinity(int cpuN, int rcnt)
{
	uint32_t i = 0;
	uint8_t flag;
	cpu_set_t mask;
	CPU_ZERO( &mask );
	CPU_SET( cpuN, &mask );
	if( sched_setaffinity( 0, sizeof(mask), &mask ) == 0 ) {
		cpu_set_t mycpuid;
		sched_getaffinity(0, sizeof(mycpuid), &mycpuid);
	    if ( rcnt == 0 ) printf("core affinity configured - application pinned to core ");
	    flag = 0;
	    for (i = 0; i < sizeof(cpu_set_t); i ++) {
	        if (CPU_ISSET(i, &mycpuid)) {
	            flag=flag==0?1:0;
	                if ( flag == 1 && rcnt == 0 ) printf("%ui", i);
	                if ( flag == 0 && rcnt == 0 ) printf(",%ui", i);
	        }
	    }
	}
    if ( rcnt == 0 ) printf("\n");
    return i;
}
char *commaprint(unsigned long long n)
{
	char *p;
	static int comma = '\0';
	static char retbuf[30];
	p = &retbuf[sizeof(retbuf)-1];
	int i = 0;
	if(comma == '\0') {
	  struct lconv *lcp = localeconv();
	  if(lcp != NULL) {
		   if(lcp->thousands_sep != NULL &&
			    *lcp->thousands_sep != '\0')
			    comma = *lcp->thousands_sep;
		   else    comma = ',';
	  }
	}
	*p = '\0';
	while (n != 0) {
	  if(i%3 == 0 && i != 0) {
		   *--p = comma;
	  }
	  *--p = '0' + n % 10;
	  n /= 10;
	  i++;
	}
	return p;
}

