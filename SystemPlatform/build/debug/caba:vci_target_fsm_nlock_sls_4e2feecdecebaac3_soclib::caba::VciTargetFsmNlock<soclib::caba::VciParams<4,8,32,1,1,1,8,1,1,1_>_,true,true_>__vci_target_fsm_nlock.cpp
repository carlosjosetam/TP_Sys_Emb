#include "linked_access_buffer_mcc.h"
#include "vci_buffers.h"
#include "mapping_table.h"
#include "dpp_placeholder.h"
#include "vci_param.h"
#include "address_decoding_table.h"
#include "exception.h"
#include "address_masking_table.h"
#include "caba_base_module.h"
#include "base_module.h"
#include "int_tab.h"
#include "vci_target_fsm_nlock.h"
#include "segment.h"
#include "/softslin/TPSoC3A/Simulation/SoCLib/contrib/dmoc_component/vci_target_fsm_nlock/src/caba/vci/vci_target_fsm_nlock.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::VciTargetFsmNlock<soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1 > ,true,true > ;
#endif
