#include "./cpusm.h"

int main(int argc, char **argv)
{
	srand(time(NULL));
	printf(DEFCOLOR);
	char *dev;
	dev=NULL;
	char *outputfile;
	outputfile=NULL;
	int cpuaffinity = 0;
	int ival = 0;
	int op = 0;
	int em[29] = {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,0,1,0,0,0,0,0,0};
	double secs = 0L;
	double decsecs = 0L;
	double dur[4] = {0L,0L,0L,0L};
	size_t yards=247;
	em[17]=em[18]=0;
	em[11] = sysconf(_SC_NPROCESSORS_CONF);
	int rmax = sysconf(_SC_CLK_TCK);
	double inmin = (double)1/sysconf(_SC_CLK_TCK);
	 while (1)
    {
      static struct option long_options[] =
        {
          {"core",     required_argument,       0, 'a'},
          {"core-opt",  required_argument,       0, 'b'},
          {"colour",  required_argument, 0, 'c'},
          {"duration",  required_argument, 0, 'd'},
          {"slip",    required_argument, 0, 'e'},
          {"file",     required_argument,       0, 'f'},
          {"coefficient",     required_argument,       0, 'g'},
          {"help",  no_argument,       0, 'h'},
          {"shortmode",  required_argument, 0, 'i'},
          {"step",  required_argument, 0, 'j'},
          {"mode",  required_argument, 0, 'k'},
          {"log",  required_argument, 0, 'l'},
          {"display-size",  required_argument, 0, 'm'},
          {"network",    required_argument, 0, 'n'},
          {"priority",    required_argument, 0, 'o'},
          {"timing",    required_argument, 0, 'p'},
          {"precision",     required_argument,       0, 'q'},
          {"rate",  required_argument,       0, 'r'},
          {"scrolling",  required_argument, 0, 's'},
          {"interval",  required_argument, 0, 't'},
          {"rolling",    required_argument, 0, 'u'},
          {"category",    required_argument, 0, 'v'},
          {"scheduler",    required_argument, 0, 'w'},
          {"header-interval",     required_argument,       0, 'x'},
          {"nice",  required_argument,       0, 'y'},
          {"samples",  required_argument,	0, 'z'},
          {0, 0, 0, 0}
        };
		int option_index = 0;
		op = getopt_long (argc, argv, "a:b:c:d:e:f:g:hi:j:k:l:m:n:o:p:q:r:s:t:u:v:w:x:y:z:", long_options, &option_index);
		if (op == -1)
        break;
		switch (op)
		{
			case 'a':  //cpu core affinity
				em[7] = atoi(optarg);
				if ( em[7] >= em[11] || em[7] <= -2 ) {
					printf("-a%i: cpu affinity violation [cpu affinity range: 0 - %i || -1 for random affinity]\n", em[7], em[11]-1);
					exit(EXIT_FAILURE);
				}
				cpuaffinity = 1;
				break;
			case 'b':  //cpu core optimization algorithm
				em[8] = atoi(optarg);
				if ( em[8] >= 4 ) {
					printf("b%i: cpu optimization algorithm violation [<0> kernel, <1> min.cpu, <2> max.cpu, <3> random]\n", em[8]);
					exit(EXIT_FAILURE);
				}
				break;
			case 'c':  //output colour scheme
				em[3] = atoi(optarg);
				if ( em[3] >= 8 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'd':  //duration
				dur[0] = dur[1] = atof(optarg);
				break;
			case 'e':  //slip interval
				em[20] = atoi(optarg);
				break;
			case 'f':  //output filename [use in conjunction with -l]
				outputfile = optarg;
				fo = fopen(outputfile, "w+");
				if( fo == NULL ) {
					fprintf ( stderr, "execution error [log file inaccessible]: -f %s\n\n", outputfile);
					exit(EXIT_FAILURE);
				}
				break;
			case 'g':  //coefficient
				dur[3] = atof(optarg);
				break;
			case 'h':  //help
				print_app_usage();
				exit(EXIT_FAILURE);
				break;
			case 'i':  //brief-start mode [0=disabled, 1=enabled]
				em[26] = atoi(optarg);
				break;
			case 'j':  //step scroll value
				em[21] = em[22] = atoi(optarg);
				break;
			case 'k':  //mode - multi/uni
				em[14] = atoi(optarg);
				if ( em[14] >= 2 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'l':  //output mode - stdout/file
				em[16] = atoi(optarg);
				if ( em[16] >= 3 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'm':  //full/compact display
				em[27] = atoi(optarg);
				break;
			case 'n':  //network device
				//~ sprintf(dev, "%s", optarg);
				dev = strdup(optarg);
				em[2] = 1;
				break;
			case 'o': //app priority
				em[24] = atoi(optarg);
				//~ if ( em[24] < 0 || em[24] > 99 ) {
					//~ printf("process priority -o%i: priority range violation [priority range: 0:99]\n", em[24]);
					//~ exit(EXIT_FAILURE);
				//~ }
				if ( em[24] == 99 ) {
					int u = getuid();
					if ( u != 0 ) {
						printf(" -o%i: real-time priority requested ['root' privileges required]\n", em[24]);
						exit(EXIT_FAILURE);
					}
				}
				break;
			case 'p':  //time tracking algorithm
				em[9] = atoi(optarg);
				if ( em[9] >= 2 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'q':  //time tracking precision
				em[4] = atoi(optarg);
				if ( em[4] >= 2 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'r':  //rate [samples/second]
				em[15] = atoi(optarg);
				if ( em[15] > rmax || em[15] < 1 ) {
					fprintf ( stderr, "execution error [sample rate violation]: -r %s\n", optarg);
					fprintf ( stderr, "sample rate limit: 1 <= r <= %i\n", rmax);
					exit(EXIT_FAILURE);
				}
				break;
			case 's':  //stdout display mode
				em[13] = atoi(optarg);
				break;
			case 't':  //interval
				decsecs = modf(atof(optarg), &secs);
				if ( (secs+decsecs) < inmin ) {
					fprintf ( stderr, "execution error [interval violation]: -t %s\n", optarg);
					fprintf ( stderr, "interval limit: t >= %6.3lf\n", inmin );
					exit(EXIT_FAILURE);
				}else{
					ival = 1;
					dur[2] = secs+decsecs;
				}
				break;
			case 'u':  //rolling average interval
				em[19] = atoi(optarg);
				if ( em[19] < 1 || em[19] > 999 ) {
					fprintf ( stderr, "configuration error: -u %s beyond runtime configuration limit of 1 - 999\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'w':  //scheduler
				em[25] = atoi(optarg);
				if ( em[25] >= 7 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'x':  //periodic header display interval
				em[17] = em[18] = atoi(optarg);
				break;
			case 'y': //app nice value
				em[5] = atoi(optarg);
				if ( em[5] < -20 || em[5] >= 20 ) {
					printf("process nice -y%i: nice range violation [nice range: -20...19]\n", em[5]);
					exit(EXIT_FAILURE);
				}
				if ( em[5] < 0 ) {
					int u = getuid();
					if ( u != 0 ) {
						printf("priority -y%i < 0 ['root' privileges required]\n", em[5]);
						exit(EXIT_FAILURE);
					}
				}
				break;
			case 'z':  //samples
				em[1] = atoi(optarg);
				break;
			case 'v':  //stats mode
				em[10] = atoi(optarg);
				if ( em[10] >= 5 ) {
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			case '?':
				if ( optopt == 'a' || optopt == 'b' || optopt == 'c' || optopt == 'd' || optopt == 'f' || optopt == 'j' || optopt == 'k' || optopt == 'l' || optopt == 'm' || optopt == 'n' || optopt == 'o' || optopt == 'q'  || optopt == 'p' || optopt == 's' || optopt == 't' || optopt == 'u' || optopt == 'v' || optopt == 'w' || optopt == 'x' || optopt == 'y' || optopt == 'z' ) {
					fprintf ( stderr, "execution error [illegal command-line option value]: -%c\n", optopt);
					print_app_usage();
					exit(EXIT_FAILURE);
				} else {
					fprintf ( stderr, "execution error [illegal command-line option]: -%c\n", optopt);
					print_app_usage();
					exit(EXIT_FAILURE);
				}
				break;
			default:
				abort ();
		}
	}
	printf("\n");
	em[6] = set_ppid(em[3]);
	int termshort = 195-termsize(em[3]);
	if ( termshort > 0 ) {
		if ( termshort != 1 ) printf("terminal line-width is %i columns short of application line-width - disabling line wrapping\n", termshort);
		if ( termshort == 1 ) printf("terminal line-width is %i column short of application line-width - disabling line wrapping\n", termshort);
		system(DISLINEWRAP);
	}
	if ( em[3] != 0 ) printf(CLOADLOW);
	printf("maximum rate: %lu [samples/second]\n", sysconf(_SC_CLK_TCK));
	if ( dev == NULL && ( em[14] == 1 || em[10] == 1) ) {
		dev = getNetDev();
	}else{
		dev = strdup("n/a");
	}
	if ( dev != NULL ) em[2] = 2;
	int sched[4] = {em[6], em[5], em[24], em[25]};
	set_priority(sched);
	em[5] = getpriority(PRIO_PROCESS, em[6]);
	em[24] = sched[2];
	em[25] = sched[3];
	printf("process %i user-defined priority: %i\n", em[6], em[5]);
	if ( cpuaffinity == 1 && em[8] !=0 ) {
		printf ("-a%i -b%i: core.affinity/optimization conflict [cpu.affinity takes precedence]\n", em[7], em[8]);
		em[8] = 0;
	}
	if ( cpuaffinity == 1 ) {
		em[7] = em[7]==-1?rng(0,em[11],1):em[7];
		set_cpuAffinity(em[7], em[12]);
	} else {
		em[7] = -9;
	}
	for (int i=optind;i<argc;i++) {
		printf ("execution error [unknown command-line option]: %s\n\n", argv[i]);
		exit(0);
	}
	if ( em[16] >= 1 && !outputfile ) {
		printf ("execution error - log file unspecified [-f 'filename']\n\n");
		exit(0);
	}
	if ( em[2] == 0 && ( em[14] == 1 || em[10] == 1) ) {
		printf ("execution error - network interface unspecified [-n 'device']\n\n");
		exit(0);
	}
	if ( em[2] == 2 && ( em[14] == 1 || em[10] == 1 ) ) printf("network interface unspecified - using device bound to default route: -n %s\n", dev);
	if (ival == 0) {
		if ( em[15] == 0 ) {
			printf ("sample interval/rate unspecified - assuming default: -t 0.1\n");
			decsecs = 0.1;
			secs = 0;
		}else {
			if ( em[15] == 1 ) {
				decsecs = 0;
				secs = 1;
			}else if ( em[15] <= RATEMAX ) {
				decsecs = 1/(double)em[15];
				secs = 0;
			}else if ( em[15] > RATEMAX ) {
				printf ("execution error [sample rate violation]: %i\n\n", em[15]);
				print_app_usage();
				exit(EXIT_FAILURE);
			}
		}
		dur[2] = secs+decsecs;
	} else if ( ival == 1 ) {
		if (em[15] != 0 && em[15] != (double)1/(secs+decsecs) ) {
			printf ("option conflit: -t%.3lf -r%i\n\n", secs+decsecs, em[15]);
			print_app_usage();
			exit(EXIT_FAILURE);
		}
	}
	if (em[1] != 0 && dur[1] !=0 ) {
		printf ("option conflit: -d%.2lf -z%i\n\n", dur[1], em[1]);
		print_app_usage();
		exit(EXIT_FAILURE);
	}
	em[15]=em[15]!=0?em[15]:rint((double)1/dur[2]);
	char *appstart = (char *) malloc (yards + 1);
	sprintf(appstart, "%s", argv[0]);
	for ( int i=1;i<argc;i++) {
		strcat(appstart, " ");
		strcat(appstart, argv[i]);
	}
//	signal(SIGINT, sig_handler);
	
	em[0] = dmtp(outputfile, appstart, dev, em, dur);
	free(appstart);
	free(dev);
	if ( em[0] == 11 ) {
		old_attr();
		execv(argv[0], argv);
	}
	if ( termshort > 0 ) system(ENALINEWRAP);
	return 0;
}
