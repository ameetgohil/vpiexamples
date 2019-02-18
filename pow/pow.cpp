#include <sv_vpi_user.h>
#include <iostream>
#include <cmath>

static int PowSizetf(char* user_data) {
    return 32;
}

static int Pow_compiletf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int arg_type;
    int err_flag = 0;
    do {
	// obtain handle to the system instance
	systf_handle = vpi_handle(vpiSysTfCall, NULL);
	// obtain handles to system task arguments
	arg_iterator = vpi_iterate(vpiArgument, systf_handle);
	if(arg_iterator == NULL) {
	    vpi_printf("ERROR: $pow requires 2 arguments; has none\n");
	    err_flag =1;
	    break;
	}
	arg_handle = vpi_scan(arg_iterator);
	arg_type = vpi_get(vpiType, arg_handle);
	if((arg_type != vpiReg) &&
	   (arg_type != vpiIntegerVar) &&
	   (arg_type != vpiConstant)) {
	    vpi_printf("ERROR: $pow arg1 must be a number, variable or net\n");
	    err_flag = 1;
	    break;
	}

	arg_handle = vpi_scan(arg_iterator);
	if (arg_iterator == NULL) {
	    vpi_printf("ERROR: $pow requires 2nd argument\n");
	    err_flag = 1;
	    break;
	}

	arg_type = vpi_get(vpiType, arg_handle);
	if((arg_type != vpiReg) &&
	   (arg_type != vpiIntegerVar) &&
	   (arg_type != vpiConstant)) {
	    	    vpi_printf("ERROR: $pow arg2 must be a number, variable or net\n");
	    err_flag = 1;
	    break;
	}
	
	if(vpi_scan(arg_iterator) != NULL) {
	    vpi_printf("ERROR: too many arguments\n");
	    err_flag = 1;
	    break;
	}
    } while (false);

    if(err_flag)
	vpi_control(vpiFinish,1); // abort simulation

    return 0;
}

static int Pow_calltf(char* user_data) {
    vpiHandle systf_handle, arg_iterator, arg_handle;
    int base, exp;
    double result;
    s_vpi_value value_s;

    // obtain handle to the system task instance
    systf_handle = vpi_handle(vpiSysTfCall, NULL);

    // obtain handle to system task argument
    arg_iterator = vpi_iterate(vpiArgument, systf_handle);
    arg_handle   = vpi_scan(arg_iterator);
    value_s.format = vpiIntVal; // define format
    vpi_get_value(arg_handle, &value_s);
    base = value_s.value.integer;

    arg_handle   = vpi_scan(arg_iterator);
    vpi_get_value(arg_handle, &value_s);
    exp = value_s.value.integer;
    //vpi_free_object(arg_iterator);

    result = pow((double) base, (double) exp);
    // write result to simulation as return value $pow
    value_s.value.integer = (PLI_INT32) result;
    vpi_printf("result: %d\n", (int) result);
    vpi_put_value(systf_handle, &value_s, NULL, vpiNoDelay);
    return 0;
}

static int PowStartOfSim(s_cb_data *callback_data) {
    vpi_printf("\n$pow_test app\n");
    return 0;
}

void Pow_register() {
    s_vpi_systf_data tf_data;
    s_cb_data cb_data_s;
    vpiHandle callback_handle;

    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSysFuncSized;
    tf_data.tfname      = "$pow_test";
    tf_data.calltf      = Pow_calltf;
    tf_data.compiletf   = Pow_compiletf;
    tf_data.sizetf      = PowSizetf;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);

    cb_data_s.reason    = cbStartOfSimulation;
    cb_data_s.cb_rtn    = PowStartOfSim;
    cb_data_s.obj       = NULL;
    cb_data_s.time      = NULL;
    cb_data_s.value     = NULL;
    cb_data_s.user_data = NULL;
    callback_handle = vpi_register_cb(&cb_data_s);
    vpi_free_object(callback_handle); // don't need callback handle
    
}

void (*vlog_startup_routines[])() = {
    Pow_register,
    0
};
