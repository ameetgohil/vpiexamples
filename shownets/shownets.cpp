#include <sv_vpi_user.h>
#include <iostream>
#include <cmath>

static int ShowNets_compiletf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int arg_type;
    int err_flag = 0;
    return 0;
}

static int ShowNets_calltf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, module_handle, net_iterator, net_handle;
    s_vpi_time current_time;
    s_vpi_value current_value;

    // obtain handle to the system task instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);

    // obtain handle to system task argument
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    module_handle   = vpi_scan(arg_iterator);
    vpi_free_object(arg_iterator);

    // read current simulation time
    current_time.type = vpiScaledRealTime;
    vpi_get_time(systf_handle, &current_time);

    std::cout << "time: " << current_time.real << " nets in module " << vpi_get_str(vpiFullName, module_handle) << std::endl;

    //obtain handles to nets in module and read current vlue
    net_iterator = vpi_iterate(vpiNet, module_handle);
    if(net_iterator == NULL)
	std::cout << "no nets found in this module" << std::endl;
    else {
	current_value.format = vpiBinStrVal; // reads values as a string
	while( (net_handle = vpi_scan(net_iterator)) != NULL) {
	    vpi_get_value(net_handle, &current_value);
	    std::cout << "net " << vpi_get_str(vpiName, net_handle) << " value is " << current_value.value.str << std::endl;
	}
    }
    return 0;
}

void ShowNets_register() {
    s_vpi_systf_data tf_data;
    s_cb_data cb_data_s;
    vpiHandle callback_handle;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$show_all_nets";
    tf_data.calltf      = ShowNets_calltf;
    tf_data.compiletf   = ShowNets_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);

}

void (*vlog_startup_routines[])() = {
    ShowNets_register,
    0
};
