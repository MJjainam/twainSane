#include "myscanLib.h"

static void auth_callback(SANE_String_Const resource,
                          SANE_Char *username, SANE_Char *password)
{

}

int main()
{
    printf("Print ni myscanLib.c");
    return 0;
}

SANE_Status ud_sane_init(SANE_Int *version_code)//, SANE_Auth_Callback authorize)
{
    return sane_init(version_code, auth_callback);
}

SANE_Status ud_sane_get_devices(const SANE_Device ***device_list, SANE_Bool local_only)
{
    return sane_get_devices(device_list, local_only);
}

SANE_Status ud_sane_open(SANE_String_Const name, SANE_Handle *h)
{
    return sane_open(name, h);
}

const SANE_Option_Descriptor *ud_sane_get_option_descriptor(SANE_Handle h, SANE_Int n)
{
    return sane_get_option_descriptor(h, n);
}

SANE_Status ud_sane_control_option(SANE_Handle h, SANE_Int n, SANE_Action a, void *v, SANE_Int *i)
{
    return sane_control_option(h, n, a, v, i);
}

void ud_sane_exit()
{
    sane_exit();
}
