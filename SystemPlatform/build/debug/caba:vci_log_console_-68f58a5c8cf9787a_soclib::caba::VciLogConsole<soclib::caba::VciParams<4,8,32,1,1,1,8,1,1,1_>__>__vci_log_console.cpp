#include "vci_log_console.h"
#include "vci_buffers.h"
#include "mapping_table.h"
#include "dpp_placeholder.h"
#include "vci_target.h"
#include "int_tab.h"
#include "address_decoding_table.h"
#include "exception.h"
#include "vci_target_fsm.h"
#include "address_masking_table.h"
#include "caba_base_module.h"
#include "vci_param.h"
#include "base_module.h"
#include "segment.h"
#include "linked_access_buffer.h"
#include "/softl1/TPSoC3A/Simulation/SoCLib/soclib/soclib/module/connectivity_component/vci_log_console/caba/source/src/vci_log_console.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::VciLogConsole<soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1 >  > ;
#endif
