#Create a component
open_component -reset component_interface_stream -flow_target vivado

add_files model.cpp

add_files -tb tb_model.cpp
add_files -tb test_data.dat
# add_files -tb sigmoid.dat
# add_files -tb tanh.dat

# Select the config (or use make SOLUTION=2).
set hls_config 1
if {[info exists ::env(HLS_SOLUTION)]} {
  set hls_config $::env(HLS_SOLUTION)
}

if {$hls_config == 1} {
  source configs/solution1.tcl
} elseif {$hls_config == 2} {
  source configs/solution2.tcl
} elseif {$hls_config == 3} {
  source configs/solution3.tcl
} else {
  error "SOLUTION must be 1, 2, or 3"
}

# Set variable to select which steps to execute.
# 1: synthesis, 2: synthesis + cosim, 3: full implementation, 4: csim only.
set hls_exec 3
if {[info exists ::env(HLS_EXEC)]} {
  set hls_exec $::env(HLS_EXEC)
}

# Override with make SAMPLES=5000 for the final accuracy benchmark.
set test_samples 50
if {[info exists ::env(HLS_SAMPLES)]} {
  set test_samples $::env(HLS_SAMPLES)
}
csim_design -argv "$test_samples"

if {$hls_exec == 1} {
  # Run Synthesis and Exit
  csynth_design

} elseif {$hls_exec == 2} {
  # Run Synthesis, RTL Simulation and Exit
  csynth_design

  cosim_design -argv "$test_samples"
} elseif {$hls_exec == 3} {
  # Run Synthesis, RTL Simulation, RTL implementation and Exit
  csynth_design

  cosim_design -argv "$test_samples"
  export_design -flow impl -rtl verilog -format ip_catalog
} else {
  # Exit after C simulation.
}

exit
