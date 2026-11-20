SOLUTION ?= 1
STAGE ?= impl
SAMPLES ?= 50
export HLS_SAMPLES = $(SAMPLES)
export HLS_SOLUTION = $(SOLUTION)
ifeq ($(STAGE),synth)
export HLS_EXEC = 1
else ifeq ($(STAGE),cosim)
export HLS_EXEC = 2
else ifeq ($(STAGE),impl)
export HLS_EXEC = 3
else ifeq ($(STAGE),csim)
export HLS_EXEC = 4
else
$(error STAGE must be csim, synth, cosim, or impl)
endif

.PHONY: all
all:
	vitis-run --mode hls --tcl .\run_hls.tcl
