
# Spin Model Checking

## WARNING!!!
The software is extermeley unsafe to use, and memory errors are likely to happen! The provided inputs are likely safe to use, but precaution is adviced!

## Compiling
To compile the example:
```
$ make fsa-example
```
For the system that models the dynamics of the population of beetles:
```
$ make fsa-insect
```
The compiled programs take a gridded partition as input, and produce a finite state abstraction.

## Model checking
Executing the scripts 
```
./example.sh
```
or, for the insect model
```
./insect.sh
```
will generate the fsa, concatenate the buchi never-claim to the model, and start up the spin model checker.
The models can be found in promela/model-example.pml and promela/model-insect.pml.







