# Solution2: Intermediate throughput; input-loop II=2.
set_top mlp_top
set_part {xc7z010clg400-1}
create_clock -period 10

set_directive_array_partition -type complete -dim 0 "Activation" coeff
# Pipeline inputs; process neurons in parallel.
set_directive_array_partition -type complete -dim 1 "Model" b1
set_directive_array_partition -type complete -dim 1 "Model" b2

set_directive_array_partition -type complete -dim 2 "Model" w1
set_directive_array_partition -type complete -dim 2 "Model" w2

set_directive_pipeline -II 2 "Model/LAYER1_LOOP"
set_directive_unroll "Model/NODES1"

set_directive_pipeline -II 2 "Model/LAYER2_LOOP"
set_directive_unroll "Model/NODES2"
