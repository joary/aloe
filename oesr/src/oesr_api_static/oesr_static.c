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

#include <stddef.h>
#include <stdlib.h>

#include "rtdal.h"
#include "defs.h"
#include "nod_waveform.h"
#include "oesr_static.h"
#include "oesr_context.h"

int _call_init(void *module);
int _call_stop(void *module);

int _run_cycle(void* context) {
	int i;
	oesr_context_t *ctx = (oesr_context_t*) context;
	nod_module_t *module = (nod_module_t*) ctx->module;
	nod_waveform_t *waveform = (nod_waveform_t*) module->parent.waveform;
	sdebug("context=0x%x, module_id=%d, changing_status=%d, cur_status=%d waveform_status=%d\n",context,
			module->parent.id, module->changing_status, module->parent.status, waveform->status.cur_status);

	if (waveform->status.cur_status == LOADED && module->parent.status == PARSED) {
		/* ack we have successfully been loaded */
		module->parent.status = LOADED;
		/* register init and stop functions */
		module->init = _call_init;
		module->stop = _call_stop;
	}

	/* Change only if finished previous status change */
	if (!module->changing_status && module->parent.status != waveform->status.cur_status) {
		sdebug("next_tslot=%d, cur_tslot=%d\n",waveform->status.next_timeslot, rtdal_time_slot());
		/* is it time to change? */
		if (rtdal_time_slot() >= waveform->status.next_timeslot) {
			switch(waveform->status.cur_status) {
			case INIT:
			case STOP:
			break;
			case STEP:
				if (module->parent.status == RUN) {
					module->parent.status = waveform->status.cur_status;
				} else {
					module->parent.status = RUN;
				}
				break;
			case PAUSE:
			case RUN:
				/* These status does not need confirmation */
				module->parent.status = waveform->status.cur_status;
			break;
			default:
				break;
			}
		}
	}

	if (!module->changing_status && module->parent.status == RUN) {

#ifdef OESR_API_GETTIME
		/* save start time */
		rtdal_time_get(&module->parent.execinfo.t_exec[1]);
#endif

		/* run aloe cycle */
		for (i=0;i<waveform->tslot_multiplicity;i++) {
			if (Run(context)) {
				sdebug("RUNERROR: module_id=%d\n",module->parent.id);

				/* set run-time error code */
				if (rtdal_process_seterror(module->process,RUNERROR)) {
					aerror("rtdal_process_seterror");
				}
			}
		}

		/* save end time */
#ifdef OESR_API_GETTIME
		rtdal_time_get(&module->parent.execinfo.t_exec[2]);
		rtdal_time_interval(module->parent.execinfo.t_exec);
		nod_module_execinfo_add_sample(&module->parent.execinfo,ctx->tstamp);
		tmdebug(module->time_log, &module->parent.execinfo.t_exec[0].tv_usec);
#endif
		ctx->tstamp++;

	} else {
		rtdal_process_stop(module->process);
	}

	return 0;
}


int _call_init(void *_module) {
	int n;
	nod_module_t *module = (nod_module_t*) _module;
	sdebug("module_id=%d, changing_status=%d, now_is=0\n",module->parent.id,
			module->changing_status);

	module->changing_status = 1;

	n = Init(module->context);
	if (n < 0) {
		return -1;
	} else if (n == 0) {
		return 0;
	}

	sdebug("module_id=%d finished init. Setting status to INIT\n",module->parent.id);
	if (n > 0) {
		module->changing_status = 0;
		module->parent.status = INIT;
	}

	return 1;
}

int _call_stop(void *_module) {
	nod_module_t *module = (nod_module_t*) _module;
	sdebug("module_id=%d, changing_status=%d\n",module->parent.id,
			module->changing_status);

	module->changing_status = 1;

	if (Stop(module->context)) {
		sdebug("module_id=%d failed stop\n",module->parent.id);
		return -1;
	}

	sdebug("module_id=%d finished stop. Setting status to STOP\n",module->parent.id);

	module->changing_status = 0;
	module->parent.status = STOP;

	return 0;
}

