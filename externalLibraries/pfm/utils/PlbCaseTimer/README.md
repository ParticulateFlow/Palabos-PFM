## PlbCaseTimer

`class PlbCaseTimer` is a helper class to obtain high-level profiling data from a case. For more detailed profiling, tools like perf or gprof are better suited, but for a quick comparison, `PlbCaseTimer` might just do fine.

#### Use

##### Initialization

Initialization is simple: the constructor takes no arguments
```
PlbCaseTimer timer;
```
Custom timers can be added by calling
```
timer.registerCustom("foobar")
```
In addition, a timer named "global" is always present.

##### Use during simulation

Custom timers can be started and stopped at any location in the code. For instance, to measure the time spent in the LB collision and streaming, the following code might be used:
```
timer.registerCustom("CollideAndStream");
timer.registerCustom("OtherStuff");
timer.startGlobal();
for(plint iT=0;iT<maxIter;iT++){
  timer.start("CollideAndStream");
  lattice.collideAndStream();
  timer.stop("CollideAndStream");
  
  timer.start("OtherStuff");
  /* do other stuff */
  timer.stop("OtherStuff");
}
timer.stopGlobal();
```
This creates two timers, "CollideAndStream" and "OtherStuff", on top of the predefined timer "global". At the end of a simulation, the results can be written by calling
```
timer.writeLog(Parallel_ostream &out);
```
where `Parallel_ostream` is the base class of both filestreams and `pcout`/`pcerr`. A sample output could look like
```
Timer CollideAndStream : 36.938209 seconds, 98.57784292%
Timer Logging : 0.000134 seconds, 0.0003576088638%
Timer Output : 0.000724 seconds, 0.001932155354%
Timer Setup : 0.530791 seconds, 1.416534078%
Timer global : 37.471107 seconds, 100%
Timer other : 0.001249 seconds, 0.003333234857%
```
Additionally, the data can be written as a json file by calling
```
timer.writeLogJSON(std::string filename);
```

##### Cleanup

No cleanup is required.
