#include "./dmtp.h"

int dmtp(char *outputfile, char *appstart, char *dev, int em[], double dur[])
{
	int scanning = 0;
	int sig = 0;
	int x = 0, y = 0;
	int xcnt = 0;
	int scsw = 0;
	int slat = 0;
	int pdiv = em[11];
	int slot = 0;
	int step = 1;
	int stepper = em[21];
	int jay = stepper==0?0:2;
	int jaystep = stepper!=0?1:0;
	int stephold = 1;
	int runtimetype = 0;
	int runtimecounttype = 0;
	double idur1 = 0L;
	double cntsum = 0L;
	size_t yards = 247;
	char *xy_string = (char *) malloc (yards);
	pthread_t ssid[6];
	timeD *xx = (timeD*)malloc( sizeof(timeD));
	prntsctrl *psc = (prntsctrl*) malloc( sizeof(prntsctrl));
	psheader *psh = (psheader*) malloc( sizeof(psheader));
	psv0 *vv0 = (psv0*)malloc( sizeof(psv0));
	cpuZ *g = (cpuZ*)malloc( sizeof(cpuZ));
	dur[0] = 0L;
	xx->colour = em[3];
	xx->cpun = em[7];
	xx->samples = em[1];
	xx->sig = sig;
	xx->netdis = em[2];
	xx->corecnt = em[11];
	xx->idurv = xx->idur = dur[1];
	xx->rate = em[15];
	xx->actlog = em[16];
	xx->secdecs = dur[2];
	xx->rcnt = 0;
	xx->slat = slat;
	xx->ppid = em[6];
	xx->step = em[22];
	xx->dsize = em[27];
	sprintf(xx->appstart, "%s", appstart);
	sprintf(xx->dev, "%s", dev);
	xx->cntsum = 0;
	g->cpucorecnt = (double)em[11];
	g->rollintval = (double)em[19];
	if ( dur[3] == 0 ) g->zecdecz = dur[2];
	if ( dur[3] != 0 ) g->zecdecz = dur[2]*dur[3];
	g->netdiz = em[2];
	g->ndev = strdup(dev);
	g->verb = psh->verb = em[10];
	g->multistat = vv0->multistat = xx->multistat = psh->multistat = em[14];
	g->slip = em[20];
	g->optalg = em[8];
	g->scroll = xx->scroll = em[13];
	psc->scnt = 0;
	psc->sthis = vv0->sthis = 0;
	psc->sthat = vv0->sthat = 1;
	psc->colour = 0;
	psc->focus = psh->focus = vv0->focus = em[23];
	g->ctemp = 0;
	g->flop  = 0;
	arrayInit(g->cStat, 32);
	arrayInit(g->vStat, 64);
	arrayInit(g->fStat, 65);
	arrayInit(vv0->stat, 192);
	arrayInit(g->freqs, 9);
	arrayInit(g->memstat, 5);
	arrayInit(g->loadavg, 3);
	for ( int i=0;i<5;i++ ) {
		arrayInit(vv0->min[i], 64);
		arrayInit(vv0->max[i], 64);
		arrayInit(vv0->mean[i], 64);
	}
	for ( int i=0;i<4;i++ ) {
		arrayInit(psc->gen_stat[i], 16);
	}
	char procsS[9] = "";
	psh->roll = em[19];
	psh->netdis = em[2];
	psh->nice = em[5];
	psh->ppid = em[6];
	psh->pri = em[24];
	psh->sched = em[25];
	psh->corecnt = em[11];
	psh->dev = strdup(dev);
	vv0->counter = 1;
	memcpy(g->procsS, procsS, sizeof(procsS));
	g->ch = 0;
	if ( em[8] != 0 ) {
		g->cpucorecnt = em[11];
		g->optalg = em[8];
	}
	int fresult = 0;
	fresult = getfreqlimits( &xx->minfreq, &xx->maxfreq, &g->freqmax);
		if ( fresult == 0 ) printf("failed to query cpu frequency limits\n");
	fresult = getthermalzones( &g->thermalzones, &vv0->thermalzones, &psh->thermalzones);
		if ( fresult == 0 ) printf("failed to query thermal monitoring framework\n");
	fresult = getmem( &xx->mmstat);
		if ( fresult == 0 ) printf("failed to query system memory\n");
	fresult = getsystype(xx);
		if ( fresult == 0 ) printf("failed to query system type\n");
	fresult = getcpuonline(xx);
		if ( fresult == 0 ) printf("failed to query active cpu status\n");
	fresult = getsysmodel(xx);
		if ( fresult == 0 ) printf("failed to query cpu model\n");
	fresult = getsysversion(xx);
		if ( fresult == 0 ) printf("failed to query o/s version\n");
	fresult = getsysname(xx);
		if ( fresult == 0 ) printf("failed to query system hostname\n");
	fresult = getsysfreqgov(xx);
		if ( fresult == 0 ) printf("failed to query cpu frequency governor\n");
	fresult = gethdd(xx, g);
		if ( fresult == 0 ) printf("failed to query active hdd status\n");
	keyboard_attr();
	exptime = get_time_nsec();
	clock_gettime(CLOCK_MONOTONIC, &tstart);
	double ttstart = (double)tstart.tv_sec + NSECS2SECS*tstart.tv_nsec;
	xx->ttstart = ttstart;
	double cnt1 = ((double)tstart.tv_sec + NSECS2SECS*tstart.tv_nsec);
	struct timespec pseudoSleep = {0,0};
	time_diff(*xx, em[12], em[26], &xy_string);
	clock_t exetime0 = clock();
	if ( em[10] == 0 || em[10] == 1 ) psh->stat_count = 8;
	if ( em[10] == 2 || em[10] == 4 ) psh->stat_count = em[11];
	if ( em[10] == 3 ) psh->stat_count = psh->thermalzones;
	if ( em[16] !=2 ) {
		psh->colour = em[3];
		psh->cpun = em[7];
		psh->verb = em[10];
		psh->scroll = em[13];				
		if ( em[27] == 0 ) print_header(*psh);
		if ( em[27] == 1 ) print_header_s(*psh);
	}
		pthread_create(&ssid[0], NULL, &one_key_actionT, (void *) g);
		if ( em[14] == 1 ) pthread_create(&ssid[1], NULL, &cpustatsT, (void *) g);
		if ( em[14] == 0 ) {
			if ( em[10] == 0 ) pthread_create(&ssid[1], NULL, &justcpustatsT, (void *) g);
			if ( em[10] == 1 ) pthread_create(&ssid[1], NULL, &justnetstatsT, (void *) g);
			if ( em[10] == 2 ) pthread_create(&ssid[1], NULL, &justvpustatsT, (void *) g);
			if ( em[10] == 3 ) pthread_create(&ssid[1], NULL, &simplecpustatsT, (void *) g);
			if ( em[10] == 4 ) pthread_create(&ssid[1], NULL, &simplecpustatsT, (void *) g);
		}
	while ((em[1]!=0)?(em[12]<em[1]):((dur[1]!=0)?(em[12]<dur[1]/(dur[2])):(1))) {
		double decvar = derive_interval(em[4], em[15], dur[2], &slot, &em[13]);
		if ( dur[3] == 0 ) g->dekvar = decvar;
		if ( dur[3] != 0 ) g->dekvar = decvar*dur[3];
		if ( slot == 19 ) slot = 0;
		if ( decvar != 0 ) {
			pseudoSleep.tv_sec = decvar/1;
			pseudoSleep.tv_nsec = fmod(decvar, 1) * NSECS;
			pselect(0, NULL, NULL, NULL, &pseudoSleep, NULL);
			if ( em[12] >= 1 ) {
				if ( g->ch != 0 ) {
					if ( g->ch == '/' ) {
						em[13] = (em[13]==0)?(1):(0);
						if ( em[13] == 1 ) {
							if ( em[21] != 0 ) {
								stephold = em[21];
							}
						}else {
							if ( jay >= 2 ) {
								if ( stepper != 0 ) {
									if ( jaystep !=0 ) em[21] = stephold;
									step = em[12]%(em[21]*em[15]);
								}
							}
						}
						if ( em[13] == 1 && em[17] == 0 ) x=10;
						if ( em[13] == 1 && em[17] != 0 ) xcnt = 0;
						if ( em[13] == 0 && em[17] == 0 ) scsw=1;
						if ( em[13] == 0 && em[17] != 0 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
							if ( em[21] != 0 && xcnt % (em[17]*em[15]) == 0 ) printf("\x1b[1A");
						}
						if ( em[21] != 0 ) step = em[12]%(em[19]*em[15]);
					}
					if ( g->ch == 'j' ) {
						if ( jay == 0 ) {
							jay = 2;
						}else if ( jay >= 2 && jaystep == 1 ) {
							jay = 1;
						}else{
							jay = 1;
						}
						em[21] = em[21]==0?em[22]!=0?em[22]:1:0;
						step = em[12]%(em[19]*em[15]);
					}
					if ( g->ch == ',' && em[16] >=1 ) {
						em[16] = (em[16]==1)?(2):(1);
						if ( em[16] == 1 && em[13] == 1) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
						}
					}
					if ( g->ch == 'v' ) {
						if ( em[27] == 1 ) print_header(*psh);
						if ( em[27] == 0 ) print_header_s(*psh);
						em[27] = em[27]==0?1:0;
					}
					if ( g->ch == '.' ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						if ( scsw == 1 ) scsw = 0;
					}
					if ( g->ch == ' ' && em[12] != 0 ) {
						if ( em[14] == 1 ) {
							if ( em[2] != 0 ) em[10] = em[10]==0?2:em[10]==2?4:em[10]==4?3:em[10]==3?1:0;
							if ( em[2] == 0 ) em[10] = em[10]==0?2:em[10]==2?3:em[10]==3?4:0;
							if ( em[13] != 0 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( scsw == 1 ) scsw = 0;
							}else if ( jay == 0 ) {
								if ( em[12] != 0 && em[13] == 0 && scsw == 0 && em[21] == 0 ) {
									printf("\x1b[7A");
								}
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( scsw == 1 ) scsw = 0;
							}else if ( jay == 1 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( scsw == 1 ) scsw = 0;
								jay = 0;
							}else if ( jay >= 2 ) {
								vv0->counter = 1;
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
								if ( scsw == 1 ) scsw = 0;
							}
						}
					}
					if ( g->ch == 'q' ) {
						sig_handler(SIGQUIT);
						xx->sig = SIGQUIT;
						timeFormat(em[13], psc->runtime, dur[0], dur[2]);
						psc->scnt = em[12];
						printf ("\x1b[1A");
						if ( em[27] == 0 ) print_stats_c(*psc);
						if ( em[27] == 1 ) print_stats_cs(*psc);
						print_stats(*vv0);
						if ( em[27] == 0 ) print_stats_F(*vv0, *psc, *psh);
						if ( em[27] == 1 ) print_stats_Fs(*vv0, *psc, *psh);
						g->flop = 1;
						time_diff(*xx, em[12], em[26], &xy_string);
						pthread_join(ssid[0], NULL);
						pthread_join(ssid[1], NULL);
						free(xx->cpuol);
						free(xx->sysid);
						free(xx->freqgov);
						free(xx->model);
						free(xx->version);
						free(xx->uname);
						free(g->ndev);
						free(psh->dev);
						for ( int i=0;i<g->diskcnt;i++ ) {
							free(xx->disk[i]);
						}
						free(xx);
						free(g);
						free(xy_string);
						free(psc);
						free(psh);
						free(vv0);
						em[0] = 1;
						return em[0];
					}
					if ( g->ch == 'x' ) {
						em[17]=em[17]==0?em[18]!=0?em[18]:40:0;
						if ( em[21] != 0 ) {
							if ( em[17] != 0 ) {
								psh->colour = em[3];
								psh->cpun = em[7];
								psh->verb = em[10];
								psh->scroll = em[13];
								if ( em[27] == 0 ) print_header(*psh);
								if ( em[27] == 1 ) print_header_s(*psh);
							}
						} else if ( em[17] != 0 && em[13] != 0 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
						} else if ( em[17] != 0 && em[13] == 0 && scsw == 1 ) {
							psh->colour = em[3];
							psh->cpun = em[7];
							psh->verb = em[10];
							psh->scroll = em[13];
							if ( em[27] == 0 ) print_header(*psh);
							if ( em[27] == 1 ) print_header_s(*psh);
							scsw = 0;
						}
						xcnt = 0;
					}
					if ( g->ch == '=' ) {
						if ( em[27] == 0 ) print_stats_F(*vv0, *psc, *psh);
						if ( em[27] == 1 ) print_stats_Fs(*vv0, *psc, *psh);
						sig_handler(SIGINT);
						xx->sig = SIGINT;
						time_diff(*xx, em[12], em[26], &xy_string);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 's' ) {
						sig_handler(SIGINT);
						xx->sig = SIGINT;
						time_diff(*xx, em[12], em[26], &xy_string);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 'm' ) {
						int lim = 0;
						if ( em[10] == 2 || em[10] == 4 ) {
							lim = 17 + em[11];
						}else{
							lim = 25;
						}
						em[23]++;
						if ( em[23] >= lim ) {
							em[23] = psh->focus = psc->focus = vv0->focus = 0;
						}else if ( em[23] < 15 ) {
							if ( em[23] == 14 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else{
								psh->focus = psc->focus = em[23];
								vv0->focus = 0;
							}
						}else {
							if ( em[23] == 15 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else if ( em[23] == 16 ) {
								psc->focus = em[23];
								vv0->focus = em[23]+1;
							}else{
								psh->focus = psc->focus = 0;
								vv0->focus = em[23]+1;
							}
						}
					}
					if ( g->ch == 'n' ) {
						if ( em[1] != 0 || dur[1] != 0 ) runtimetype=runtimetype==0?1:0;
					}
					if ( g->ch == 'b' ) {
						if ( em[1] != 0 || dur[1] != 0 ) runtimecounttype=runtimecounttype==0?1:0;
					}
					if ( g->ch == '!' ) {
						em[13]=em[13]==13?1:13;
					}
					if ( g->ch == 'l' ) {
						scanning=scanning==0?1:0;
						if ( scanning == 0 ) vv0->focus = psc->focus = 0;
					}
					if ( g->ch == '`' ) {
						pdiv=pdiv==1?em[11]:1;
					}
					if ( g->ch == '1' ) {
						em[4]=em[4]==1?0:1;
					}
					if ( g->ch == 'c' ) {
						em[3]=em[3]==0?1:em[3]==1?2:em[3]==2?3:em[3]==3?4:em[3]==4?5:em[3]==5?6:em[3]==6?7:0;
					}
					if ( g->ch == 'z' ) {
						em[0] = 11;
						g->flop = 1;
						pthread_join(ssid[0], NULL);
						pthread_join(ssid[1], NULL);
						time_diff(*xx, em[12], em[26], &xy_string);
						free(xx->cpuol);
						free(xx->sysid);
						free(xx->freqgov);
						free(xx->model);
						free(xx->version);
						free(xx->uname);
						free(g->ndev);
						free(psh->dev);
						for ( int i=0;i<g->diskcnt;i++ ) {
							free(xx->disk[i]);
						}
						free(xx);
						free(g);
						free(xy_string);
						free(psc);
						free(psh);
						free(vv0);
						return em[0];
					}
					if ( g->ch == 'a' && em[7] != -9 && em[8] == 0 ) {
						for ( int i=0;i<em[11];i++ ) {
							if ( em[7] == i ) {
								if ( i == em[11]-1 ) {
									em[7] = 0;
								}else{
									em[7] = i+1;
								}
								break;
							}
						}
						set_cpuAffinity(em[7], em[12]);
						if ( em[13] == 0 && em[21] == 0 ) printf("\x1b[7A");
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
					}
					if ( g->ch == 'h' ) {
						sig_handler(SIGTRAP);
						xx->sig = SIGTRAP;
						xx->scroll = em[13];
						time_diff(*xx, em[12], 0, &xy_string);
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						if ( em[13] != 0 ) {
							slot = 19;
							slat++;
							xx->slat++;
						}
					}
					g->ch = 0;
				}
			}
			if (em[13] == 0 || ( em[13] == 1 && x == 10) || em[13] == 4 ) {
				if(em[16] <= 1) {
					if ( em[21] == 0 ) {
						printf ("\x1b[1A");
					}else{
						if ( step != 0 || em[12] == 0 ) {
							printf ("\x1b[1A");
						}
						step++;
						if ( em[12] == 0 ) step--;
						if (step >= (em[19]*1/dur[2])/em[21]) step = 0;
					}
				}
				x=0;
			}
			clock_gettime(CLOCK_MONOTONIC, &tstart);
			double cnt0 = ((double)tstart.tv_sec + NSECS2SECS*tstart.tv_nsec);
			double cnt = cnt0 - cnt1;
			if ( slot == 19 ) cnt -= em[15]*dur[2]*HSLEEP;
			cntsum += cnt;
			xx->cntsum += cnt;
			cnt1 = cnt0;
			if ( em[9] == 0 ) idur1 = cnt;
			if ( em[9] == 1 ) idur1 = dur[2];
			dur[0] += idur1;
			xx->idur = dur[0];
			clock_t exetime = clock();
			xx->cpuspin = ((double)exetime - (double)exetime0)/(double)CLOCKS_PER_SEC;
			double pcpu = (xx->cpuspin / cntsum * 100L) / pdiv;
			if ( em[8] != 0 ) {
				em[7] = g->core;
				if ( em[12] != 0 ) set_cpuAffinity(em[7], em[12]);
			}
			if ( em[17] != 0 && ( em[13] != 0 || em[21] != 0 ) ) {
				if ( em[13] != 0 ) {
					if ( xcnt != 0 && xcnt % em[17] == 0 ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						xcnt = 0;
					}
				}else{
					
					if ( xcnt != 0 && xcnt % ((em[17]*(em[15]/em[21]))*em[19]) == 0 ) {
						psh->colour = em[3];
						psh->cpun = em[7];
						psh->verb = em[10];
						psh->scroll = em[13];
						if ( em[27] == 0 ) print_header(*psh);
						if ( em[27] == 1 ) print_header_s(*psh);
						printf ("\x1b[1A");
						xcnt = 0;
					}
				}
			}
			if ( em[12] < rint((double)1/dur[2]) ) {
				psc->gen_stat[0][0] = 100;
				psc->gen_stat[0][1] = 0;
			}else{
				double alignment = (double)g->cndcnt/(double)em[12]*100L;
				psc->gen_stat[0][0] = alignment>=100?100:alignment;			
				psc->gen_stat[0][1] = pcpu;
			}
			double secondssofar = dur[0];
			double secondstogo = 0L;
			if ( dur[1] != 0L ) {
				secondstogo = dur[1] - dur[0];
			}else if ( em[1] != 0 ) {
				if ( dur[2] != 0 ) {
					secondstogo = (double)em[1]*dur[2] - dur[0];
				}else{
					secondstogo = (double)(em[1]*1/em[15]) - dur[0];
				}
			}else{
				secondstogo = 0L;
			}
			double whichseconds = runtimetype==0?secondssofar:secondstogo;
			double switch60 = runtimetype==0?2:em[13];
			timeFormat(switch60, psc->runtime, whichseconds, dur[2]);
			psc->zecdecz = dur[2];
			psc->colour = em[3];
			if ( em[14] == 1 ) {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			} else if ( em[10] == 4 ) {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			} else {
				psc->gen_stat[0][2] = g->fStat[em[11]];
			}
			psc->gen_stat[0][3] = g->ctemp;
			psc->gen_stat[0][4] = g->memstat[1]/g->memstat[0];
			psc->gen_stat[0][5] = g->memstat[2]/g->memstat[0];
			psc->gen_stat[0][6] = g->memstat[3]/g->memstat[0];
			psc->gen_stat[0][7] = g->memstat[4]/g->memstat[0];
			for ( int i=0;i<3;i++ ) {
				psc->gen_stat[0][i+8] = g->loadavg[i];
			}
			sscanf( g->procsS, "%lf/%lf", &psc->gen_stat[0][11], &psc->gen_stat[0][12] );
			if ( em[10] == 2 && em[14] == 0 ) {
				psc->gen_stat[0][13] = g->cStat[0];
				psc->gen_stat[0][14] = g->cStat[1];
				psc->gen_stat[0][15] = g->cStat[2];
			}else{
				psc->gen_stat[0][13] = g->cStat[10];
				psc->gen_stat[0][14] = g->cStat[8];
				psc->gen_stat[0][15] = g->cStat[9];
			}
			if ( em[1] != 0 ) {
				if ( runtimecounttype != 0 ) {
					psc->scnt = em[1]- ( em[12] + 1 );
				}else{
					psc->scnt = em[12]+1;
				}
			}else if ( dur[1] != 0L ) {
				if ( runtimecounttype != 0 ) {
					if ( dur[2] != 0L ) psc->scnt = rint(dur[1]/dur[2]) - (em[12] + 1);
					if ( dur[2] == 0L ) psc->scnt = rint(dur[1]*(double)1/em[15]) - (em[12] + 1);
				}else{
					psc->scnt = em[12]+1;
				}
			}else{
				psc->scnt = em[12]+1;
			}
			if ( psc->scnt % (em[20]) == 0 ) {
				psc->sthis=psc->sthis==0?1:0;
				psc->sthat=psc->sthis==0?1:0;
			}
			if ( em[16] <= 1 ) {
				if ( em[27] == 0 ) print_stats_c(*psc);
				if ( em[27] == 1 ) print_stats_cs(*psc);
			}
			arrayX(psc->gen_stat[0], psc->gen_stat[1], psc->gen_stat[2], psc->gen_stat[3], 16, em[12]+1);
			if ( em[16] <= 1 ) {
				vv0->corecnt = em[11];
				if ( em[10] == 0 ) {
					vv0->stat_count = 8;
					arraySet(g->cStat, vv0->stat, vv0->stat_count, 0);
					vv0->type = 0;
					vv0->threshold = 0;
				}
				if ( em[10] == 1 ) {
					vv0->stat_count = 8;
					if ( em[14] == 0 ) arraySet(g->cStat, vv0->stat, vv0->stat_count, 0);
					if ( em[14] == 1 ) arraySet(g->cStat, vv0->stat, vv0->stat_count, 11);
					vv0->type = 2;
					vv0->threshold = 0;
				}
				if ( em[10] == 2 ) {
					vv0->stat_count = em[11];
					arraySet(g->vStat, vv0->stat, vv0->stat_count, 0);
					vv0->core = em[7];
					vv0->type = 0;
					vv0->threshold = VCPUTH;
				}
				if ( em[10] == 3 ) {
					vv0->stat_count = g->thermalzones;
					if ( em[14] == 0 ) arraySet(g->tStat, vv0->stat, vv0->stat_count, 0);
					if ( em[14] == 1 ) arraySet(g->tStat, vv0->stat, vv0->stat_count, 0);
					vv0->type = 0;
					vv0->threshold = 0;
				}
				if ( em[10] == 4 ) {
					vv0->stat_count = em[11];
					arraySet(g->fStat, vv0->stat, vv0->stat_count, 0);
					vv0->core = em[7];
					vv0->type = 0;
					vv0->threshold = 0;
				}
				vv0->colour = em[3];
				vv0->verb = em[10];
				if ( scanning == 1 ) scanner(&y, em, vv0, psc);
				if ( psc->scnt % (em[20]) == 0 ) {
					vv0->sthis=vv0->sthis==0?1:0;
					vv0->sthat=vv0->sthis==0?1:0;
				}
				print_stats(*vv0);
				arrayX(vv0->stat, vv0->min[em[10]], vv0->max[em[10]], vv0->mean[em[10]], vv0->stat_count, vv0->counter);
				vv0->counter++;
			}
			if ( em[16] >= 1 ) {
				if ( em[16] == 1 ) {
					if ( em[10] == 0 ) {
					}else if (em[10] == 1 ) {
					}else if ( em[10] == 2 ) {
					}else if ( em[10] == 3 ) {
					}
				}else if ( em[16] == 2 ) {
					print_log_stats(*psc, *vv0);
				}
			}
			printf(DEFCOLOR);
			em[12]++;
			xx->rcnt++;
			xcnt++;
			if ( g->flop == 9 ) break;
		}
	}
	xx->sig = SIGQUIT;
	psc->scnt = em[12];
	timeFormat(em[13], psc->runtime, dur[0], dur[2]);
	printf ("\x1b[1A");
	if ( em[27] == 0 ) print_stats_c(*psc);
	if ( em[27] == 1 ) print_stats_cs(*psc);
	print_stats(*vv0);
	if ( em[27] == 0 ) print_stats_F(*vv0, *psc, *psh);
	if ( em[27] == 1 ) print_stats_Fs(*vv0, *psc, *psh);
	g->flop = 1;
	time_diff(*xx, em[12], em[26], &xy_string);
	pthread_join(ssid[0], NULL);
	pthread_join(ssid[1], NULL);
	free(xx->cpuol);
	free(xx->sysid);
	free(xx->freqgov);
	free(xx->model);
	free(xx->version);
	free(xx->uname);
	free(g->ndev);
	free(psh->dev);
	for ( int i=0;i<g->diskcnt;i++ ) {
		free(xx->disk[i]);
	}
	free(xx);
	free(g);
	free(xy_string);
	free(psc);
	free(psh);
	free(vv0);
	return em[0];
}
