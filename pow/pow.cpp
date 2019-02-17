#include <sv_vpi_user.h>
#include <iostream>


static int ShowVal_compiletf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int arg_type;
    // obtain handle to the system instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    if(systf_handle == NULL) {
	vpi_printf("ERROR: $show_value failed to obtain systf handle\n");
	vpi_control(vpiFinish, 0); // abort simulation
	return 0;
    }

    // obtain handles to system task arguments
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    if (arg_iterator == NULL) {
	vpi_printf("ERROR: $show_value requires 1 argument\n");
	vpi_control(vpiFinish, 0); // abort simulation;
	return 0;
    }

    // check the type of object in system task arguments
    arg_handle = vpi_scan(arg_iterator);
    arg_type = vpi_get(vpiType, arg_handle);
    if (arg_type != vpiNet && arg_type != vpiReg) {
	vpi_printf("ERROR: $show_value arg must be a net or reg\n");
	vpi_free_object(arg_iterator); // free iterator memory
	vpi_control(vpiFinish, 0); // abort simulation
	return 0;
    }

    // check that there are no more system task arguments
    arg_handle = vpi_scan(arg_iterator);
    if(arg_handle != NULL) {
	vpi_printf("ERROR: $show_value can only have 1 argument\n");
	vpi_free_object(arg_iterator); // free iterator memory
	vpi_control(vpiFinish, 0); // abort simulation
	return 0;
    }
    return 0;
}

static int ShowVal_calltf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle, net_handle;
    s_vpi_value current_value;

    // obtain handle to the system task instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);

    // obtain handle to system task argument
    // compiletf has already verified only 1 arg with correct type
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    net_handle   = vpi_scan(arg_iterator);
    vpi_free_object(arg_iterator); // free iterator memory

    //read current value
    current_value.format = vpiBinStrVal; // read value as a string
    vpi_get_value(net_handle, &current_value);
    vpi_printf("Signal %s ", vpi_get_str(vpiFullName, net_handle));
    vpi_printf("has the value %s\n", current_value.value.str);
    return 0;
}

void pow_register() {
    s_vpi_systf_data tf_data;
    s_cb_data cb_data_s;
    vpiHandle callback_handle;

    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSysFuncSized;
    tf_data.tfname      = "$show_value";
    tf_data.calltf      = ShowVal_calltf;
    tf_data.compiletf   = ShowVal_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

void (*vlog_startup_routines[])() = {
    pow_register,
    0
};
