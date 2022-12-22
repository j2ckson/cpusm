#include "./oneKeyAction.h"

void *one_key_actionT(void * q)
{
	cpuZ *h;
	h=(cpuZ*)q;
	int slap = 0;
	double interval = h->dekvar;
	int hslap = rint(h->slip/interval);
	fd_set readfds;
	int fd_stdin = (fileno(stdin));
	FD_ZERO(&readfds);
	FD_SET(fileno(stdin), &readfds);
	struct timespec pseudoSleeps = {0,0};
	while (1) {
		pseudoSleeps.tv_sec = h->dekvar/1;
		pseudoSleeps.tv_nsec = fmod(h->dekvar, 1) * NSECS;
		if ( slap == 0 ) {
			if( access(MSTAT, F_OK ) != -1 ) {
				FILE *memF;
				memF = fopen(MSTAT, "r");
				for(int mm=0; mm<5; mm++) {
					fscanf( memF, MEM_STRING, &h->memstat[mm] );
				}
				fclose(memF);
			}
		}
		slap++;
		slap=slap==hslap?0:slap;
		if( access(LDAVG, F_OK ) != -1 ) {
			FILE *loadF;
			loadF = fopen(LDAVG, "r");
			fscanf( loadF, LOAD_STRING, &h->loadavg[0], &h->loadavg[1], &h->loadavg[2], h->procsS );
			fclose(loadF);
		}
		if ( h->thermalzones != 0 ) {
			char thermalcmd[64];
			double tempT = 0;
			FILE *fp;
			strcpy(thermalcmd, TEMPQUERY);
			fp = popen(thermalcmd, "r");
			if ( fp != NULL ) {
				size_t linesiz=0;
				char* linebuf;
				linebuf=NULL;
				for ( int k=0;k<h->thermalzones;k++ ) {
					getline(&linebuf, &linesiz, fp);
					double temptemp = atof(linebuf);
					temptemp /= 1000L;
					tempT += temptemp;
					if ( h->multistat == 1 || ( h->multistat == 0 && h->verb == 3 ) ) h->tStat[k] = temptemp;
				}
				h->ctemp = tempT/(double)h->thermalzones;
				pclose(fp);
				free(linebuf);
				linebuf=NULL;
			}
		}else{
			h->ctemp = 0;
		}
		if ( h->freqmax != 0 ) {
			if( access(CPUI, F_OK ) != -1 ) {
				double freqA = 0;
				FILE *freqF;
				for ( int k=0;k<h->cpucorecnt;k++ ) {
					double freqT = 0;
					char ffilename[128];
					sprintf(ffilename, "%s%d%s", "/sys/devices/system/cpu/cpu", k, "/cpufreq/scaling_cur_freq");
					freqF = fopen(ffilename, "r");
					size_t linesiz=0;
					char* linebuf;
					linebuf=NULL;
					getline(&linebuf, &linesiz, freqF);
					sscanf( linebuf, "%lf", &freqT );
					h->fStat[k] = freqT/1000;
					freqA += h->fStat[k];
					fclose(freqF);
					free(linebuf);
					linebuf=NULL;
				}
				freqA /= h->cpucorecnt;
				h->fStat[h->cpucorecnt] = freqA;
			}
		}else{
			for ( int k=0;k<h->cpucorecnt;k++ ) {
				h->fStat[k] = 0;
			}
			h->fStat[h->cpucorecnt] = 0;
		}
		for ( int i=0;i<h->slip;i++ ) {
			FD_SET(fileno(stdin), &readfds);
			int srett = pselect(fd_stdin + 1, &readfds, NULL, NULL, &pseudoSleeps, NULL);
			if ( srett > 0 ) {
				char cch = getchar();
				h->ch = cch;
			}
			if ( h->flop != 0 ) break;
		}
		if ( h->flop != 0 ) break;
	}
	pthread_exit(NULL);
	return 0;
}
