
# Script to monitor the cache behaviour on two kind of memory access
# compile example doing CD to this folder and run "make app_st" then copy binary to ST board.
perf stat ./cacheperformance_1_st  sequential
# output from an STM32MP157D-DK1
#Performance counter stats for './cacheperformance_1_st sequential':
# 
#              66.16 msec task-clock                       #    0.963 CPUs utilized
#                  1      context-switches                 #   15.116 /sec
#                  0      cpu-migrations                   #    0.000 /sec
#               1059      page-faults                      #   16.007 K/sec
#           49116844      cycles                           #    0.742 GHz
#           25743809      instructions                     #    0.52  insn per cycle
#            2002548      branches                         #   30.270 M/sec
#             159550      branch-misses                    #    7.97% of all branches
# 
#        0.068698418 seconds time elapsed
# 
#        0.058470000 seconds user
#        0.011694000 seconds sys
perf stat ./cacheperformance_1_st  stride
# output from an STM32MP157D-DK1
#  Performance counter stats for './cacheperformance_1_st stride':
# 
#              27.72 msec task-clock                       #    0.922 CPUs utilized
#                  0      context-switches                 #    0.000 /sec
#                  0      cpu-migrations                   #    0.000 /sec
#               1062      page-faults                      #   38.314 K/sec
#           17993304      cycles                           #    0.649 GHz
#            8205761      instructions                     #    0.46  insn per cycle
#             967080      branches                         #   34.890 M/sec
#             158511      branch-misses                    #   16.39% of all branches
# 
#        0.030076023 seconds time elapsed
# 
#        0.000000000 seconds user
#        0.031582000 seconds sys
# 
# 