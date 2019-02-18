#include <sv_vpi_user.h>
#include <iostream>
#include <cmath>

static int EndOfTime_compiletf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int arg_type;
    int err_flag = 0;
    std::cout << "compile" << std::endl;
    return 0;
}

int EndOfTime_callback(p_cb_data cb_data_p) {
    s_vpi_time time_s;
    s_vpi_value value_s;
    vpiHandle arg_h;

    arg_h = (vpiHandle) cb_data_p->user_data;
    value_s.format = vpiBinStrVal;
    vpi_get_value(arg_h, &value_s);
    std::cout << "At " << cb_data_p->time->low << ": " << vpi_get_str(vpiFullName, arg_h) << " = " << value_s.value.str << std::endl;
}

static int EndOfTime_calltf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle, cb_handle;
    s_vpi_time time_s;
    s_cb_data cb_data_s;

    // obtain handle to the system task instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);
    // obtain handle to system task argument
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    arg_handle   = vpi_scan(arg_iterator);
    if(arg_handle == NULL)
	std::cout << "arg_handle null" << std::endl;
    vpi_free_object(arg_iterator);

    // setup callback
    time_s.low = 0;
    time_s.high = 0;
    time_s.type = vpiSimTime;
    cb_data_s.reason = cbReadOnlySynch;
    cb_data_s.cb_rtn = EndOfTime_callback;
    cb_data_s.obj    = arg_handle;
    cb_data_s.time   = &time_s;
    cb_data_s.value  = NULL;
    cb_data_s.index  = 0;
    cb_data_s.user_data = (char *) arg_handle;
    cb_handle = vpi_register_cb(&cb_data_s);
    std::cout << "::::call::::" << std::endl;
    vpi_free_object(cb_handle);

    return 0;
}

void EndOfTime_register() {
    s_vpi_systf_data tf_data;
    s_cb_data cb_data_s;
    vpiHandle callback_handle;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$end_of_time";
    tf_data.calltf      = EndOfTime_calltf;
    tf_data.compiletf   = EndOfTime_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);

}

void (*vlog_startup_routines[])() = {
    EndOfTime_register,
    0
};
