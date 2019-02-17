#include <sv_vpi_user.h>
#include <iostream>

static int hello_compiletf(char* user_data) {
    return 0;
}

static int hello_calltf(char* user_data) {
    vpi_printf("Hello, World!\n");
    std::cout << "meow" << std::endl;
    return 0;
}

void hello_register() {
    s_vpi_systf_data tf_data;

    tf_data.type = vpiSysTask;
    tf_data.tfname = "$hello";
    tf_data.calltf = hello_calltf;
    tf_data.compiletf = hello_compiletf;
    tf_data.sizetf = 0;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

static int task_compiletf(char *user_data) {
    return 0;
}

static int task_calltf(char *user_data) {
    vpiHandle vhandle, iterator, inhandle;
    s_vpi_value inval;
    vpiHandle outhandle;
    vpiHandle outhandle2;
    vpiHandle outhandle3;
    s_vpi_value outval;
    s_vpi_value outval2;
    s_vpi_value outval3;

    int din, dout, dout2, dout3;

    vhandle = vpi_handle(vpiSysTfCall, NULL);
    iterator = vpi_iterate(vpiArgument, vhandle);
    inhandle = vpi_scan(iterator);

    inval.format = vpiIntVal;
    vpi_get_value(inhandle, &inval);
    din = inval.value.integer;

    vpi_printf("din=%d\n", din);

    outhandle = vpi_scan(iterator);
    outhandle2 = vpi_scan(iterator);
    outhandle3 = vpi_scan(iterator);

    dout = din;
    outval.format = vpiIntVal;
    outval.value.integer = dout;
    vpi_put_value(outhandle, &outval, NULL, vpiNoDelay);

    dout2 = din*2;
    outval2.format = vpiIntVal;
    outval2.value.integer = dout2;
    vpi_put_value(outhandle2, &outval2, NULL, vpiNoDelay);

    dout3 = din*3;
    outval3.format = vpiIntVal;
    outval3.value.integer = dout3;
    vpi_put_value(outhandle3, &outval3, NULL, vpiNoDelay);

    vpi_printf("dout: %d, dout2: %d, dout3: %d\n", outval.value.integer, outval2.value.integer, outval3.value.integer);
    return 0;
}

void task_register() {
    s_vpi_systf_data tf_data;
    
    tf_data.type = vpiSysTask;
    tf_data.tfname = "$task_test";
    tf_data.calltf = task_calltf;
    tf_data.compiletf = task_compiletf;
    tf_data.sizetf = 0;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

static int32_t EndOfCompCB(p_cb_data cb_data) {
    std::cout << "End of compile cb" << std::endl;
}

static int32_t StartOfSimulationCB(p_cb_data cb_data) {
    std::cout << "Start of Simulation cb" << std::endl;
}


static int delay_compiletf(char* user_data) {
    return 0;
}


static int32_t clkChangeCB(p_cb_data cb_data) {
    std::cout << "clk change" << std::endl;
}

static int delay_calltf(char* user_data) {
    s_cb_data cb_data_s;
    vpiHandle vhandle, iterator, clkHandle;
    vhandle = vpi_handle(vpiSysTfCall, NULL);
    iterator = vpi_iterate(vpiArgument, vhandle);
    clkHandle = vpi_scan(iterator);
    
    cb_data_s.reason    = cbValueChange;
    cb_data_s.cb_rtn    = clkChangeCB;
    cb_data_s.obj       = clkHandle;
    cb_data_s.user_data = user_data;
    std::cout << iterator << std::endl;
    //(void) vpi_register_cb(&cb_data_s);
    vpi_printf("Delay!\n");
    //    std::cout << "meow" << std::endl;
    return 0;
}

void delay_register() {
    s_cb_data cb_data_s;
    s_vpi_time vpi_time_s;
    s_vpi_systf_data tf_data;
    //p_vpi_cb_user_data user_data;

    vpi_time_s.type = vpiSimTime;
    vpi_time_s.high = (uint32_t)(10);
    vpi_time_s.low  = (uint32_t)(0);

    tf_data.type = vpiSysTask;
    tf_data.tfname = "$delay";
    tf_data.calltf = delay_calltf;
    tf_data.compiletf = delay_compiletf;
    tf_data.sizetf = 0;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
    
    cb_data_s.reason    = cbAfterDelay;
    cb_data_s.reason    = cbEndOfSimulation;
    cb_data_s.cb_rtn    = EndOfCompCB;
    cb_data_s.obj       = NULL;
    cb_data_s.time      = &vpi_time_s;
    //cb_data_s.value     = NULL;
    cb_data_s.user_data = 0;//(char *)user_data;
    (void) vpi_register_cb(&cb_data_s);

    cb_data_s.reason    = cbStartOfSimulation;
    cb_data_s.cb_rtn    = StartOfSimulationCB;
    cb_data_s.obj       = NULL;
    cb_data_s.user_data = 0;//(char *)user_data;
    (void) vpi_register_cb(&cb_data_s);

    

}

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

void ShowVal_register() {
    s_vpi_systf_data tf_data;

    tf_data.type        = vpiSysTask;
    tf_data.sysfunctype = 0;
    tf_data.tfname      = "$show_value";
    tf_data.calltf      = ShowVal_calltf;
    tf_data.compiletf   = ShowVal_compiletf;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = NULL;
    vpi_register_systf(&tf_data);
    return;
}

void (*vlog_startup_routines[])() = {
    hello_register,
    task_register,
    delay_register,
    ShowVal_register,
    0
};
