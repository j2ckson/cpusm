#include "./printFunctions.h"

void scanner(int *y, int em[], psv0 *vv0, prntsctrl *psc)
{
	int lim = 0;
	if ( em[10] == 2 || em[10] == 4 ) {
		lim = 17 + em[11];
	}else if ( em[10] == 3 ) {
		lim = 17 + vv0->thermalzones;
	}else{
		lim = 25;
	}
	if ( *y == 0 ) {
		if ( em[3] != 7 ) {
			em[23]=em[27]==1?em[23]==3?12:em[23]:em[23];
			if ( em[23] >= lim ) {
				*y = 1;
				em[23] = lim-3;
			}else if ( em[23] >= 15 ) {
				if ( em[23] <= 17 ) {
					psc->focus = em[23];
					vv0->focus = em[23];
				}else{
					psc->focus = 0;
					vv0->focus = em[23];
				}
			}else if ( em[23] < 15 ) {
				psc->focus = em[23];
				vv0->focus = 0;
			}
		}
		if ( em[3] == 7 ) {
			em[23]=em[27]==1?em[23]==4?13:em[23]:em[23];
			if ( em[23] >= lim ) {
				*y = 1;
				em[23] = lim-3;
			}else if ( em[23] >= 16 ) {
				psc->focus = em[23];
				vv0->focus = em[23];
			}else if ( em[23] < 16 ) {
				vv0->focus = 0;
				psc->focus = em[23];
			}
		}
		em[23]++;
	}
	if ( *y == 1 ) {
		if ( em[3] != 7 ) {
			if ( em[23] <= 3 ) {
				*y = 0;
				em[23] = 5;
			}else if ( em[23] >= 16 ) {
					vv0->focus = em[23];
			}else{
				vv0->focus = 0;
				psc->focus = em[23];
			}
			em[23]=em[27]==1?em[23]==13?4:em[23]:em[23];
		}else{
			if ( em[23] == 3 ) {
				*y = 0;
				em[23] = 5;
			}else if ( em[23] >= 16 ) {
				vv0->focus = em[23];
			}else{
				vv0->focus = 0;
			}
			em[23]=em[27]==1?em[23]==14?5:em[23]:em[23];
		}			
		for ( int i=18;i>2;i-- ) {
			if ( em[23] == i ) psc->focus = i-2;
		}
		em[23]--;
	}
}

