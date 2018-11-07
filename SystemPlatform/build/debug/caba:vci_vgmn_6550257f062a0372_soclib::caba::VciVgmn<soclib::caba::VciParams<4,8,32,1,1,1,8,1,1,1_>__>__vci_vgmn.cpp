#include "vci_vgmn.h"
#include "mapping_table.h"
#include "dpp_placeholder.h"
#include "vci_target.h"
#include "int_tab.h"
#include "address_decoding_table.h"
#include "vci_initiator.h"
#include "exception.h"
#include "address_masking_table.h"
#include "caba_base_module.h"
#include "vci_param.h"
#include "base_module.h"
#include "segment.h"
#include "/softl1/TPSoC3A/Simulation/SoCLib/soclib/soclib/module/network_component/vci_vgmn/caba/source/src/vci_vgmn.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::VciVgmn<soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1 >  > ;
#endif
