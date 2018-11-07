#include "binary_file_symbol.h"
#include "binary_file_section.h"
#include "exception.h"
#include "sram_param.h"
#include "caba_base_module.h"
#include "sram.h"
#include "base_module.h"
#include "int_tab.h"
#include "segment.h"
#include "loader.h"
#include "/softslin/TPSoC3A/Simulation/SoCLib/contrib/dmoc_component/generic_sram/source/src/sram.cpp"
#include "/softslin/TPSoC3A/Simulation/SoCLib/contrib/dmoc_component/generic_sram/source/src/sram_utils.cpp"
#include "/softslin/TPSoC3A/Simulation/SoCLib/contrib/dmoc_component/generic_sram/source/src/sram_mealy.cpp"
#include "/softslin/TPSoC3A/Simulation/SoCLib/contrib/dmoc_component/generic_sram/source/src/sram_cdb.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::SRam<soclib::caba::SramParam<4,32 >  > ;
#endif
