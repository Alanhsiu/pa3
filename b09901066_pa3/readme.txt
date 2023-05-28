To compile the program, run the following command:
```make```

To run the program, run the following command like this:
```./bin/place -aux <aux file>```
for example:
```./bin/place -aux ./benchmark/ibm01/ibm01-cu85.aux```

To clean the program, run the following command:
```make clean```

To evaluate the program, run the following command like this:
```bash evaluator/evaluator.sh <aux file> <HPWL> <Time(s)>```
for example:
```bash evaluator/evaluator.sh benchmark/ibm01/ibm01-cu85.aux 255316134 6```
```bash evaluator/evaluator.sh benchmark/ibm05/ibm05.aux 24296540 18```