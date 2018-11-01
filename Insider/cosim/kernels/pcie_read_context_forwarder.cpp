#ifndef PCIE_READ_CONTEXT_FORWARDER_CPP_
#define PCIE_READ_CONTEXT_FORWARDER_CPP_

#include <insider_kernel.h>

void pcie_read_context_forwarder(
				 ST_Queue<unsigned char>& pcie_read_multiplexer_write_context,
				 ST_Queue<unsigned char>& pcie_read_multiplexer_read_context

				 ) {
  while (1) {
#pragma HLS pipeline
    unsigned char context;
    if (pcie_read_multiplexer_write_context.read_nb(context)) {
      pcie_read_multiplexer_read_context.write(context);
    }
  }
}

#endif
