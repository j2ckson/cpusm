#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#define APPNAME "CPU STATISTICS MONITOR"
#define PPMYD "/tmp/pid.ppmyd"
#define DEBUGLOG "/tmp/debug.log"
#define AFFCPU "/sys/devices/system/cpu/online"
#define CPUI "/proc/cpuinfo"
#define VER "/proc/version"
#define HN "/etc/hostname"
#define GOV "/sys/devices/system/cpu/cpufreq/policy0/scaling_governor"
#define SOCT "/sys/devices/virtual/thermal/thermal_zone0/temp"
#define TEMPLS "ls -1 /sys/devices/virtual/thermal/ | egrep -c '^thermal'"
#define TEMPQUERY "cat /sys/class/hwmon/hwmon*/temp*_input"
#define TEMPNUM "cat /sys/class/hwmon/hwmon*/temp*_input | grep -c ."
#define LDAVG "/proc/loadavg"
#define PSTAT "/proc/stat"
#define STAT_STRING "%*s %lf %lf %lf %lf %lf %lf %lf"
#define NET_STRING "%*s %lf %lf %*i %*i %*i %*i %*i %*i %lf %lf"
#define DISK_STRING "%*i %*i %*s %lf %*i %lf %*i %lf %*i %lf"
#define LOAD_STRING "%lf %lf %lf %s"
#define FREQ_STRING "%*s %*s %*s %lf"
#define MEM_STRING "%*s %lf %*s"
#define NETSTAT "/proc/net/dev"
#define MSTAT "/proc/meminfo"
#define DSTAT "/proc/diskstats"
#define FREQ "lscpu --parse=MINMHZ,MAXMHZ | egrep -v '#'"
#define ID "uname -s"
#define THREADS "ps -A H | egrep -c ."
#define PROCS "ps -A | egrep -c ."
#define FREQS "lscpu | egrep -m1 MHz | egrep -o '[0-9](.*)'"
#define DEFDEV "ip route list 0.0.0.0/0 | egrep -o 'dev [[:graph:]]*' | sed -e 's/dev //'"
#define DEFDSK "mount | egrep -o '^/dev/[[:graph:]]*' | sed -e 's/.dev.//'"
#define DISLINEWRAP "setterm -linewrap off"
#define ENALINEWRAP "setterm -linewrap on"
#define COLOUR1 "\033[0;34m"
#define COLOUR2 "\033[0;37m"
#define ILOADLOW "\033[0;34m"
#define ILOADHI "\033[1;37m"
#define ILOADHI4 "\033[1;31m"
#define ACTIVECPU "\033[0;36m"
#define CLOADLOW "\033[1;31m"
#define FLOADONE "\033[1;31m"
#define CLOADHI "\033[1;37m"
#define LEFTSTATS "\033[1;32m"
#define DEFCOLOR "\033[0;37m"
#define HEADER "\033[0;37m"
#define MINCOLOUR 30
#define MAXCOLOUR 34
#define MINCOLOUR2 34
#define MAXCOLOUR2 38
#define MINCOLOUR3 34
#define MAXCOLOUR3 36
#define VALMAX "\033[1;37m"
#define HSLEEP 11
#define VARINT 10
#define ENAINT 0
#define NSECS2SECS 1.0e-9
#define NSECS 1000000000L
#define USECS 1000000L
#define BANDWIDTH 1000
#define BWCE 0.75
#define RATEMAX 1000
#define INTMIN 0.001
#define SECTSIZE 512L
#define TEMPTH 70
#define CPUTH 0.50
#define VCPUTH 0.75
#define KAVGTH 0.25

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <locale.h>
#include <signal.h>
#include <pthread.h>
#include <sys/ptrace.h>
#include <inttypes.h>
#include <termios.h>
#include <sys/resource.h> 
#include <sched.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include "handlers.c"
#include "arrayCalc.c"
#include "printFunctions.c"
#include "timeDiff.c"
#include "statThreads.c"
#include "oneKeyAction.c"
#include "dmtp.c"

//~ em[0]: return value for the dmtp function
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
