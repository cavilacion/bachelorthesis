./fsa-example < example_grid.in > promela/abstraction-example.pml
cat promela/abstraction-example.pml > promela/model-example.pml
echo "" >> promela/model-example.pml
echo "" >> promela/model-example.pml
cat promela/buchi-example >> promela/model-example.pml
spin -run promela/model-example.pml
