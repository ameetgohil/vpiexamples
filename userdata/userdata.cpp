#include <sv_vpi_user.h>
#include <iostream>
#include <cmath>

int GetVectorCalltf(char *user_data) {
    std::cout << "user data: " << (int)*user_data << std::endl;
    if(*user_data == 1)
	std::cout << "get_vector_bin called" << std::endl;
    else if(*user_data == 2)
	std::cout << "get_vector_hex called" << std::endl;
    return 0;
}

void test_user_data_register() {
    s_vpi_systf_data tf_data;
    char *id1 = (char *)malloc(sizeof(int)); //allocate storage
    char *id2 = (char *)malloc(sizeof(int)); //allocate storage
    *id1 = 1;
    *id2 = 2;

    tf_data.type        = vpiSysTask;
    //    tf_data.sysfunctype = vpiSysTaskSized;
    tf_data.tfname      = "$get_vector_bin";
    tf_data.calltf      = GetVectorCalltf;
    tf_data.compiletf   = NULL; // don't really need compiletf if you don't want check the inputs
    tf_data.sizetf      = NULL;
    tf_data.user_data   = (PLI_BYTE8 *) id1;
    vpi_register_systf(&tf_data);

    tf_data.type        = vpiSysTask;
    //    tf_data.sysfunctype = vpiSysTaskSized;
    tf_data.tfname      = "$get_vector_hex";
    tf_data.calltf      = GetVectorCalltf;
    tf_data.compiletf   = NULL;
    tf_data.sizetf      = NULL;
    tf_data.user_data   = (PLI_BYTE8 *) id2;
    vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
    test_user_data_register,
    0
};
