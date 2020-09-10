# cpusm
cpu statistics monitor for linux cli


cpusm is a cli-based system statistics monitor for linux. Developed using C, cpusm execution levies a relatively small system penalty, making it the ideal cli application for realtime statistics monitoring.

The code is not well documented, but I'm working on it [see cpusm.h for a breakdown of the totally intuitive and highly descriptive array, em[]]



cpusm build instructions:

	git clone https://github.com/j2ckson/cpusm.git
	cd ./cpusm
	make
	sudo make install


cpusm execution:

	cpusm --help

	cpusm [option]<value>...[option]<value>
	_______________________________________________________________________________________________________________
	command-line options [* default]

	-a || --core: cpusm core affinity
	-b || --core-opt: cpusm core optimization [<0*> kernel, <1> min.cpu, <2> max.cpu, <3> random]
	-c || --colour: stdout/terminal display colour [0* - 7]
	-d || --duration: cpusm execution duration [x.y seconds]
	-e || --slip: interval affecting frequency and temperature statistics [1/slip >=1, 4*]
	-f || --file: <filename>: log destination
	-g || --coefficient: statistics sampling interval coefficient [g * t, 1*]
	-h || --help: execution assistance
	-i || --shortmode: cpusm start information mode [0* - normal, 1 - abbreviated]
	-j || --step: cpusm statistics output step interval [1/j * r * u]
	-k || --mode: cpusm statistics output mode [0 - uni, 1* - multi]
	-l || --log cpusm output mode [0* - stdout/terminal, 1 - stdout/terminal+file, 2 - file]
	-m || --display-size: [cpusm statistics mode [0* - normal, 1 - abbreviated]
	-n || --network: network interface to monitor [-k1 || -k0 -v1]
	-o || --priority: cpusm application execution priority [0* - 99]
	-p || --timing: time tracking alogrithm [0* - dynamic, 1 - static]
	-q || --precision: time tracking precision [0 - off, 1* - on]
	-r || --rate: sample rate per second [1 - 1000, 10*]
	-s || --scrolling: output display mode [0* - fixed-line, 1 - scrolling]
	-t || --interval: sample interval [s.n seconds, 0.1*, minimum == 0.001]
	-u || --rolling: cpusm statistics rolling average interval [1* - 999] seconds
	-v || --category: cpusm statistics category
	        0: cpu.kernel mode
	        1: network mode [use in conjunction with -n]
	        2: cpu.core mode
	        3: thermal mode
	        4*: core.frequency mode
	-w || --scheduler: cpusm process scheduling algorithm [0* - 6]
	-x || --header-interval: periodic column heading display [0* == off, *40 == runtime switch default]
	-y || --nice: cpusm execution nice value [-20 to 20, 0*]
	-z || --samples: total samples [>=0, 0* - no limit]
	_______________________________________________________________________________________________________________
	runtime options

	<h>: show runtime help
	<a>: switch core affinity [base process only - established threads will not switch
	<c>: switch stdout colour
	<q>: quit cpusm
	<j>: enable/disable statistics step display
	<l>: enable/disable colour-mode focus scanning
	<m>: enable/switch colour-mode focus
	<s>: display cpusm summary statistics
	<x>: enable/disable periodic header printing
	<z>: restart cpusm
	<=>: display cpusm min/max/avg statistics
	</>: switch output mode [fixed-line/scrolling]
	<.>: display column headings
	<`>: switch cpusm cpu utilization tracking [core/cpu]
	<,>: enable/diable stdout [file logging-mode]]
	<SPACE>||<v>: switch display mode [cpu.kernel/cpu.core/core.frequency/thermal/network]



cpusm example output:


	jack@zenux:~/dev/c/active$ cpusm -z2 -v0    

	terminal geometry: 59 rows x 263 columns                                                                                                                                                                                                                               
	maximum rate: 100 [samples/second]                                                                                                                                                                                                                                     
	scheduler: 0 priority: 0                                                                                                                                                                                                                                               
	process 10287 user-defined priority: 0                                                                                                                                                                                                                                 
	sample interval/rate unspecified - assuming default: -t 0.1                                                                                                                                                                                                            

	┌CPU STATISTICS MONITOR───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                    

	┌─SYSTEM.ATTRIBUTES────────────────────────────────────────┐                                                    ┌─HEADING.KEY─────────────────────────────────────────────────────────────────────┐                                                                    

	  system.type: linux                                                                                              10287: application pid                                                                                                                               
	  hostname: zenux                                                                                                 a.freq: average core frequency [per interval]                                                                                                        
	  o/s.version: 5.6.0-1020-oem                                                                                     a.°C: zone temperature [degrees centigrade]                                                                                                          
	  cpu.model:  Intel(R) Core(TM) i9-9980HK CPU @ 2.40GHz                                                           free: free system memory [percentage]                                                                                                                
	  cpu.cores: 16 [0-15]                                                                                            avai: available system memory [percentage]                                                                                                           
	  min.freq: 800.00MHz                                                                                             buff: buffered system memory [percentage]                                                                                                            
	  max.freq: 5000.00MHz                                                                                            cach: cached system memory [percentage]                                                                                                              
	  governor: powersave                                                                                             load.avg: system load averages [per interval]                                                                                                        
	  sys.memory: 32683724 kB                                                                                         01mn: one minute load average                                                                                                                        
	  sys.disks: 5 [nvme0n1p4][nvme0n1p1][fuse][sdb1][sda2]                                                           05mn: five minute load average
	  net.device: n/a                                                                                                 15mn: fifteen minute load average
	  start.time: 01:58:23.36                                                                                         procs: system processes
	  current.time: 01:58:23.36                                                                                       rnng: running system processes
	  exp.run.time: 00:00:00.20                                                                                       actv: active system processes [running+idle]
	  interval: 0.100                                                                                                 cpu.avg: cpu utilization averages [percentage]
	  int.samples/s: 0.00                                                                                             spot: per-interval
	  exp.samples/s: 10.00                                                                                            cuml: cumulative
	  int.samples: 0                                                                                                  r.nnxn: rolling average [n seconds]
	  exp.samples: 2                                                                                                  user: normal-priority cpu utilization
	                                                                                                                  nice: low-prioritycpu utilization
	┌─RUNTIME.OPTIONS────────────────────────────────────────┐                                                        syst: system-mode cpu utilization
	                                                                                                                  idle: idle-task cpu utilization
	  <h>: show runtime help                                                                                          iowa: i/o-wait-based cpu utilization
	  <c>: switch display colour characteristics                                                                      irqs: interrupt-based cpu utilization
	  <q>: quit cpusm                                                                                                 sirq: soft-irq-based cpu utilization
	  <s>: display interim cpusm summary statistics                                                                   maxi: maximum cpu utilization
	  <x>: enable/disable periodic header printing                                                                    count: total samples captured
	  <j>: enable/disable step display                                                                                algn: collection/processing thread alignment
	  <m>: shift field focus                                                                                          pcpu: cumulative application cpu utilization
	  <z>: restart cpusm                                                                                              runtime: cumulative application runtime
	  </>: switch output mode [fixed-line/scrolling]                                                                  schd: kernel scheduling algorithm
	  <.>: display column headings                                                                                    pri: kernel scheduling priority
	  <=>: display interim min/max statistics                                                                         nce: application priority ['nice' value]
	  <l>: enable/disable shifting field focus                                                                        core: virtual core affinity
	  <1>: enable/disable precise time-tracking
	  <`>: switch pcpu accounting [per-core/aggregate]
	  <v>: switch statistics display length]

	cpusm -z2 -v0

	10287.metrics────────────────────────┐┃┌cpu.statistics────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
	                                      ┃
	  schd:nrml  pri:00  nce:000  core:k  ┃
	─────────────────────────────────────┐┃┌────────────┐┌─────────memory──────────┐┌──────load.avg──────┐┌────procs───┐┌──────cpu.avg──────┐┌───────────────────kernel.performance───────────────────┐
	count     algn    pcpu        runtime ┃ a.freq  a.°C  free   avai   buff   cach   01mn   05mn   15mn   rnng   actv   spot   cuml   r.001  user   nice   syst   idle   iowa   irqs   sirq   maxi

	0         100.00  0.000        00.201   4545.7  52.5  84.28  88.90  0.696  5.464  0.110  0.140  0.140  00002  01119  9.375  4.688  4.688  6.875  0.000  1.250  90.62  0.625  0.000  0.625  9.375
	──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
	minimum   100.00  0.000      min.stat   4545.7  52.5  84.28  88.90  0.696  5.464  0.110  0.140  0.140  00002  01119  9.375  4.688  4.688  6.875  0.000  1.250  90.62  0.625  0.000  0.625  9.375
	maximum   100.00  0.000      max.stat   4545.7  52.5  84.28  88.90  0.696  5.464  0.110  0.140  0.140  00002  01119  9.375  4.688  4.688  6.875  0.000  1.250  100.0  0.625  0.000  0.625  9.375
	average   100.00  0.000      avg.stat   4545.7  52.5  84.28  88.90  0.696  5.464  0.110  0.140  0.140  00002  01119  4.688  2.344  2.344  3.438  0.000  0.625  95.31  0.312  0.000  0.312  4.688


	 start.time:            01:58:23.36
	 end.time:              01:58:23.56
	 run.time:              00:00:00.20
	 active.time:           00:00:00.20
	 cpu.time:              0:00:00.002
	 %cpu:                  1.070/0.067
	 actual.samples/s:      10.00
	 exp.samples/s:         10.00
	 actual.samples:        2
	 exp.samples:           2
	 nominal.variation:     0.00
	 relative.variation:    0.0000


	jack@zenux:~$ cpusm -z2 -i1 -v1

	terminal geometry: 59 rows x 263 columns                                                                                                                                                                                                                               
	maximum rate: 100 [samples/second]                                                                                                                                                                                                                                     
	scheduler: 0 priority: 0                                                                                                                                                                                                                                               
	process 9636 user-defined priority: 0                                                                                                                                                                                                                                  
	network interface unspecified - using device bound to default route: -n wlo1                                                                                                                                                                                           
	sample interval/rate unspecified - assuming default: -t 0.1                                                                                                                                                                                                            

	9636.metrics─────────────────────────┐┃┌network.statistics────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                    
	                                      ┃                                                                                                                                                                                                                                
	  schd:nrml  pri:00  nce:000  core:k  ┃                                                                                                                                                                                                                                
	─────────────────────────────────────┐┃┌────────────┐┌─────────memory──────────┐┌──────load.avg──────┐┌────procs───┐┌──────cpu.avg──────┐┌──────────wlo1.rx──────────┐┌──────────wlo1.tx──────────┐                                                                    
	count     algn    pcpu        runtime ┃ a.freq  a.°C  free   avai   buff   cach   01mn   05mn   15mn   rnng   actv   spot   cuml   r.001  spot   cuml   r.001  p/sec   spot   cuml   r.001  p/sec                                                                      

	0         100.00  0.000        00.201   4480.3  52.3  84.78  89.33  0.687  5.304  0.100  0.250  0.120  00002  01114  9.317  4.658  4.658  0.000  0.000  0.000  0.000   0.000  0.000  0.000  0.000                                                                      
	──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                    
	minimum   100.00  0.000      min.stat   4480.3  52.3  84.78  89.33  0.687  5.304  0.100  0.250  0.120  00002  01114  9.317  4.658  4.658  0.000  0.000  0.000  0.000   0.000  0.000  0.000  0.000                                                                      
	maximum   100.00  0.000      max.stat   4480.3  52.3  84.78  89.33  0.687  5.304  0.100  0.250  0.120  00002  01114  9.317  4.658  4.658  0.000  0.000  0.000  0.000   0.000  0.000  0.000  0.000                                                                      
	average   100.00  0.000      avg.stat   4480.3  52.3  84.78  89.33  0.687  5.304  0.100  0.250  0.120  00002  01114  4.658  2.329  2.329  0.000  0.000  0.000  0.000   0.000  0.000  0.000  0.000                                                                      


	 start.time:            01:42:19.86                                                                                                                                                                                                                                    
	 end.time:              01:42:20.06                                                                                                                                                                                                                                    
	 run.time:              00:00:00.20                                                                                                                                                                                                                                    
	 active.time:           00:00:00.20                                                                                                                                                                                                                                    
	 cpu.time:              0:00:00.003                                                                                                                                                                                                                                    
	 %cpu:                  1.262/0.079                                                                                                                                                                                                                                    
	 actual.samples/s:      10.00                                                                                                                                                                                                                                          
	 exp.samples/s:         10.00                                                                                                                                                                                                                                          
	 actual.samples:        2                                                                                                                                                                                                                                              
	 exp.samples:           2                                                                                                                                                                                                                                              
	 nominal.variation:     0.00                                                                                                                                                                                                                                           
	 relative.variation:    0.0000                                                                                                                                                                                                                                         


	jack@zenux:~/dev/c/active$ cpusm -z2 -i1 -v2 -m1

terminal geometry: 59 rows x 263 columns                                                                                                                                                                                                                               
maximum rate: 100 [samples/second]                                                                                                                                                                                                                                     
scheduler: 0 priority: 0                                                                                                                                                                                                                                               
process 10584 user-defined priority: 0                                                                                                                                                                                                                                 
sample interval/rate unspecified - assuming default: -t 0.1                                                                                                                                                                                                            

10584.metrics─────────┐┃┌core.statistics────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                                          
                       ┃                                                                                                                                                                                                                                               
 s:0  p:00  n:000  c:k ┃                                                                                                                                                                                                                                               
──────────────────────┐┃┌────────────┐┌──────cpu.avg──────┐┌────────────────────────────────────────────────core.performance────────────────────────────────────────────────┐                                                                                          
count          runtime ┃ a.freq  a.°C  spot   cuml   r.001  cpu.0  cpu.1  cpu.2  cpu.3  cpu.4  cpu.5  cpu.6  cpu.7  cpu.8  cpu.9  cpu.10 cpu.11 cpu.12 cpu.13 cpu.14 cpu.15                                                                                            

0               00.200   4572.4  52.9  8.295  4.148  4.148  0.000  18.18  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  60.00  0.000  54.55  0.000                                                                                             
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                                          
minimum       min.stat   4572.4  52.9  8.295  4.148  4.148  0.000  18.18  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  60.00  0.000  54.55  0.000                                                                                             
maximum       max.stat   4572.4  52.9  8.295  4.148  4.148  0.000  18.18  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  60.00  0.000  54.55  0.000                                                                                             
average       avg.stat   4572.4  52.9  4.148  2.074  2.074  0.000  9.091  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  0.000  30.00  0.000  27.27  0.000                                                                                             


 start.time:            02:07:47.13                                                                                                                                                                                                                                    
 end.time:              02:07:47.33                                                                                                                                                                                                                                    
 run.time:              00:00:00.20                                                                                                                                                                                                                                    
 active.time:           00:00:00.20                                                                                                                                                                                                                                    
 cpu.time:              0:00:00.002                                                                                                                                                                                                                                    
 %cpu:                  0.833/0.052                                                                                                                                                                                                                                    
 actual.samples/s:      10.00                                                                                                                                                                                                                                          
 exp.samples/s:         10.00                                                                                                                                                                                                                                          
 actual.samples:        2                                                                                                                                                                                                                                              
 exp.samples:           2                                                                                                                                                                                                                                              
 nominal.variation:     0.00                                                                                                                                                                                                                                           
 relative.variation:    0.0000                                                                                                                                                                                                                                         


jack@zenux:~/dev/c/active$ cpusm -z2 -i1 -v3 -m1

terminal geometry: 59 rows x 263 columns                                                                                                                                                                                                                               
maximum rate: 100 [samples/second]                                                                                                                                                                                                                                     
scheduler: 0 priority: 0                                                                                                                                                                                                                                               
process 10604 user-defined priority: 0                                                                                                                                                                                                                                 
sample interval/rate unspecified - assuming default: -t 0.1                                                                                                                                                                                                            

10604.metrics─────────┐┃┌thermal.statsistics───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐                                                                                   
                       ┃                                                                                                                                                                                                                                               
 s:0  p:00  n:000  c:k ┃                                                                                                                                                                                                                                               
──────────────────────┐┃┌────────────┐┌──────cpu.avg──────┐┌─────────────────────────────────────────────────────── sys.temp───────────────────────────────────────────────────────┐                                                                                   
count          runtime ┃ a.freq  a.°C  spot   cuml   r.001  °C00   °C01   °C02   °C03   °C04   °C05   °C06   °C07   °C08   °C09   °C10   °C11   °C12   °C13   °C14   °C15   °C16

0               00.201   4490.5  53.0  8.075  4.037  4.037  51.00  52.85  52.85  37.85  37.85  37.85  77.00  53.00  77.00  53.00  53.00  52.00  52.00  56.00  53.00  62.00  42.00
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
minimum       min.stat   4490.5  53.0  8.075  4.037  4.037  51.00  52.85  52.85  37.85  37.85  37.85  77.00  53.00  77.00  53.00  53.00  52.00  52.00  56.00  53.00  62.00  42.00
maximum       max.stat   4490.5  53.0  8.075  4.037  4.037  51.00  52.85  52.85  37.85  37.85  37.85  77.00  53.00  77.00  53.00  53.00  52.00  52.00  56.00  53.00  62.00  42.00
average       avg.stat   4490.5  53.0  4.037  2.019  2.019  51.00  52.85  52.85  37.85  37.85  37.85  77.00  53.00  77.00  53.00  53.00  52.00  52.00  56.00  53.00  62.00  42.00


 start.time:            02:07:58.62
 end.time:              02:07:58.82
 run.time:              00:00:00.20
 active.time:           00:00:00.20
 cpu.time:              0:00:00.003
 %cpu:                  1.336/0.083
 actual.samples/s:      10.00
 exp.samples/s:         10.00
 actual.samples:        2
 exp.samples:           2
 nominal.variation:     0.00
 relative.variation:    0.0000


jack@zenux:~/dev/c/active$ cpusm -z2 -i1 -v4 -m1

terminal geometry: 59 rows x 263 columns
maximum rate: 100 [samples/second]
scheduler: 0 priority: 0
process 10623 user-defined priority: 0
sample interval/rate unspecified - assuming default: -t 0.1

10623.metrics─────────┐┃┌frequency.statistics───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
                       ┃
 s:0  p:00  n:000  c:k ┃
──────────────────────┐┃┌────────────┐┌──────cpu.avg──────┐┌─────────────────────────────────────────────────core.frequency─────────────────────────────────────────────────┐
count          runtime ┃ a.freq  a.°C  spot   cuml   r.001  cpu.0  cpu.1  cpu.2  cpu.3  cpu.4  cpu.5  cpu.6  cpu.7  cpu.8  cpu.9  cpu.10 cpu.11 cpu.12 cpu.13 cpu.14 cpu.15

0               00.200   4487.2  52.7  10.30  5.152  5.152  4613.4 4762.3 4647.1 4532.5 4589.7 4553.8 4335.9 4697.7 4552.7 4822.3 4756.1 4451.3 4603.2 4334.5 4502.3 3040.5
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
minimum       min.stat   4487.2  52.7  10.30  5.152  5.152  4613.4 4762.3 4647.1 4532.5 4589.7 4553.8 4335.9 4697.7 4552.7 4822.3 4756.1 4451.3 4603.2 4334.5 4502.3 3040.5
maximum       max.stat   4487.2  52.7  10.30  5.152  5.152  4613.4 4762.3 4647.1 4532.5 4589.7 4553.8 4335.9 4697.7 4552.7 4822.3 4756.1 4451.3 4603.2 4334.5 4502.3 3040.5
average       avg.stat   4487.2  52.7  5.152  2.576  2.576  4613.4 4762.3 4647.1 4532.5 4589.7 4553.8 4335.9 4697.7 4552.7 4822.3 4756.1 4451.3 4603.2 4334.5 4502.3 3040.5


 start.time:            02:08:05.19
 end.time:              02:08:05.39
 run.time:              00:00:00.20
 active.time:           00:00:00.20
 cpu.time:              0:00:00.002
 %cpu:                  0.838/0.052
 actual.samples/s:      10.00
 exp.samples/s:         10.00
 actual.samples:        2
 exp.samples:           2
 nominal.variation:     0.00
 relative.variation:    0.0000