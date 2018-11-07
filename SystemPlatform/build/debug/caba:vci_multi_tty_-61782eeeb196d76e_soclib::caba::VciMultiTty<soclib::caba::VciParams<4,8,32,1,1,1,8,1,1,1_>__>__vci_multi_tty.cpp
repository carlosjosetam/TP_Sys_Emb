#include "process_wrapper.h"
#include "vci_buffers.h"
#include "mapping_table.h"
#include "dpp_placeholder.h"
#include "pts_wrapper.h"
#include "vci_target.h"
#include "tty.h"
#include "address_decoding_table.h"
#include "int_tab.h"
#include "exception.h"
#include "xterm_wrapper.h"
#include "address_masking_table.h"
#include "caba_base_module.h"
#include "vci_multi_tty.h"
#include "fd_poller.h"
#include "vci_param.h"
#include "base_module.h"
#include "vci_target_fsm.h"
#include "segment.h"
#include "linked_access_buffer.h"
#include "tty_wrapper.h"
#include "/softl1/TPSoC3A/Simulation/SoCLib/soclib/soclib/module/connectivity_component/vci_multi_tty/caba/source/src/vci_multi_tty.cpp"
#ifndef ENABLE_SCPARSE
template class soclib::caba::VciMultiTty<soclib::caba::VciParams<4,8,32,1,1,1,8,1,1,1 >  > ;
#endif
