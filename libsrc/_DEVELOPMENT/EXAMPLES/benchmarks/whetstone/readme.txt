WHETSTONE 1.2
=============

https://en.wikipedia.org/wiki/Whetstone_(benchmark)
http://www.electronic-engineering.ch/fpga/projects/whetstone/Whetstone.pdf
http://www.roylongbottom.org.uk/whetstone.htm

Original Source Code:
http://www.netlib.org/benchmark/whetstone.c

Check Out How the Z80 Compares to Historical Results:
http://www.roylongbottom.org.uk/whetstone.htm#anchorIndex

The base source code used for benchmarking is in this directory.

This is modified as little as possible to be compilable by the
compilers under test and any modified source code is present in
subdirectories.

Floating point performance is measured in KWIPS (kilo-whetstones
per second) or MWIPS (millions of whetstone per second) which
can be computed from:

KWIPS = 100*LOOPS*ITERATIONS / Execution_Time
MWIPS = KWIPS / 1000

In these timing tests, LOOPS will default to 10 and ITERATIONS
will default to 1.

As can be expected, the implementation's floating point precision
will greatly impact on comparative accuracy and performance so
the floating point format must be reported along with KWIPS rating
to allow for suitable insight into results.

When compiling whetstone 1.2, several defines are possible:

/*
 * COMMAND LINE DEFINES
 * 
 * -DSTATIC
 * Use static variables instead of locals.
 *
 * -DPRINTOUT
 * Enable printing of intermediate results.
 *
 * -DTIMER
 * Insert asm labels into source code at timing points (Z88DK).
 *
 * -DTIMEFUNC
 * Platform timer functions are available (must supply timer functions).
 *
 * -DCOMMAND
 * Enable command line processing (LOOP=10, II=1 if disabled).
 *
 */

STATIC can be defined freely for best compiler performance.

All compiles are first checked for correctness by running the program
with PRINTOUT defined.  After correctness is verified, time should be
measured with PRINTOUT undefined so that execution time of printf is not
measured.

=====================================

RESULTS ARE ALLOWED TO VARY SOMEWHAT
DUE TO DIFFERENCES IN FLOAT PRECISION

N=      0 J=      0 K=      0
X1=  1.0000e+00 X2= -1.0000e+00
X3= -1.0000e+00 X4= -1.0000e+00

N=    120 J=    140 K=    120
X1= -6.8342e-02 X2= -4.6264e-01
X3= -7.2972e-01 X4= -1.1240e+00

N=    140 J=    120 K=    120
X1= -5.5336e-02 X2= -4.4744e-01
X3= -7.1097e-01 X4= -1.1031e+00

N=   3450 J=      1 K=      1
X1=  1.0000e+00 X2= -1.0000e+00
X3= -1.0000e+00 X4= -1.0000e+00

N=   2100 J=      1 K=      2
X1=  6.0000e+00 X2=  6.0000e+00
X3= -7.1097e-01 X4= -1.1031e+00

N=    320 J=      1 K=      2
X1=  4.9041e-01 X2=  4.9041e-01
X3=  4.9039e-01 X4=  4.9039e-01

N=   8990 J=      1 K=      2
X1=  1.0000e+00 X2=  1.0000e+00
X3=  9.9994e-01 X4=  9.9994e-01

N=   6160 J=      1 K=      2
X1=  3.0000e+00 X2=  2.0000e+00
X3=  3.0000e+00 X4= -1.1031e+00

N=      0 J=      2 K=      3
X1=  1.0000e+00 X2= -1.0000e+00
X3= -1.0000e+00 X4= -1.0000e+00

N=    930 J=      2 K=      3
X1=  8.3467e-01 X2=  8.3467e-01
X3=  8.3467e-01 X4=  8.3467e-01

=====================================

TIMEFUNC allows the host system's own clock to be used for timing so
that results can be printed out as part of the execution.  See source
code for details.

TIMER is defined for Z88DK compiles so that assembly labels are inserted
into the code at time begin and time stop points.

When COMMAND is not defined, LOOP=10 and II=1.

For a timed run, the program is compiled and simulated by TICKS.  TICKS
must be given a start address to start timing and a stop address to stop
timing.  In Z88DK compiles these show up in the map file.  Other compilers'
output may have to be disassembled to locate the correct address range.

The output of TICKS is a cycle count.  To convert to time in seconds:

Execution_Time = CYCLE_COUNT / FCPU
where FCPU = clock frequency of Z80 in Hz.

This time can be plugged into the whetstone formulas above to compute
exact KWIPS performance.


RESULTS
=======

