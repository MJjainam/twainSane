#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
// #include <twain.h>
#include <sane/sane.h>
#include <sane/saneopts.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

// #include <sane/sanei_auth.h>
#define BUFSIZE 512

//-------------------------------------------------------------------

#include <alloca.h>
// #include <sys/types.h>
#include <sys/stat.h>
#include <locale.h>

#define INTSIZE sizeof(int)
// #include <sane/.so>
//SANE_Status sanestatus;
// SANE_Status sane_init(SANE_Int *version_code,
//                       SANE_Auth_Callback authorize)
// {

//     typedef SANE_Status (*sane_init)(SANE_Int * version_code,
//                                      SANE_Auth_Callback authorize);

//     sane_init x = (sane_init)dlsym(RTLD_NEXT, "sane_init");

//     FILE *log = fopen("/home/radix/Desktop/Git/saneHook/logfile", "ab+");
//     if(log==NULL){
//         return -1;
//     }
//     fprintf(log, "sane_init is called");
//     fprintf(stdout, "sane_init is called");
//     fclose(log);

//         // return sanestatus;
//     return (*x)(version_code, authorize);
// }

int writeToLog(char *buf)
{
    FILE *log = fopen("/home/radix/Desktop/Git/saneHook/logfile", "ab+");
    if (log == NULL)
    {
        fprintf(stdout, "Could not open log\n");
    }
    else
    {
        fprintf(log, buf);
    }

    fclose(log);
}

SANE_Status sane_open (SANE_String_Const name, SANE_Handle * h){
    writeToLog("Sane open is called\n");

    SANE_Status (*old_sane_open)(SANE_String_Const name, SANE_Handle * h);
    old_sane_open = dlsym(RTLD_NEXT, "sane_open");
    writeToLog("This is after getting the old_sane_open\n");
    return old_sane_open(name, h);
}


SANE_Status sane_read(SANE_Handle h, SANE_Byte *buf, SANE_Int maxlen, SANE_Int *len)
{

    int errStatus;

    //typedef SANE_Status (*sane_read)(SANE_Handle h, SANE_Byte *buf, SANE_Int maxlen, SANE_Int *len);
    writeToLog("sane_read is called\n");
    SANE_Status status;
    SANE_Status (*old_sane_read)(SANE_Handle h, SANE_Byte * buf, SANE_Int maxlen, SANE_Int * len);

    old_sane_read = dlsym(RTLD_NEXT, "sane_read");

    status = old_sane_read(h, buf, maxlen, len);

    //Send the read bytes here
    //Create IPC to twainServer
    char *imageDataFifo = "/tmp/imageDataFifo";
    // char buffer[BUFSIZE];
    mkfifo(imageDataFifo, 0666);
    int writeFd = open(imageDataFifo, O_WRONLY);

    //1. write the SANE_STATUS
    //writeToLog("Status value is " +  itoa(status));
    if (write(writeFd, &status, INTSIZE) != INTSIZE)
    {
        writeToLog("Error in writing SANE_Status to writeFd in saneHook.c");
        return SANE_STATUS_CANCELLED;
    }
    
    //2. write the length of the buffer
    if (write(writeFd, len, INTSIZE) != INTSIZE)
    {
        writeToLog("Error in writing lenght to writeFd in saneHook.c");
        return SANE_STATUS_CANCELLED;
    }
    
    //3. write the buf
    if (write(writeFd, buf, *len) != *len)
    {
        writeToLog("Error in writing buf to writeFd in saneHook.c");
        return SANE_STATUS_CANCELLED;
    }

    return status;
}

SANE_Status sane_get_parameters (SANE_Handle h, SANE_Parameters * p){
    umask(0000);
    writeToLog("Sane get parameters is called\n");

    SANE_Status (*old_sane_get_parameters) (SANE_Handle h, SANE_Parameters * p);



    old_sane_get_parameters = dlsym(RTLD_NEXT, "sane_get_parameters");
    writeToLog("This is after getting the handle of old_sane_parameters\n");

    int status = old_sane_get_parameters(h, p);

    FILE* saneParams = fopen("/tmp/saneParams", "w+");
    fwrite(p, 1, sizeof(*p), saneParams);
    fclose(saneParams);

    char* buf = (char *)malloc(200);
    snprintf(buf, 200, "This is the bytes per line %d", p->bytes_per_line);
    writeToLog(buf);
    memset(buf, 0, 200);

    snprintf(buf, 200, "This is the pixels per line %d", p->pixels_per_line);
    writeToLog(buf);
    memset(buf, 0, 200);
    
    free(buf);
    //fclose(saneParams);
    umask(0022);
    return status;
}


// 