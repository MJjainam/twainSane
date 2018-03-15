#include <sane/sane.h>
#include <sane/saneopts.h>

#include <alloca.h>
#include <stdio.h>

static void auth_callback(SANE_String_Const resource,
    SANE_Char *username, SANE_Char *password)
{
}

int main(){
    int version_code;
    unsigned char buf8[2 * 32768];

    // SANE_Byte *image;
    SANE_Word total_bytes; //stores total image length
    // unsigned char *buf8ptr;
    SANE_Int len;
    SANE_Handle handle;
    // bunch.dummy_filename = "dummy.in";

    SANE_Status status = sane_init(&version_code, (void *)auth_callback);
    sane_exit();
    return 0;
}