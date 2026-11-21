# Solution1: Small-footprint design.
set_top mlp_top
set_part {xc7z010clg400-1}
create_clock -period 10

set_directive_array_partition -type complete -dim 0 "Activation" coeff
set_directive_pipeline -off "Model/LAYER2_LOOP"
set_directive_unroll -factor 1 "Model/NODES2"
# Activation and NODES2 pipeline directives were not explicitly enabled.

# Keep both MAC loops sequential across neurons.
set_directive_pipeline -off "Model/LAYER1_LOOP"
set_directive_unroll -factor 1 "Model/NODES1"
