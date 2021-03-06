/* 
 * Copyright (c) 2012, Ismael Gomez-Miguelez <ismael.gomez@tsc.upc.edu>.
 * This file is part of ALOE++ (http://flexnets.upc.edu/)
 * 
 * ALOE++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * ALOE++ is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with ALOE++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "packet.h"
#include "defs.h"
#include "objects_max.h"
#include "rtdal.h"
#include "oesr.h"
#include "nod_anode.h"
#include "nod_dispatcher.h"
#include "mempool.h"

nod_anode_t anode;

packet_t *node_packet;

void nod_anode_initialize_waveforms();

/**
 * 1) Pre-allocates max_waveforms objects of type Waveform
 * (with max_modules_x_waveform and max_variables_x_module modules and variables object instances)
 * in the +loaded_waveforms array and initialize synchronization master/slave
 * 2) Find physical interface with name "ctrl" and save to ctrlItf
 * 3) Find physical interface with name "probe" and save to probeItf
 * 4) If !rtdal.machine.syncMaster,
 *    4.1) find physical interface with name "sync" and save pointer to syncItf                              
 *    4.2) syncItf.setCallback(SyncSlave,kernelPrio)
 *    4.3) if !rtdal.machine.syncContinuous add periodic function SyncRequest() to the kernel
 * with period "period"
 * 5) If rtdal.machine.syncMaster,
 *    5.1) find all physical interface with name starting with "slavesync*" and save their object
 *  address to slaveItf array
 *    5.2) if rtdal.machine.isContinuous add SyncMaster() as a periodic function to the kernel with
 * period 1
 *    5.3) if !rtdal.machine.isContinuous slaveItf[i].setCallback(SyncMaster,kernelPrio) for each
 * slaveItf
 */
int nod_anode_initialize(int max_waveforms) {
	ndebug("max_waveforms=%d\n",max_waveforms);
	node_packet = &anode.packet;
	if (packet_init(&anode.packet, 64*1024)) {
		aerror("initializing packet\n");
		return -1;
	}
	nod_anode_initialize_waveforms(max_waveforms);
	return 0;
}

void nod_anode_initialize_waveforms(int max_waveforms) {
	ndebug("max_waveforms=%d\n",max_waveforms);

	int i;

	anode.loaded_waveforms = (nod_waveform_t*) pool_alloc(max_waveforms,sizeof(waveform_t));
	assert(anode.loaded_waveforms);
	anode.max_waveforms = max_waveforms;

	for (i=0;i<max_waveforms;i++) {
		anode.loaded_waveforms[i].status.cur_status = STOP;
	}
}

/**
 * This is a thread created by the rtdal kernel thread with normal priority. Reads commands from
 * ctrl interface and processes them.
 */
int nod_anode_cmd_recv() {
	/**@TODO: This should be a thread. Send nod_anode_dispatch() return value using ACK */
	return nod_anode_dispatch(&anode.packet);
}

