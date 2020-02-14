# HW2 FM algorithm
### File hierarchy
```
+-- src/
|   +-- main.cpp
|   +-- ds.h
|   +-- read_file.h
|   +-- read_file.cpp
|   +-- fm.h
|   +-- fm.cpp
+-- obj/
|   +-- objective codes
+-- benchmark/
|   +-- benchmark files
+-- BC-HW/BC_P76071218/
|   +-- .clusters, .cnets files
--- Makefile
|
--- FM_P76071218
```

### Platform
- Ubuntu 18.04.1 LTS
- g++ 7.4.0

### Usage
- compile
```
$ make
```
- run benchmark 1 ~ 5
```
$ make test1
$ make test2
$ make test3
$ make test4
$ make test5
```
- run all benchmark
```
$ make test_all
```

- result
```
$ make test_all

Reading files...
Initial Status
Number of pins: 923513
Group1: 14914680
Group2: 22371612
Cut size: 20989
------------------------------------
Iteration 1
Size of Group1: 14913636
Size of Group2: 22372656
Cut Size: 8255

------------------------------------
Iteration 2
Size of Group1: 14915004
Size of Group2: 22371288
Cut Size: 6448

------------------------------------
Iteration 3
Size of Group1: 14915352
Size of Group2: 22370940
Cut Size: 6357

------------------------------------
Iteration 4
Size of Group1: 14913648
Size of Group2: 22372644
Cut Size: 6356

------------------------------------
Iteration 5
Size of Group1: 14913744
Size of Group2: 22372548
Cut Size: 6355

------------------------------------
Iteration 6
Size of Group1: 14913744
Size of Group2: 22372548
Cut Size: 6355

Finish.


Reading files...
Initial Status
Number of pins: 1045699
Group1: 13456272
Group2: 20184216
Cut size: 17849
------------------------------------
Iteration 1
Size of Group1: 13456404
Size of Group2: 20184084
Cut Size: 8901

------------------------------------
Iteration 2
Size of Group1: 13455684
Size of Group2: 20184804
Cut Size: 8012

------------------------------------
Iteration 3
Size of Group1: 13456728
Size of Group2: 20183760
Cut Size: 7961

------------------------------------
Iteration 4
Size of Group1: 13455420
Size of Group2: 20185068
Cut Size: 7913

------------------------------------
Iteration 5
Size of Group1: 13455768
Size of Group2: 20184720
Cut Size: 7863

------------------------------------
Iteration 6
Size of Group1: 13455408
Size of Group2: 20185080
Cut Size: 7838

------------------------------------
Iteration 7
Size of Group1: 13455408
Size of Group2: 20185080
Cut Size: 7838

Finish.


Reading files...
Initial Status
Number of pins: 1843852
Group1: 27920100
Group2: 41879400
Cut size: 107973
------------------------------------
Iteration 1
Size of Group1: 27920592
Size of Group2: 41878908
Cut Size: 31981

------------------------------------
Iteration 2
Size of Group1: 27919812
Size of Group2: 41879688
Cut Size: 18486

------------------------------------
Iteration 3
Size of Group1: 27919008
Size of Group2: 41880492
Cut Size: 12556

------------------------------------
Iteration 4
Size of Group1: 27920268
Size of Group2: 41879232
Cut Size: 12180

------------------------------------
Iteration 5
Size of Group1: 27919836
Size of Group2: 41879664
Cut Size: 12133

------------------------------------
Iteration 6
Size of Group1: 27920112
Size of Group2: 41879388
Cut Size: 12097

------------------------------------
Iteration 7
Size of Group1: 27918948
Size of Group2: 41880552
Cut Size: 12037

------------------------------------
Iteration 8
Size of Group1: 27920688
Size of Group2: 41878812
Cut Size: 11907

------------------------------------
Iteration 9
Size of Group1: 27919308
Size of Group2: 41880192
Cut Size: 11904

------------------------------------
Iteration 10
Size of Group1: 27920676
Size of Group2: 41878824
Cut Size: 11888

------------------------------------
Iteration 11
Size of Group1: 27919008
Size of Group2: 41880492
Cut Size: 11886

------------------------------------
Iteration 12
Size of Group1: 27920688
Size of Group2: 41878812
Cut Size: 11875

------------------------------------
Iteration 13
Size of Group1: 27920688
Size of Group2: 41878812
Cut Size: 11875

Finish.


Reading files...
Initial Status
Number of pins: 2684818
Group1: 24403149
Group2: 36604683
Cut size: 286337
------------------------------------
Iteration 1
Size of Group1: 24403545
Size of Group2: 36604287
Cut Size: 102531

------------------------------------
Iteration 2
Size of Group1: 24403041
Size of Group2: 36604791
Cut Size: 45534

------------------------------------
Iteration 3
Size of Group1: 24403041
Size of Group2: 36604791
Cut Size: 45534

Finish.


Reading files...
Initial Status
Number of pins: 3022434
Group1: 33069609
Group2: 49604310
Cut size: 329913
------------------------------------
Iteration 1
Size of Group1: 33069924
Size of Group2: 49603995
Cut Size: 143925

------------------------------------
Iteration 2
Size of Group1: 33069501
Size of Group2: 49604418
Cut Size: 75050

------------------------------------
Iteration 3
Size of Group1: 33069663
Size of Group2: 49604256
Cut Size: 69260

------------------------------------
Iteration 4
Size of Group1: 33069402
Size of Group2: 49604517
Cut Size: 57168

------------------------------------
Iteration 5
Size of Group1: 33069132
Size of Group2: 49604787
Cut Size: 55586

------------------------------------
Iteration 6
Size of Group1: 33069870
Size of Group2: 49604049
Cut Size: 54585

------------------------------------
Iteration 7
Size of Group1: 33069429
Size of Group2: 49604490
Cut Size: 45262

------------------------------------
Iteration 8
Size of Group1: 33069330
Size of Group2: 49604589
Cut Size: 45092

------------------------------------
Iteration 9
Size of Group1: 33069969
Size of Group2: 49603950
Cut Size: 45033

------------------------------------
Iteration 10
Size of Group1: 33069339
Size of Group2: 49604580
Cut Size: 45023

------------------------------------
Iteration 11
Size of Group1: 33070005
Size of Group2: 49603914
Cut Size: 45012

------------------------------------
Iteration 12
Size of Group1: 33069960
Size of Group2: 49603959
Cut Size: 45011

------------------------------------
Iteration 13
Size of Group1: 33069960
Size of Group2: 49603959
Cut Size: 45011

Finish.

```
