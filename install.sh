#!/bin/sh

export SOC_HOME=/softslin/TPSoC3A/Simulation
export APES_ROOT=$SOC_HOME/Apes
source $APES_ROOT/install.sh

export PATH=$SOC_HOME/SoCLib/soclib/utils/bin:$PATH
export LD_LIBRARY_PATH=$SOC_HOME/SystemCASS/lib-linux:$LD_LIBRARY_PATH

export SOCLIB_CC_MODE=debug

