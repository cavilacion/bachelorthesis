./fsa-insect < insect_grid.in > promela/abstraction-insect.pml
cat promela/abstraction-insect.pml > promela/model-insect.pml
echo "" >> promela/model-insect.pml
echo "" >> promela/model-insect.pml
cat promela/buchi-insect >> promela/model-insect.pml
spin -run promela/model-insect.pml
