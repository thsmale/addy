/*
 * Lets play a game 
 * How many times will each print statement occur in the file
 * pritnf("child\n");
 * printf("parent\n");
 * printf("uno\n");
 * printf("once\n");
 * printf("end\n");
 * i = number_of_forks;
 * i = 1
 * i = 2
 * i = 10
 for (int i = 0; i < 100; ++i) 
 ./a.out | grep uno | wc -l 
 ./a.out | grep once | wc -l 
 ./a.out | grep one | wc -l
 How many times will these be printed out? 
 8, 

uno
once
should only print one
pid 0 created
child
uno
once
should only print one
pid 34936 created
parent
Process 34936 exited peacefully with status 0
csv, 34936, 0, 1676363449
CSV TABLE
Limits on consumption of system resources by the current process and each process it creates
----|------------------------
current, max, unit, description
0, 9223372036854775807, bytes, largest *core* file size that may be created
9223372036854775807, 9223372036854775807, seconds, max CPU used by process <pid>
9223372036854775807, 9223372036854775807, bytes, max data segement for process <pid>
9223372036854775807, 9223372036854775807, bytes, largest file size that may be created
256, 9223372036854775807, decimal, open files for this process <pid>
2784, 2784, decimal, simulateneous processes for this <pid>
9223372036854775807, 9223372036854775807, bytes, physical memory allocated to processes <pid>
8388608, 67104768, bytes, max stack size for processes <pid>
Number of processes at exit 0
