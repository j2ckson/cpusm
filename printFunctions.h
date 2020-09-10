#ifndef PRINTFUNCTIONS_H
#define PRINTFUNCTIONS_H

void scanner(int *y, int em[],  psv0 *vv0,  prntsctrl *psc);
void print_stats_c(prntsctrl psc);
void print_stats_cs(prntsctrl psc);
void print_stats(psv0 vv0);
void print_stats_F(psv0 vv0,  prntsctrl psc,  psheader psh);
void print_stats_Fs(psv0 vv0,  prntsctrl psc,  psheader psh);
void print_stats_v0(psv0 vv0);
void print_app_usage(void);
void print_header(psheader psh);
void print_header_s(psheader psh);
void print_logheader(int netdis, int corecnt);
void print_log_stats(prntsctrl psc,  psv0 vv0);
void print_current_time_with_ms(int rcnt, char* *xy_string, char* *yx_string);
void print_div(double stat, int verb);
void print_colour(int colour, double stat, double val, int focus);

#endif
