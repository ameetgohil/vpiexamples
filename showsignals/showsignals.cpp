#include <sv_vpi_user.h>
#include <iostream>
#include <cmath>

static int ShowSignals_compiletf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int arg_type;
    int err_flag = 0;
    return 0;
}

void PrintSignalValues(vpiHandle signal_iterator) {
    vpiHandle signal_handle;
    int signal_type;
    s_vpi_value current_value;
    while( (signal_handle = vpi_scan(signal_iterator)) != NULL) {
	signal_type = vpi_get(vpiType, signal_handle);
	switch(signal_type) {
	case vpiNet:
	    current_value.format = vpiBinStrVal;
	    vpi_get_value(signal_handle, &current_value);
	    std::cout << " Net " << vpi_get_str(vpiName, signal_handle) << " value is " << current_value.value.str << std::endl;
	    break;
	case vpiReg:
	    current_value.format = vpiBinStrVal;
	    vpi_get_value(signal_handle, &current_value);
	    std::cout << " Reg " << vpi_get_str(vpiName, signal_handle) << " value is " << current_value.value.str << std::endl;
	    break;
	case vpiIntegerVar:
	    current_value.format = vpiIntVal;
	    vpi_get_value(signal_handle, &current_value);
	    std::cout << " Integer " << vpi_get_str(vpiName, signal_handle) << " value is " << current_value.value.integer << std::endl;
	    break;
	case vpiRealVar:
	    current_value.format = vpiRealVal;
	    vpi_get_value(signal_handle, &current_value);
	    std::cout << " Real " << vpi_get_str(vpiName, signal_handle) << " value is " << current_value.value.real << std::endl;
	    break;
	case vpiTimeVar:
	    current_value.format = vpiTimeVal;
	    vpi_get_value(signal_handle, &current_value);
	    std::cout << " Real " << vpi_get_str(vpiName, signal_handle) << " value is " << current_value.value.time->high << current_value.value.time->low << std::endl;
	    break;
	}
    }
}

static int ShowSignals_calltf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, module_handle, signal_iterator;
    int format;
    s_vpi_time current_time;

    // obtain handle to the system task instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);

    // obtain handle to system task argument
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    module_handle   = vpi_scan(arg_iterator);
    vpi_free_object(arg_iterator);

    // read current simulation time
    current_time.type = vpiScaledRealTime;
    vpi_get_time(systf_handle, &current_time);

    std::cout << "At time: " << current_time.real << " nets in module " << vpi_get_str(vpiFullName, module_handle) << std::endl;

    //obtain handles to nets in module and read current vlue
    signal_iterator = vpi_iterate(vpiNet, module_handle); //get wires
    if(signal_iterator != NULL)
	PrintSignalValues(signal_iterator);

    signal_iterator = vpi_iterate(vpiReg, module_handle); //gets reg
    if(signal_iterator != NULL)
	PrintSignalValues(signal_iterator);

    signal_iterator = vpi_iterate(vpiVariables, module_handle); //gets variables
    if(signal_iterator != NULL)
	PrintSignalValues(signal_iterator);
    std::cout << std::endl;
    return 0;
}

void ShowSignals_register() {
    s_vpi_systf_data tf_data;
    s_cb_data cb_data_s;
    vpiHandle callback_handle;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$show_all_signals";
    tf_data.calltf      = ShowSignals_calltf;
    tf_data.compiletf   = ShowSignals_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);

}

void (*vlog_startup_routines[])() = {
    ShowSignals_register,
    0
};