void print_div(double stat, int verb)
{
	if ( verb == 4 ) {
		printf ("%-7.1lf", stat);
	}else if ( verb == 3 ) {
		printf ("%-7.2lf", stat);
	}else if ( verb == 5 ) {
		printf ("%05.0lf  ", stat);
	}else if ( verb == 7 ) {
		printf ("%05.0lf  ", stat);
	}else if ( verb == 9 ) {
		printf ("       ");
	}else if ( verb == 0 || verb == 2 ) {
		if ( stat >= 1L ) {
			printf ("%-7.1lf", stat*100L);
		}else if ( stat*100L < 10L && stat*100L > 9.999) {
			printf ("%-7.2f", stat*100L);
		}else if ( stat < 0.1L ) {
			printf ("%-7.3f", stat*100L);
		}else{
			printf ("%-7.2f", stat*100L);
		}
	}else{
		if ( stat < 10) printf ("%-7.3lf", stat);
		if ( stat < 100 && stat >= 10) printf ("%-7.2lf", stat);
		if ( stat < 1000 && stat >= 100 ) printf ("%-7.1lf", stat);
		if ( stat < 10000 && stat >= 1000 ) printf ("%05.0lf  ", stat);
		if ( stat >= 10000 ) printf ("%-7.0lf", stat);
	}
	return;
}
void print_colour(int colour, double stat, double val, int focus)
{
	if ( colour != 0 ) {
		if ( focus == 1 ) {
			//~ if ( colour == 1 ) printf("\033[0;40m");
			if ( colour == 1 ) printf(CLOADLOW);
			if ( colour == 2 ) printf(ILOADLOW);
			if ( colour == 3 ) printf("\033[0;38m");
			if ( colour == 4 ) printf("\033[0;38m");
			if ( colour == 7 ) printf(ILOADLOW);
			if ( colour == 6 ) printf("\033[0;38m");
			if ( colour == 5 ) printf("\033[0;38m");
		}else if ( focus == 2 ) {
			if ( colour == 1 ) {
				printf(ILOADLOW);
			}else if ( colour == 2 ){
				printf (CLOADLOW);
			}else if ( colour == 7 ){
				printf (CLOADLOW);
			}else{
				printf ("\033[0;38m");
			}
		}else{
			if ( colour == 1 ) {
				printf (ILOADLOW);
			}
			if ( colour == 2 ) {
				printf (CLOADLOW);
			}
			if ( colour == 5 ) {
				printf ("\033[1;%im", rng(MINCOLOUR,MAXCOLOUR,1));
			}
			if ( colour == 6 ) {
				printf ("\033[1;%im", rng(MINCOLOUR2,MAXCOLOUR2,1));
			}
			if ( colour == 4 ) {
				printf ("\033[1;%im", rng(MINCOLOUR3,MAXCOLOUR3,1));
			}
			if ( colour == 3 ) {
				if ( val != 0 ) printf (ILOADLOW);
				if ( val == 0 ) printf (CLOADLOW);
			}
		}
	}else{
		printf(DEFCOLOR);
	}
	return;
}
void print_max_colour(double stat, double val, int colour)
{
	if ( stat > val && stat != 0 ) printf (VALMAX);
	//~ if ( colour == 0 || stat <= val ) printf (DEFCOLOR);
	return;
}
void print_stats_c(prntsctrl psc)
{
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-10i%-7.2lf ", psc.scnt,psc.gen_stat[0][0]);
	printf ("%-6.2lf%s", psc.gen_stat[0][1], psc.runtime);
	if ( psc.colour == 3 ) {
		for ( int i=2;i<16;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( psc.focus == i+1 && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[0][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[0][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[0][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[0][i], 7);
		}
	}else{
		for ( int i=2;i<16;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else if ( psc.focus == i+1 && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
			if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[0][i], 0);
			if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[0][i], 1);
			if ( i == 11 ) print_div(psc.gen_stat[0][i], 5);
			if ( i == 12 ) print_div(psc.gen_stat[0][i], 7);
		}
	}
	return;
}
void print_stats_cs(prntsctrl psc)
{
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-9i", psc.scnt);
	if ( psc.idurh > 0 ) {
		printf ("%4i:%02i:%05.2lf", psc.idurh,psc.idurm,psc.idurs);
	}else if ( psc.idurm > 0 ) {
		printf ("     %02i:%05.2lf", psc.idurm,psc.idurs);
	}else if ( psc.zecdecz > 0.1 ) {
		printf ("        %05.2lf", psc.idurs);
	}else{
		printf ("       %06.3lf", psc.idurs);
	}
	if ( psc.colour == 3 ) {
		for ( int i=2;i<4;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
		}
		for ( int i=13;i<16;i++ ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
			}else if ( psc.focus == i ) {
				print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
			}else if ( i % 2 == 0 ) {
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthis, 0);
			}else{
				print_colour(psc.colour, psc.gen_stat[0][i], psc.sthat, 0);
			}
			print_div(psc.gen_stat[0][i], 0);
		}
	}else{
		for ( int i=2;i<4;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[0][2]);
			if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[0][3]);
		}
		for ( int i=13;i<16;i++ ) {
			if ( psc.colour != 0 ) {
				if ( psc.focus == i-1 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 1);
				}else if ( psc.focus == i && psc.colour != 0 ) {
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 2);
				}else{
					print_colour(psc.colour, psc.gen_stat[0][i], 0, 0);
				}
			}
			print_div(psc.gen_stat[0][i], 0);
		}
	}
	return;
}
void print_stats(psv0 vv0)
{
	int whichone = 0;
	int onewhich = 0;
	if ( vv0.multistat == 1 ) {
		whichone = vv0.corecnt>vv0.thermalzones?vv0.corecnt:vv0.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( vv0.verb == 0 || vv0.verb == 1 ) onewhich = 8;
		if ( vv0.verb == 2 || vv0.verb == 4 ) onewhich = vv0.corecnt;
		if ( vv0.verb == 3 ) onewhich = vv0.thermalzones;
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		if ( vv0.colour == 3 ) {
			if ( vv0.focus == i+16 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( vv0.focus == i+17 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 2);
			}else if ( vv0.focus == i+18 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( i % 2 == 0 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.sthis, 0);
			}else{
				print_colour(vv0.colour, vv0.stat[i], vv0.sthat, 0);
			}
			print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			if ( (vv0.verb == 2 || vv0.verb ==4) && vv0.core == i ) printf(ACTIVECPU);
			print_div(vv0.stat[i], vv0.verb);
		}else{
			if ( vv0.focus == i+16 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else if ( vv0.focus == i+17 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 2);
			}else if ( vv0.focus == i+18 ) {
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 1);
			}else{
				print_colour(vv0.colour, vv0.stat[i], vv0.threshold, 0);
			}
			if ( vv0.colour != 7 ) print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			//~ print_max_colour(vv0.stat[i], vv0.max[vv0.verb][i], vv0.colour);
			if ( (vv0.verb == 2 || vv0.verb ==4) && vv0.core == i ) printf(ACTIVECPU);
			print_div(vv0.stat[i], vv0.verb);
		}
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	if ( vv0.multistat == 1 ) {
		if ( vv0.stat_count < onewhich ) {
			for ( int i=vv0.stat_count;i<onewhich;i++ ) {
				print_div(0, 9);
			}
		}
		if ( onewhich < 8 ) {
			for ( int i=onewhich;i<8;i++ ) {
				print_div(0, 9);
			}
		}
	}

	printf(DEFCOLOR);
	printf("\n");
	return;
}
void print_stats_F(psv0 vv0, prntsctrl psc, psheader psh)
{
	int whichone = 0;
	int onewhich = 0;
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	for ( int i=0;i<(40+(14+onewhich)*7);i++) {
		printcc("\u2500", 1, psc.colour);
	}
	printf("%s", "\u2510\n");
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-10s%-7.2lf ", "minimum",psc.gen_stat[1][0]);
	printf("%-6.3lf",psc.gen_stat[1][1]);
	printf ("     %8s", "min.stat");
	for ( int i=2;i<16;i++ ) {
		print_colour(psc.colour, psc.gen_stat[1][i], 0, 0);
		if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[1][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[1][3]);
		if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[1][i], 0);
		if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[1][i], 1);
		if ( i == 11 ) print_div(psc.gen_stat[1][i], 5);
		if ( i == 12 ) print_div(psc.gen_stat[1][i], 7);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.min[vv0.verb][i], 0, 0);
		print_div(vv0.min[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-10s%-7.2lf ", "maximum",psc.gen_stat[2][0]);
	printf("%-6.3lf",psc.gen_stat[2][1]);
	printf ("     %8s", "max.stat");
	for ( int i=2;i<16;i++ ) {
		print_colour(psc.colour, psc.gen_stat[2][i], 0, 0);
		if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[2][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[2][3]);
		if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[2][i], 0);
		if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[2][i], 1);
		if ( i == 11 ) print_div(psc.gen_stat[2][i], 5);
		if ( i == 12 ) print_div(psc.gen_stat[2][i], 7);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.max[vv0.verb][i], 0, 0);
		print_div(vv0.max[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	if ( psc.colour == 1 ) printcc("", 2, psc.colour);
	if ( psc.colour >= 2 ) printcc("", 1, psc.colour);
	printf ("%-10s%-7.2lf ", "average",psc.gen_stat[3][0]);
	printf("%-6.3lf",psc.gen_stat[3][1]);
	printf ("     %8s", "avg.stat");
	for ( int i=2;i<16;i++ ) {
		print_colour(psc.colour, psc.gen_stat[3][i], 0, 0);
		if ( i == 2 ) printf ("   %6.1lf  ", psc.gen_stat[3][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[3][3]);
		if ( (i >=4 && i <= 7) || (i >= 13) ) print_div(psc.gen_stat[3][i], 0);
		if ( i > 7 && i <= 10 ) print_div(psc.gen_stat[3][i], 1);
		if ( i == 11 ) print_div(psc.gen_stat[3][i], 5);
		if ( i == 12 ) print_div(psc.gen_stat[3][i], 7);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.mean[vv0.verb][i], 0, 0);
		print_div(vv0.mean[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	return;
}
void print_stats_Fs(psv0 vv0, prntsctrl psc, psheader psh)
{
	if ( psc.colour != 0 ) printf (ILOADLOW);
	int whichone = 0;
	int onewhich = 0;
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	for ( int i=0;i<(25+(5+onewhich)*7);i++) {
		printf("%s", "\u2500");
	}
	printf("%s", "\u2510\n");
	if ( psc.colour != 0 ) printf (ILOADLOW);
	printf ("%-10s    ", "minimum");
	printf ("%8s", "min.stat  ");
	for ( int i=2;i<4;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 0);
			}
		}
		if ( i == 2 ) printf (" %6.1lf  ", psc.gen_stat[1][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[1][3]);
	}
	for ( int i=13;i<16;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[1][i], 0, 0);
			}
		}
		print_div(psc.gen_stat[1][i], 0);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.min[vv0.verb][i], 0, 0);
		print_div(vv0.min[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	if ( psc.colour != 0 ) printf (ILOADLOW);
	printf ("%-10s    ", "maximum");
	printf ("%8s", "max.stat  ");
	for ( int i=2;i<4;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 0);
			}
		}
		if ( i == 2 ) printf (" %6.1lf  ", psc.gen_stat[2][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[2][3]);
	}
	for ( int i=13;i<16;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[2][i], 0, 0);
			}
		}
		print_div(psc.gen_stat[2][i], 0);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.max[vv0.verb][i], 0, 0);
		print_div(vv0.max[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	if ( psc.colour != 0 ) printf (ILOADLOW);
	printf ("%-10s    ", "average");
	printf ("%8s", "avg.stat  ");
	for ( int i=2;i<4;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 0);
			}
		}
		if ( i == 2 ) printf (" %6.1lf  ", psc.gen_stat[3][2]);
		if ( i == 3 ) printf ("%-5.1lf ", psc.gen_stat[3][3]);
	}
	for ( int i=13;i<16;i++ ) {
		if ( psc.colour != 0 ) {
			if ( psc.focus == i-1 ) {
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 1);
			}else if ( psc.focus == i && psc.colour != 0 ) {
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 2);
			}else{
				print_colour(psc.colour, psc.gen_stat[3][i], 0, 0);
			}
		}
		print_div(psc.gen_stat[3][i], 0);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		print_colour(vv0.colour, vv0.mean[vv0.verb][i], 0, 0);
		print_div(vv0.mean[vv0.verb][i], vv0.verb);
		if ( vv0.type != 0 ) {
			if ( i  == vv0.stat_count/vv0.type-1 ) printf(" ");
		}
	}
	printf("\n");
	return;
}
void print_header(psheader psh)
{
	int vs = 0;
	const char* verb0;
	const char* schd0;
	if ( psh.verb == 0 ) {
		verb0 = "cpu.statistics";
	}else if ( psh.verb == 1 ) {
		verb0 = "network.statistics";
	}else if ( psh.verb == 2 ) {
		verb0 = "core.statistics";
	}else if ( psh.verb == 3 ) {
		verb0 = "thermal.statsistics";
	}else{
		verb0 = "frequency.statistics";
	}
	if ( psh.sched == 0 ) {
		schd0 = "nrml";
	}else if ( psh.sched == 1 ) {
		schd0 = "fifo";
	}else if ( psh.sched == 2 ) {
		schd0 = "rdrn";
	}else if ( psh.sched == 3 ) {
		schd0 = "btch";
	}else if ( psh.sched == 5 ) {
		schd0 = "idle";
	}else if ( psh.sched == 6 ) {
		schd0 = "ddln";
	}else{
		schd0 = "n/a ";
	}
	vs = strlen(verb0);
	printf(DEFCOLOR);
	char sppid[1234567];
	sprintf(sppid, "%d", psh.ppid);
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("%i%s", psh.ppid, ".metrics");
	int strlensppid = 29 - strlen(sppid);
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<strlensppid;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf ("%-1s", "\u2503");
	printf("%s", "\u250C");
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf ("%-*s", vs, verb0);
	int whichone = 0;
	int onewhich = 0;
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	for ( int i=0;i<(14+onewhich)*7-vs;i++) {
		printcc("\u2500", 1, psh.colour);
	}
	printcc("\u2510\n", 1, psh.colour);
	verb0=NULL;
	printcc("                                      \u2503\n", 1, psh.colour);
	printcc("", 2, psh.colour);
	if ( psh.cpun == -9 ) printf ("%s%4s %4s%02i %4s%03i %s","  schd:",schd0," pri:",psh.pri," nce:",psh.nice," core:k  ");
	if ( psh.cpun == -9 )printcc("┃", 1, psh.colour);
	printcc("", 2, psh.colour);
	if ( psh.cpun != -9 ) printf ("%s%4s %4s%02i %4s%03i %s%02i","  schd:",schd0," pri:",psh.pri," nce:",psh.nice," core:",psh.cpun);
	if ( psh.cpun != -9 )printcc(" ┃", 1, psh.colour);
	printf("\n");
	printcc("\u2500", 1, psh.colour);
	for (int i=0;i<36;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf("%s", "┃");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	printf("%s", "\u250C");
	for (int i=0;i<12;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf("%s", "\u250C");
	for (int i=0;i<9;i++ ) {
		printf("\u2500");
	}
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("memory");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<10;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf("%s", "\u250C");	
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("load.avg");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");	
	printf("%s", "\u250C");
	for (int i=0;i<4;i++ ) {
		printf("\u2500");
	}
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("procs");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<3;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");	
	printf("%s", "\u250C");
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("cpu.avg");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");	
	const char* title0;
	int sizeTitle = 0, sizeTitlePrint = 0;
	if ( psh.verb == 0 ) {
		title0 = "kernel.performance";
	}else if ( psh.verb == 1 ) {
		title0 = psh.dev;
	}else if ( psh.verb == 2 ) {
		title0 = "core.performance";
	}else if ( psh.verb == 3 ) {
		title0 = "sys.temp";
	}else{
		title0 = "core.frequency";
	}
	sizeTitle = strlen(title0);
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	if ( psh.verb == 0 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	if ( psh.verb == 2 || psh.verb == 4 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	if ( psh.verb == 1 ) {

		sizeTitlePrint = (8*7/4-(sizeTitle+4)/2);
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s%3s", sizeTitle, title0, ".rx");
		if ( psh.colour != 0 ) printf(ILOADLOW);

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-2s", "\u2510\u250C");

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s%3s", sizeTitle, title0, ".tx");
		if ( psh.colour != 0 ) printf(ILOADLOW);

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			printf ("%-1s","\u250C");
			for ( int q=2;q<(onewhich-8)*7;q++ ) {
				printf("\u2500");
			}
			printf("%-1s", "\u2510");
		}
		printf("\n");
	}
	if ( psh.verb == 3 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	title0=NULL;
	if ( psh.colour != 0 ) printf(ILOADLOW);
	printf ("%-10s%-6s%6s        %7s", "count","algn","pcpu","runtime");
	printf (" ┃");
	printf (" ");
	printf ("%-6s  ", "a.freq");
	printf ("%-5s  %-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-2s%03i  ","a.\u00B0C","free","avai","buff","cach","01mn","05mn","15mn","rnng","actv","spot","cuml","r.",psh.roll);
	if ( psh.verb == 0 ) {
		printf ("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s","user","nice","syst","idle","iowa","irqs","sirq","maxi");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if ( psh.netdis != 0 && psh.verb == 1 ) {
		printf ("%-7s%-7s%-2s%03i  %-7s %-7s%-7s%-2s%03i  %-7s","spot","cuml","r.",psh.roll,"p/sec","spot","cuml","r.",psh.roll,"p/sec");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.verb == 2 || psh.verb == 4 ) {
	   for ( int f=0; f<psh.corecnt; f++ ) {
			printf("%-4s%-3i", "cpu.", f);
		}
		if ( psh.multistat == 1 && psh.corecnt < 8 ) {
			for ( int f=psh.corecnt; f<8; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.verb == 3) {
		for ( int k=0;k<psh.thermalzones;k++ ) {
			printf("%2s%02i   ", "\u00B0C", k);
		}
		if ( psh.multistat == 1 && psh.corecnt > psh.thermalzones ) {
			for ( int f=psh.thermalzones; f<psh.corecnt; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.scroll <= 0) {
		printf ("\n");
	}
	if ( psh.colour != 0 ) printf(DEFCOLOR);
	return;
}
void print_header_s(psheader psh)
{
	int vs = 0;
	const char* verb0;
	if ( psh.verb == 0 ) {
		verb0 = "cpu.statistics";
		vs = strlen(verb0);
	}else if ( psh.verb == 1 ) {
		verb0 = "network.statistics";
	}else if ( psh.verb == 2 ) {
		verb0 = "core.statistics";
	}else if ( psh.verb == 3 ) {
		verb0 = "thermal.statsistics";
	}else{
		verb0 = "frequency.statistics";
	}
	vs = strlen(verb0);
	printf(DEFCOLOR);
	char sppid[1234567];
	sprintf(sppid, "%d", psh.ppid);
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("%i%s", psh.ppid, ".metrics");
	int strlensppid = 14 - strlen(sppid);
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<strlensppid;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf ("%-1s", "\u2503");
	printf("%s", "\u250C");
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf ("%-*s", vs, verb0);
	int whichone = 0;
	int onewhich = 0;
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for ( int i=0;i<(5+onewhich)*7-vs;i++) {
		printf("%s", "\u2500");
	}
	printf("%s", "\u2510\n");
	printf(DEFCOLOR);
	if ( psh.colour != 0 ) printf(ILOADLOW);
	verb0=NULL;
	printf("                       \u2503\n");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	if ( psh.cpun == -9 ) printf ("%s%i %s%02i %s%03i %s"," s:",psh.sched," p:",psh.pri," n:",psh.nice," c:k ┃");
	if ( psh.cpun != -9 ) printf ("%s%i %s%02i %s%03i %s%i%s"," s:", psh.sched," p:",psh.pri," n:",psh.nice," c:",psh.cpun," ┃");
	printf("\n");
	printf("%s", "\u2500");
	for (int i=0;i<21;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf("%s", "┃");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	printf("%s", "\u250C");
	for (int i=0;i<12;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");
	printf("%s", "\u250C");
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	if ( psh.colour != 0 ) printf(CLOADLOW);
	printf("cpu.avg");
	if ( psh.colour != 0 ) printf(ILOADLOW);
	for (int i=0;i<6;i++ ) {
		printf("\u2500");
	}
	printf("%s", "\u2510");	
	const char* title0;
	int sizeTitle = 0, sizeTitlePrint = 0;
	if ( psh.verb == 0 ) {
		title0 = "kernel.performance";
	}else if ( psh.verb == 1 ) {
		title0 = psh.dev;
	}else if ( psh.verb == 2 ) {
		title0 = "core.performance";
	}else if ( psh.verb == 3 ) {
		title0 = "sys.temp";
	}else{
		title0 = "core.frequency";
	}
	sizeTitle = strlen(title0);
	if ( psh.multistat == 1 ) {
		whichone = psh.corecnt>psh.thermalzones?psh.corecnt:psh.thermalzones;
		onewhich = whichone>8?whichone:8;
	}else{
		if ( psh.verb == 0 || psh.verb == 1 ) onewhich = 8;
		if ( psh.verb == 2 || psh.verb == 4 ) onewhich = psh.corecnt;
		if ( psh.verb == 3 ) onewhich = psh.thermalzones;
	}
	if ( psh.verb == 0 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	if ( psh.verb == 2 || psh.verb == 4 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	if ( psh.verb == 1 ) {
		sizeTitlePrint = (8*7/4-(sizeTitle+4)/2);

		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s%3s", sizeTitle, title0, ".rx");
		if ( psh.colour != 0 ) printf(ILOADLOW);

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-2s", "\u2510\u250C");

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s%3s", sizeTitle, title0, ".tx");
		if ( psh.colour != 0 ) printf(ILOADLOW);

		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			printf ("%-1s","\u250C");
			for ( int q=2;q<(onewhich-8)*7;q++ ) {
				printf("\u2500");
			}
			printf("%-1s", "\u2510");
		}
		printf("\n");
	}
	if ( psh.verb == 3 ) {
		if ( sizeTitle % 2 == 1 || onewhich % 2 == 1 ) sizeTitle += 1;
		sizeTitlePrint = (onewhich*7-sizeTitle)/2;
		printf ("%-1s","\u250C");
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		if ( psh.colour != 0 ) printf(CLOADLOW);
		printf("%*s", sizeTitle, title0);

		if ( psh.colour != 0 ) printf(ILOADLOW);
		for ( int q=0;q<sizeTitlePrint;q++ ) {
			printf("\u2500");
		}
		printf("%-1s", "\u2510\n");
	}
	title0=NULL;
	if ( psh.colour != 0 ) printf(ILOADLOW);
	printf ("%-10s     %7s", "count","runtime");
	printf (" ┃");
	printf (" ");
	printf ("%-6s  ", "a.freq");
	printf ("%-5s  %-7s%-7s%-2s%03i  ","a.\u00B0C","spot","cuml","r.",psh.roll);
		if ( psh.verb == 0 ) {
		printf ("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s","user","nice","syst","idle","iowa","irqs","sirq","maxi");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if ( psh.netdis != 0 && psh.verb == 1 ) {
		printf ("%-7s%-7s%-2s%03i  %-7s %-7s%-7s%-2s%03i  %-7s","spot","cuml","r.",psh.roll,"p/sec","spot","cuml","r.",psh.roll,"p/sec");
		if ( psh.multistat == 1 && onewhich > 8 ) {
			for ( int f=8; f<onewhich; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.verb == 2 || psh.verb == 4 ) {
	   for ( int f=0; f<psh.corecnt; f++ ) {
			printf("%-4s%-3i", "cpu.", f);
		}
		if ( psh.multistat == 1 && psh.corecnt < 8 ) {
			for ( int f=psh.corecnt; f<8; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.verb == 3) {
		for ( int k=0;k<psh.thermalzones;k++ ) {
			printf("%2s%02i   ", "\u00B0C", k);
		}
		if ( psh.multistat == 1 && psh.corecnt > psh.thermalzones ) {
			for ( int f=psh.thermalzones; f<psh.corecnt; f++ ) {
				printf("%-7s","  --");
			}
		}
		printf ("\n\n");
	}
	if (psh.scroll <= 0) {
		printf ("\n");
	}
	if ( psh.colour != 0 ) printf(DEFCOLOR);
	return;
}
void print_log_stats(prntsctrl psc, psv0 vv0)
{
	fprintf(fo, "%i,%.1lf,%.1lf,%02i:%02i:%06.3lf,", psc.scnt,psc.gen_stat[0][0],psc.gen_stat[0][1],psc.idurh,psc.idurm,psc.idurs);
	for ( int i=2;i<16;i++ ) {
		fprintf(fo, "%4.2lf,", psc.gen_stat[0][i]);
	}
	for ( int i=0;i<vv0.stat_count;i++ ) {
		if ( i != vv0.stat_count-1 ) fprintf(fo, "%4.2lf,", vv0.stat[i]);
		if ( i == vv0.stat_count-1 ) fprintf(fo, "%4.2lf\n", vv0.stat[i]);
	}
}
void print_app_usage(void)
{
	printf("\ncpusm [option]<value>...[option]<value>\n");
	printf("_______________________________________________________________________________________________________________\n");
	printf("command-line options [* default] \n\n");
	printf("-a || --core: cpusm core affinity \n");
	printf("-b || --core-opt: cpusm core optimization [<0*> kernel, <1> min.cpu, <2> max.cpu, <3> random] \n");
	printf("-c || --colour: stdout/terminal display colour [0* - 7]\n");
	printf("-d || --duration: cpusm execution duration [x.y seconds] \n");
	printf("-e || --slip: interval affecting frequency and temperature statistics [1/slip >=1, 4*]\n");
	printf("-f || --file: <filename>: log destination\n");
	printf("-g || --coefficient: statistics sampling interval coefficient [g * t, 1*]\n");
	printf("-h || --help: execution assistance \n");
	printf("-i || --shortmode: cpusm start information mode [0* - normal, 1 - abbreviated] \n");
	printf("-j || --step: cpusm statistics output step interval [1/j * r * u] \n");
	printf("-k || --mode: cpusm statistics output mode [0 - uni, 1* - multi] \n");
	printf("-l || --log cpusm output mode [0* - stdout/terminal, 1 - stdout/terminal+file, 2 - file] \n");
	printf("-m || --display-size: [cpusm statistics mode [0* - normal, 1 - abbreviated] \n");
	printf("-n || --network: network interface to monitor [-k1 || -k0 -v1]\n");
	printf("-o || --priority: cpusm application execution priority [0* - 99]\n");
	printf("-p || --timing: time tracking alogrithm [0* - dynamic, 1 - static]\n");
	printf("-q || --precision: time tracking precision [0 - off, 1* - on]\n");
	printf("-r || --rate: sample rate per second [1 - 1000, 10*]\n");
	printf("-s || --scrolling: output display mode [0* - fixed-line, 1 - scrolling]\n");
	printf("-t || --interval: sample interval [s.n seconds, 0.1*, minimum == 0.001] \n");
	printf("-u || --rolling: cpusm statistics rolling average interval [1* - 999] seconds \n");
	printf("-v || --category: cpusm statistics category \n");
	printf("        0: cpu.kernel mode\n");
	printf("        1: network mode [use in conjunction with -n]\n");
	printf("        2: cpu.core mode\n");
	printf("        3: thermal mode\n");
	printf("        4*: core.frequency mode\n");
	printf("-w || --scheduler: cpusm process scheduling algorithm [0* - 6]\n");
	printf("-x || --header-interval: periodic column heading display [0* == off, *40 == runtime switch default]\n");
	printf("-y || --nice: cpusm execution nice value [-20 to 20, 0*] \n");
	printf("-z || --samples: total samples [>=0, 0* - no limit] \n");
	printf("_______________________________________________________________________________________________________________\n");
	printf("runtime options \n\n");
	printf("<h>: show runtime help\n");
	printf("<a>: switch core affinity [base process only - established threads will not switch\n");
	printf("<c>: switch stdout colour\n");
	printf("<q>: quit cpusm\n");
	printf("<j>: enable/disable statistics step display\n");
	printf("<l>: enable/disable colour-mode focus scanning \n");
	printf("<m>: enable/switch colour-mode focus \n");
	printf("<s>: display cpusm summary statistics \n");
	printf("<x>: enable/disable periodic header printing \n");
	printf("<z>: restart cpusm\n");
	printf("<=>: display cpusm min/max/avg statistics\n");
	printf("</>: switch output mode [fixed-line/scrolling]\n");
	printf("<.>: display column headings\n");
	printf("<`>: switch cpusm cpu utilization tracking [core/cpu]\n");
	printf("<,>: enable/diable stdout [file logging-mode]]\n");
	printf("<SPACE>||<v>: switch display mode [cpu.kernel/cpu.core/core.frequency/thermal/network]\n\n");
//~ em[1]: cli option 'z' - total samples
//~ em[2]: cli option 'n' - network device
//~ em[3]: cli option 'c' - terminal output colour
//~ em[4]: cli option 'q' - time tracking precision [0 == off, 1* == on]
//~ em[5]: cli option 'y' - execution nice value [-20 to 20]
//~ em[6]: cpusm process identification number [pid]
//~ em[7]: cli option 'a' - cpusm core affinity
//~ em[8]: cli option 'b' - core optimization algorithm [<0*> kernel, <1> min.cpu, <2> max.cpu, <3> random]
//~ em[9]: cli option 'p' - time tracking alogrithm [0* - dynamic, 1 - static]
//~ em[10]: cli option 'v' - cpusm statistics output type [0 - cpu kernel, 1 - network, 2 - core utilization, 3 - temperature, 4* core frequency]
//~ em[11]: system processor count per sysconf(_SC_NPROCESSORS_CONF)
//~ em[12]: running sample count
//~ em[13]: cli option 's' - cpusm output mode [0* fixed-line, 1 scrolling]
//~ em[14]: cli option 'k' - cpusm statistics output mode [0 - uni/em[10]-fixed, 1* multi]
//~ em[15]: cli option 'r' - sample rate per second [1 - 1000, 10*]
//~ em[16]: cli option 'l' - cpusm output mode [0* - stdout/terminal, 1 - stdout/terminal+file, 2 - file]
//~ em[17]: cli option 'x' - cpusm periodic header display interval [40* - runtime switch default]
//~ em[18]: copy of em[17] initial value
//~ em[19]: cli option 'u' - cpusm statistics rolling average interval [1* - 999] seconds
//~ em[20]: cli option 'e' - cpusm 'slip' interval [4*]
//~ em[21]: cli option 'j' - cpusm statistics output step interval [1/j * r * u]
//~ em[22]: copy of em[21] initial value
//~ em[23]: cpusm statistics focus value
//~ em[24]: cli option 'o' - cpusm application execution priority [0* - 99]
//~ em[25]: cli option 'w' - cpusm process scheduling algorithm [0* - 6]
//~ em[26]: cli option 'i' - cpusm start information mode [0* - normal, 1 - abbreviated]
//~ em[27]: cli option 'm' - cpusm statistics display mode [0* - normal, 1 - short]
//~ em[28]: cli option ' ' - cpusm client/server mode [not yet implemented]
//~ dur[0]: cli option 'd' - cpusm execution duration [x.y seconds]
//~ dur[1]: copy of dur[0] initial value
//~ dur[2]: cli option 't' - cpusm sample interval [conflicts with em[15]]
//~ dur[3]: cli option 'g' - coefficient of statistics sampling interval [g * t]
	return;
}
void print_logheader(int netdis, int corecnt)
{
	fprintf (fo,"%-4s %-4s %-4s %-4s ","cntt","ctme","%cpu","time");
		fprintf (fo,"%-4s %-4s %-4s ","freq","temp","load");
		fprintf (fo,"%-4s ", "idle");
		fprintf (fo,"%-4s ", "vidl");
		fprintf (fo,"%-4s ", "mcpu");
		fprintf (fo,"%-4s ", "vcpu");
		fprintf (fo,"%-4s ", "cavg");
		fprintf (fo,"%-4s ", "ca1s");
		fprintf (fo,"%-4s ", "syss");
		fprintf (fo,"%-4s ", "usrr");
		fprintf (fo,"%-4s ", "nice");
		fprintf (fo,"%-4s ", "ioio");
		fprintf (fo,"%-4s ", "irqq");
		for ( int i=0; i<corecnt; i++ ) {
			fprintf (fo,"%-3s%i,", "cpu",i);
		}
		if ( netdis != 0 ) {
			fprintf (fo, "%-4s ", "rxMs");
			fprintf (fo, "%-4s ", "rxMa");
			fprintf (fo, "%-4s ", "rxM1");
			fprintf (fo, "%-4s ", "rxps");
			fprintf (fo, "%-4s ", "txMs");
			fprintf (fo, "%-4s ", "txMa");
			fprintf (fo, "%-4s ", "txM1");
			fprintf (fo, "%-4s ", "txps");
		}
		fprintf (fo,"%-4s %-4s ", "prcs","nlwp");
		fprintf (fo,"%-4s ", "avai");
		fprintf (fo,"%-4s ", "free");
		fprintf (fo,"%-4s\n", "swap");
	return;
}
void print_current_time_with_ms(int rcnt, char* *xy_string, char* *yx_string)
{
	long  ms;
	time_t ss;
	struct timespec spec;
	struct tm* myty;
	char tyu_string[40];
	char y_string[40];
	char x_string[40];
	clock_gettime(CLOCK_REALTIME, &spec);
	ss  = spec.tv_sec;
	ms = round((double)spec.tv_nsec / (double)1.0e7);
	if (ms > 99) {
		ss++;
		ms = 0;
	}
	myty = localtime (&ss);
	if (rcnt == 0 ) {
		strftime (x_string, sizeof (x_string), "%H:%M:%S", myty);
		sprintf(tyu_string, ".%02ld", ms);
		strcat(x_string, tyu_string);
		memcpy(*xy_string, x_string, sizeof(x_string));
	}
	strftime (y_string, sizeof (y_string), "%H:%M:%S", myty);
	sprintf(tyu_string, ".%02ld", ms);
	strcat(y_string, tyu_string);
	memcpy(*yx_string, y_string, sizeof(y_string));

	return;
}