1.
HITECH C CPM V309
** Several results with large error (maybe should be DQ)
24 bit mantissa + 8 bit exponent
7605 bytes less cpm overhead

cycle count  = 639413871
time @ 4MHz  = 639413871 / 4*10^6 = 159.8535 seconds
KWIPS        = 100*10*1 / 159.8535 = 6.2557
MWIPS        = 6.2557 / 1000 = 0.0062557

2.
IAR Z80 V4.06A
24 bit mantissa + 8 bit exponent
6524 bytes less small amount

cycle count  = 732360277
time @ 4MHz  = 732360277 / 4*10^6 = 183.0901 seconds
KWIPS        = 100*10*1 / 183.0901 = 5.4618
MWIPS        = 5.4618 / 1000 = 0.0054618

3.
Z88DK April 28, 2021
zsdcc #12070 / new c library / math48
24 bit mantissa + 8 bit exponent (internally 40+8)
6221 bytes less page zero

cycle count  = 914412771
time @ 4MHz  = 914412771 / 4x10^6 = 228.6031 seconds
KWIPS        = 100*10*1 / 228.6031 = 4.3743
MWIPS        = 4.3743 / 1000 = 0.0043743

4.
Z88DK January 3, 2022
classic/zsdcc #12070/math48
24 bit mantissa + 8 bit exponent (internally 40+8)
7064 bytes less page zero

cycle count  = 917739704
time @ 4MHz  = 917739704 / 4x10^6 = 229.4349 seconds
KWIPS        = 100*10*1 / 230.1954 = 4.3585
MWIPS        = 4.3585 / 1000 = 0.0043585

5.
Z88DK April 28, 2021
sccz80 / new c library / math48 float package
40 bit mantissa + 8 bit exponent
5362 bytes less page zero

cycle count  = 972899568
time @ 4MHz  = 972899568 / 4x10^6 = 243.2248 seconds
KWIPS        = 100*10*1 / 243.2248 = 4.1114
MWIPS        = 4.1114 / 1000 = 0.0041114

6.
Z88DK January 3, 2022
sccz80 / classic c library / genmath float package
40 bit mantissa + 8 bit exponent
6411 bytes less page zero

cycle count  = 1284172870
time @ 4MHz  = 1284172870 / 4x10^6 = 321.0432 seconds
KWIPS        = 100*10*1 / 320.8179 = 3.1148
MWIPS        = 3.1148 / 1000 = 0.0031148

7.
SDCC 3.6.5 #9842 (MINGW64)
24 bit mantissa + 8 bit exponent
14379 bytes less page zero

cycle count  = 2184812093
time @ 4MHz  = 2184812093 / 4x10^6 = 546.2030 seconds
KWIPS        = 100*10*1 / 546.2030 = 1.8308
MWIPS        = 1.8308 / 1000 = 0.0018308

SDCC implements its float library in C.

8.
Z88DK April 30, 2021
zsdcc #12250 / new c library / math32
24 bit mantissa + 8 bit exponent
10113 bytes less page zero

cycle count  = 576187434
time @ 4MHz  = 576187434 / 4x10^6 = 144.0468 seconds
KWIPS        = 100*10*1 / 144.0468 = 6.9421
MWIPS        = 6.9421 / 1000 = 0.0069421

9.
Z88DK January 3, 2022
sccz80 / new c library / math32
24 bit mantissa + 8 bit exponent
8861 bytes less page zero

cycle count  = 568209557
time @ 4MHz  = 568209557 / 4x10^6 = 142.0524 seconds
KWIPS        = 100*10*1 / 142.0524 = 7.0396
MWIPS        = 7.0396 / 1000 = 0.0070396

10.
Z88DK January 3, 2022
sccz80 / classic c library / MBF32
Microsoft 32-bit math 24 bit mantissa + 8 bit exponent
6237 bytes less page zero

cycle count  = 547041311
time @ 4MHz  = 547041311 / 4x10^6 = 136.7603 seconds
KWIPS        = 100*10*1 / 136.7603 = 7.3121
MWIPS        = 7.3121 / 1000 = 0.0073121

11.
Z88DK January 3, 2022
sccz80 / classic c library / 8085 CPU / MBF32
Microsoft 32-bit math 24 bit mantissa + 8 bit exponent
6133 bytes less page zero

cycle count  = 549592505
time @ 4MHz  = 549592505 / 4x10^6 = 137.3981 seconds
KWIPS        = 100*10*1 / 137.3981 = 7.2781
MWIPS        = 7.2781 / 1000 = 0.0072781

DISQ.
HITECH C MSDOS V750
24 bit mantissa + 8 bit exponent
7002 bytes exact

Incorrect results.
HTC V750 does not have a functioning float library.
