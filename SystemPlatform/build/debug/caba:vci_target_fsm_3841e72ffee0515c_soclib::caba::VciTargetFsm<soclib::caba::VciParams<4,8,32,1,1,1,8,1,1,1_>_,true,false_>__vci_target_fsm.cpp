#include "vci_buffers.h"
#include "mapping_table.h"
#include "dpp_placeholder.h"
#include "vci_param.h"
#include "int_tab.h"
#include "address_decoding_table.h"
#include "exception.h"
#include "vci_target_fsm.h"
#include "address_masking_table.h"
#include "caba_base_module.h"
#include "base_module.h"
#include "segment.h"
#include "linked_access_buffer.h"
#include "/softl1/TPSoC3A/Simulation/SoCLib/soclib/soclib/lib/vci_target_fsm/src/caba/vci/vci_target_fsm.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::VciTargetFsm<soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1 > ,true,false > ;
#endif
