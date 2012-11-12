#include "man_platform.h"
#include <stddef.h>
#include "defs.h"
#include "str.h"

static man_platform_t *platform;

void man_platform_set_context(man_platform_t *context) {
	platform = context;
}

man_platform_t *man_platform_get_context() {
	return platform;
}

/**
 * For each node, send a HWINFO command. Save the contents of the received packet into node.hwinfo structure
 */
int man_platform_update() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) For each node, call node[i].ctrlItf.connect() and node[i].probeItf.connect()
 * 3) If syncLocal==false, syncItf.connect()
 * 4) For each node, send Cmd.CONNECT command
 */
int man_platform_connect() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * 1) For each node, send Cmd.DISCONNECT command
 * 2) For each node, call node.ctrlItf.close(), node.probeItf.close()
 * 3) call syncItf.close()
 */
int man_platform_disconnect() {
	aerror("Not yet implemented");
	return -1;
}

int man_platform_tslot_length() {
	aerror("Not yet implemented");
	return -1;
}

man_platform_model_t* man_platform_get_model() {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Initializes the HWAPI and reads and parses the platform configuration file with path "configFile". It must be called before calling any other function.
 *
 * 1) Parse "configFile"
 * 2) Call hwapi_create_manager_itfs(configFile)
 * 3) Configure clock:
 *   3.1) If clockLocal==TRUE, call hwapi_attach_local_time() and set sync_local=TRUE
 *   3.2) Otherwise, set sync_itf=hwapi_physicitf_get("sync"); and set sync_local=FALSE
 * 4) For each node
 *   4.1) Configure node, processor and interfaces instances
 *   4.2) set node[i].ctrl_itf=hwapi_itfphysic_get("ctrl_nodename") where nodename is the node name
 *   4.3) set node[i].probe_itf=hwapi_itfphysic_get("probe_nodename") where nodename is the node name 
 *   4.4) Add asyncError to all probe interfaces listeners. Node.probeListener.addCallback(process_probe_pkt)
 *
 * 5) Create C, B, I platform model from the nodes and data interfaces specified in the configuration file
 *
 * The function must check that the configuration file parameters are correct, issuing warnings when necessary
 */

int man_platform_config(string config_file) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Synchronizes with the master clock reference (using shared memory if master is in the same computer
 * or sync interfaces otherwise.
 * @return
 */
int man_platform_sync() {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Sends a command to a destination. Packet must be filled before.
 */
int man_platform_send_cmd(packet_dest_t dest, packet_command_t cmd) {
	aerror("Not yet implemented");
	return -1;
}

/**
 * Searches node_id in the current platform
 * @param node_id node id to look for
 * @return
 */
man_node_t* man_platform_node_id(int node_id) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Searches processor index in the current platform.
 * @param processor_idx
 * @return
 */
man_node_t* man_platform_node_pidx(int processor_idx) {
	aerror("Not yet implemented");
	return NULL;
}

/**
 * Processes asynchronous execution error messages from nodes. The format of the asyncError packet is as follows:
 *
 * 1st 32-bit word: Waveform id
 * 2st 32-bit word: Module id
 * 3st 32-bit word: Error code
 * Next: aerrorMsg string
 *
 * For any error message!=OK, set STOP status to the waveform that caused it and print message to SDTDOUT.
 */
int man_platform_process_probe_pkt(void* data, int size) {
	aerror("Not yet implemented");
	return -1;
}