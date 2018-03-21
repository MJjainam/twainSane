//Server program:w

//#include <cstdio>
//#include <sane.h>
//#include "myscanLib.h"
// #include <tchar.h>
#include "twainServer.h"
#include "linServer.h"
using namespace std;


SANE_Parameters parm;
struct Mysane bunch;
Image image;
scannerDevs devs;
SANE_Handle *pHandle = (SANE_Handle *)malloc(sizeof(SANE_Handle));
int status; //This will be used to check the return code of the system function calls

static void auth_callback(SANE_String_Const resource,
                          SANE_Char *username, SANE_Char *password)
{
}

retStruct *switchFunction(TW_IDENTITY Origin, TW_IDENTITY Dest, TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG)
{
    printf("in switch function\n");
    retStruct *pRet = (retStruct *)malloc(sizeof(retStruct));
    if (DG == DG_CONTROL)
    {
        if (DAT == DAT_PARENT)
        {
            switch (MSG)
            {
            case MSG_OPENDSM:
            {

                printf("LINUX: inside MSG_OPENDSM\n");
                int version_code;
                memset(pRet, sizeof(retStruct), 0);
                sane_exit();
                status = sane_init(&version_code, auth_callback);
                if (status == SANE_STATUS_GOOD)
                {
                    pRet->twrc = TWRC_SUCCESS;
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "Succesful run of the function sane_init\n");
                    printf("Succesful run of the function sane_init\n");
                }
                else
                {
                    pRet->twrc = TWRC_FAILURE;
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "error in sane_init\n");
                    printf("LINUX: error in sane_init\n");
                }

                break;
            }
            case MSG_CLOSEDSM:
            {

                printf("LINUX: Inside MSG_CLOSEDSM\n");
                sane_exit();
                memset(pRet, sizeof(retStruct), 0);
                pRet->twrc = TWRC_SUCCESS;
                snprintf(pRet->message, MAX_MESSAGE_SIZE, "sane-backends closed succesfully");
                printf("Closed sane_backend\n");
                break;
            }
            }
        }
        else if (DAT == DAT_IDENTITY)
        {
            switch (MSG)
            {
            case MSG_GETDEFAULT:
            {

                printf("LINUX: inside MSG_GETDEFAULT\n");
                memset(pRet, sizeof(retStruct), 0);
                pRet->twrc = TWRC_SUCCESS;
                snprintf(pRet->message, MAX_MESSAGE_SIZE, "This is MSG_GETDEFAULT, we do nothing here");
                break;
            }
            case MSG_USERSELECT:
            {

                printf("LINUX: Inside MSG_USERSELECT\n");
                memset(pRet, sizeof(retStruct), 0);
                if (SANE_STATUS_GOOD == sane_get_devices(&bunch.devlist, SANE_TRUE))
                {
                    pRet->twrc = TWRC_SUCCESS;
                    //int totalDevices = sizeof(bunch.devlist)/sizeof(*bunch.devlist);
                    // int totalDevices = 3; //Should get the total devices based on the number of devices connected
                    // int totalDevices = strlen(bunch.devlist)/sizeof(bunch.devlist[0]);
                    // printf("size of bunc,devlist is %d", sizeof(bunch.devlist) );
                    // printf("Total number of devices found is %d", totalDevices );
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "Got sane devices");
                    printf("Got SANE devices succesfully\n");

                    // printf("This is the name of the device %s\n", bunch.devlist[0]->name);
                    // printf("This is the vendor of the device %s\n", bunch.devlist[0]->vendor);
                    // printf("This is the model of the device %s\n", bunch.devlist[0]->model);
                    // printf("This is the type of the device %s\n", bunch.devlist[0]->type);

                    // printf("This is the pRet->message %s \n", pRet->message);
                    printf("size of ret struct is : %d ", sizeof(retStruct));
                    int totalDevices = 0;
                    for (int i = 0; i < 100; i++)
                    // while(true)
                    {
                        if (bunch.devlist[i] == NULL)
                        {
                            break;
                        }
                        totalDevices++;
                        strncpy(devs.devinfo[i].name, bunch.devlist[i]->name, strlen(bunch.devlist[i]->name) + 1);
                        strncpy(devs.devinfo[i].vendor, bunch.devlist[i]->vendor, strlen(bunch.devlist[i]->vendor) + 1);
                        strncpy(devs.devinfo[i].model, bunch.devlist[i]->model, strlen(bunch.devlist[i]->model) + 1);
                        strncpy(devs.devinfo[i].type, bunch.devlist[i]->type, strlen(bunch.devlist[i]->type) + 1);

                        printf("\nThis should print the name %s\n", devs.devinfo[i].name);
                        printf("\nThis should print the vendor %s\n", devs.devinfo[i].vendor);
                        printf("\nThis should print the model %s\n", devs.devinfo[i].model);
                        printf("\nThis should print the type %s\n", devs.devinfo[i].type);
                        printf("\n");

                        // printf("The length of the bunch.devlist[i]->name) is %d\n", strlen(bunch.devlist[i]->name));
                    }
                    devs.numOfDevices = totalDevices;
                    pRet->devs = devs;
                    // pRet->devs.numOfDevices = totalDevices;
                }
                else
                {
                    printf("LINUX: Failure in getting all the connected devices\n");
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "Did not get sane devices\n");
                    pRet->twrc = TWRC_FAILURE;
                }

                break;
            }

            /*
                In this we are scannimg and transferring the image
            */
            case MSG_OPENDS:
            {
                printf("Linux: Inside MSG_OPENDS \n");
                memset(pRet, sizeof(retStruct), 0);

                scannerDevs tempDevs = recvDevs();
                devs.curDevID = tempDevs.curDevID;

                if (devs.curDevID < 0 || devs.curDevID > MAX_DEVICES)
                {
                    printf("The devs.curDevId is not in the range 0 - MAX_DEVICES\n");
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "Not in range 0-100 error!!");
                    pRet->twrc = TWRC_FAILURE;
                }
                else if (devs.curDevID == 0)
                {
                    printf("The devs.curDevId == 0, This means we have to let the user select \n");
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "devs.curDevId = 0, opening default device");

                    // Close the sane coz it is opened
                    sane_exit();
                    //Open up xscanner here without specifying any scanner
                    status = system("xsane");
                    if (status != 0)
                    {
                        cout << "some error";
                        pRet->twrc = TWRC_FAILURE;
                    }
                    else
                    {
                        if (sendImage((char *)"/home/radix/out.jpeg\0") == -1)
                        {
                            printf("Somwething went wrong when sending the file... error is %s\n", strerror(errno));
                            pRet->twrc = TWRC_FAILURE;
                        }
                        else
                        {
                            pRet->twrc = TWRC_SUCCESS;
                        }
                    }
                }
                else
                {
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "devs.curDevId is selected");

                    //Using sane_exit so that it won't be a problem when the "xsane starts sane_start"
                    sane_exit();


                    //I'll make a FIFO here -----------------------------------------------------------------------------READER
                    //In progress
                    
                    int fork_result = fork();
                    if (fork_result == 0) // Child will run the xsane command
                    {

                        char *xsaneCommand = (char *)malloc(MAX_MESSAGE_SIZE);
                        snprintf(xsaneCommand, MAX_MESSAGE_SIZE, "export LD_PRELOAD=\"/home/radix/Desktop/Git/saneHook/saneHook.so\";xsane %s", devs.devinfo[devs.curDevID - 1].name);
                        printf("xsaneCommand is %s\n", xsaneCommand);

                        status = system(xsaneCommand);
                        if (status != 0)
                        {
                            cout << "some error in running system(xsaneCommand)\n";
                            pRet->twrc = TWRC_FAILURE;
                        }
                        printf("running free\n");
                        free((void *)xsaneCommand);
                        exit(0);
                    }

                  
                    //----------------------------------------------------------------------------------------------------READER-END
                    //Parent will read the bytes
                    //else
                    //{ 
                        char *imageDataFifo = (char*)"/tmp/imageDataFifo";
                        char buffer[2 * 32768];
                        //Should we do memset(buffer, 0 , sizeof(buffer)) here as that might slow down the execution time?
                        SANE_Status sane_status;
                        mkfifo(imageDataFifo, 0666);
                        int readFd = open(imageDataFifo, O_RDONLY);
                        int bufLen;
                   
                        while (1)
                        {
                            //printf("Loop number is %d:", i++);
                            //1. Read the sane_status
                            memset(buffer, 0, sizeof(buffer));
                            //Read the sane status
                            assert(read(readFd, buffer, INTSIZE /*bytes*/)== INTSIZE);
                            //printf("This is the buffer value when reading sane_status: %d", *(int *)buffer);
                            sane_status = *(SANE_Status *)buffer;
                            if (sane_status == SANE_STATUS_EOF)
                            {
                                printf("Reached SANE_STATUS_EOF \n");
                                assert(sendImageStrip((char *)0,0) == 0);
                                break;
                            }
                            else if (sane_status == SANE_STATUS_GOOD)
                            {
                                //2. Read the length of the next incoming buf
                                status = read(readFd, buffer, INTSIZE);
                                if(status!=INTSIZE){
                                    printf("Status value is unexpected, %d\n", status);
                                    char* buf = (char*)malloc(260);
                                    printf("This is the error string: %s\n ",strerror_r(errno, buf, 256));
                                    free(buf);
                                    sane_exit();
                                    exit(EXIT_FAILURE);  
                                }



                             
                                bufLen = *(int *)buffer;
                                printf("This is the length of the buffer: %d\n", bufLen);
                                //3. Read the buffer here
                                int readLen = read(readFd, buffer, bufLen); //If read return zero then some error has occured
                                if (readLen != bufLen)
                                {
                                    printf("readlen != bufLen\n");
                                    sane_exit();
                                    exit(EXIT_FAILURE);
                                }
                                //4. Send the image strip to Windows here
                                sendImageStrip(buffer, bufLen);
                                
                                
                            }
                            else
                            {
                                printf("Something is wrong in SANE_Status, the value is %d\n", sane_status);
                                sane_exit();
                                exit(EXIT_FAILURE);
                            }
                            //read until eof is reached i.e totalSize bytes
                            //And send it over the network to windows.
                        }

                        //Read the sane_get_parameters values here and send it to windows



                        pRet->twrc = TWRC_SUCCESS;
                        snprintf(pRet->message, MAX_MESSAGE_SIZE, "Succesfully read all bytess");
                        printf("Succesfully read all bytes\n");
                        sendSaneParams();



                        /* This part sends the image after its fully scanned*/
                        
                        // if (sendImage((char *)"/home/radix/out.jpeg\0") == -1)
                        // {
                        //     printf("Somwething went wrong when sending the file... error is %s\n", strerror(errno));
                        //     pRet->twrc = TWRC_FAILURE;
                        // }
                        // else
                        // {
                        //     pRet->twrc = TWRC_SUCCESS;
                        // }
                   
                    //}
                }

                break;
            }
            }
        }
        else if (DAT == DAT_CAPABILITY)
        {
            switch (MSG)
            {
            case MSG_SET:
            {

                memset(pRet, sizeof(retStruct), 0);
                snprintf(pRet->message, MAX_MESSAGE_SIZE, "Nothing to do in DAT_CAPABILITY / MSG_SET for now\n");

                //should fill this
                //This is to set the capabilities of the source by the application
                //Capabilities are something that a data source offers.
                //The capability in MSG_SET / MSG_GET is specified by the application using Cap identifier in TW_CAPABILITY structure
                //That is the structure pointed by pData memeber
                //can be fille later.

                break;
            }
            }
        }
        else if (DAT == DAT_USERINTERFACE)
        {
            switch (MSG)
            {
            case MSG_ENABLEDS:
            {
                memset(pRet, sizeof(retStruct), 0);
                cout << "before first if in MSG_ENABLE_DS\n ";
                if (*pHandle == 0)
                {
                    printf("pHandle == 0\n");
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "LINUX: Error! cannot execute sane_start coz pHandle is not set to anything");
                }
                else
                {
                    cout << "phandle is not zero\n";
                    snprintf(pRet->message, MAX_MESSAGE_SIZE, "LINUX: Mapping MSG_ENABLEDS to sane_start()");
                    status = sane_start(*pHandle);
                    if (status == SANE_STATUS_GOOD)
                    {
                        printf("LINUX: successfully executed sane_start()\n");
                        snprintf(pRet->message, MAX_MESSAGE_SIZE, "LINUX: successfully executed sane_start()");
                    }
                    else
                    {
                        printf("error in sane_start()");
                        snprintf(pRet->message, MAX_MESSAGE_SIZE, "LINUX: error in sane_start()");
                    }
                }
            }
            }
        }
    }

    else if (DG == DG_IMAGE)
    {
        if (DAT == DAT_IMAGENATIVEXFER && MSG == MSG_GET)
        {
            printf("Inside DG_IMAGE, DAT_IMAGENATIVEXFER and MSG_GET\n");

            //Do the conversion from jpeg to bmp here

            //Send the file here
            pRet->twrc = TWRC_SUCCESS;
            snprintf(pRet->message, MAX_MESSAGE_SIZE, "Converted and sent the file successfully");
        }
    }
    return pRet;
}
//#include "structures.h"
int main(int argc, char *argv[])
{

    printf("size fo retStruct is %d : \n", sizeof(retStruct));
    printf("size of scannerDevs is %d \n", sizeof(scannerDevs));

    while (1)
    {
        printf("\n\n");
        char *message = (char *)malloc(MAX_MESSAGE_SIZE);
        recvMsg(message);
        printf("%s\n", message);

        fParameters paramPacket;
        paramPacket = recvfParams();

        retStruct *pRet;
        //pRet = (retStruct *)malloc(sizeof(retStruct));
        pRet = switchFunction(paramPacket.Origin, paramPacket.Dest, paramPacket.DG, paramPacket.DAT, paramPacket.MSG);

        sendRetStruct(pRet);
    }
}
