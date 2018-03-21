

#include "linServer.h"

bool initialized = false;
int portno;
int sockfd, newsockfd;
socklen_t clilen;
struct sockaddr_in cliaddr, servaddr;
// char *message;

int recvMsg(char *message)
{
    if (initialized == false)
    {
        connectTo("2222\0");
    }
    memset((void *)message, 0, MAX_MESSAGE_SIZE);
    read(newsockfd, message, MAX_MESSAGE_SIZE); //This reads the print messages to be written in stdout

    return 0;
}

scannerDevs recvDevs()
{

    if (initialized == false)
    {
        connectTo("2222\0");
    }

    char *data = (char *)malloc(sizeof(scannerDevs));
    //memset(data,0,sizeof(data));

    scannerDevs devsPacket;

    int stat = read(newsockfd, data, sizeof(scannerDevs)); //This reads the data arguments of the DSM_Entry function

    // int stat = read(newsockfd, data, sizeof(scannerDevs));
    if (stat == -1)
    {
        printf("error in readnig from socket from recvDevs!!!!");
    }
    else
    {

        printf("read from socket \n");
    }

    deserializeScannerDevs(data, &devsPacket);

    return devsPacket;
}

fParameters recvfParams()
{
    if (initialized == false)
    {
        connectTo("2222\0");
    }
    char *paramData = (char *)malloc(sizeof(fParameters));
    fParameters paramPacket;
    read(newsockfd, paramData, sizeof(fParameters)); //This reads the data arguments of the DSM_Entry function
    deserializeFParameters(paramData, &paramPacket); //Convert the serialized character array to fParameters structure
    return paramPacket;
}

int sendRetStruct(retStruct *pRet)
{
    if (initialized == false)
    {
        connectTo("2222\0");
    }
    char *retData = (char *)malloc(sizeof(retStruct));
    serializeRetStruct(pRet, retData);
    send(newsockfd, retData, sizeof(retStruct), 0);
    return 0;
}

int sendImage(char *path)
{
    if (initialized == false)
    {
        connectTo("2222\0");
    }
    //check if its a proper path and the path exists
    int image_fd = open(path, O_RDONLY);
    if (image_fd == -1)
    {
        printf("Could not open the path error is %s\n", strerror(errno));

        return -1;
    }
    struct stat filestat;
    fstat(image_fd, &filestat);
    send(newsockfd, &(filestat.st_size), sizeof(int), 0);

    // char* imgData = (char* )malloc(MAX_FILE_SIZE);
    // int status;
    // status = read(image_fd, imgData, filestat.st_size);
    // if(status == filestat.st_size){
    //     int offset = 0;
    //     char* tempImgData = imgData;
    //     while(true){
    //         status = send(newsockfd, tempImgData, BUFSIZE, 0);
    //         if(status = )

    //     }

    // }

    int status = sendfile(newsockfd, image_fd, 0, filestat.st_size);
    if (status == -1)
    {
        printf("status value is -1, something is wrong....retry..\n");
        return -1;
    }
    else if (status != filestat.st_size)
    {
        printf("Full file not sent, retrying\n");
        return -1;
    }
    else
    {
        printf("%d amount of bytes has been transferred\n", filestat.st_size);
        return 0;
    }
}

int sendSaneParams()
{
    FILE *saneParamStream = fopen("/tmp/saneParams", "r");

    //This is what we read from the file directly
    char *paramData = (char *)malloc(sizeof(SANE_Parameters));
    //This is to convert the file data to network ordered data
    //char* paramDataX = (char *)malloc(sizeof(SANE_Parameters));

    // Reading from the file
    fread(paramData, 1, sizeof(SANE_Parameters), saneParamStream);

    // printf("Inside linServer.cpp , this is bytes per line: %d\n", ((SANE_Parameters*)paramData)->bytes_per_line);

    TW_UINT32 *p = (TW_UINT32 *)paramData;
    //TW_UINT32 *dataX = (TW_UINT16 *);
    TW_UINT32 temp;
    //memcpy(data, pmsgPacket, 320);

    for (int i = 0; i < sizeof(SANE_Parameters) / 4; i++)
    {
        memset(&temp, 0, sizeof(temp));
        temp = htonl(*p);
        memcpy(p, &temp, 4);
        //(TW_UINT32*)paramData++;
        p++;
    }

    //Sending the network ordered data
    assert(send(newsockfd, paramData, sizeof(SANE_Parameters), 0) == sizeof(SANE_Parameters));

    free(paramData);

    fclose(saneParamStream);
    //assert(send(newsockfd))
}

int sendImageStrip(char *imageDataStrip, int imageDataStripLen)
{
    int status;
    if (send(newsockfd, &imageDataStripLen, INTSIZE, 0) != INTSIZE)
    {
        printf("In sendImageStrip, error while sending the int\n");
        return -1;
    }

    if (imageDataStripLen == 0)
    {
        return 0;
    }

    status = send_r(newsockfd, imageDataStrip, imageDataStripLen, 0);
    if (status != imageDataStripLen)
    {

        printf("In sendImageStrip, error while sending the imageDataStrip\n");
        char *errMsg = strerror(errno);
        printf("This is the error message:: %s\n", errMsg);
        printf("This is the value of the imageDataStrinLen and status : %d and %d\n", imageDataStripLen, status);
        return -1;
    }

    return 0;
}

int send_r(int sock, void *buf, int totalBytes, int flags)
{
    int bytesWritten = 0;
    int written;
    while (bytesWritten != totalBytes)
    {
        written = send(sock, buf + bytesWritten, totalBytes - bytesWritten, flags);
        //while(written == -1 && errno == EINTR);
        if (written == -1)
        {
            return written;
        }
        bytesWritten += written;
    }
    return bytesWritten;
}

int connectTo(const char *a)
{
    // printf("In connect to\n");
    if (initialized == false)
    {

        initialized = true;
        portno = atoi(a);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(portno);

        bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        printf("after bind\n");

        listen(sockfd, 3);
        clilen = sizeof(cliaddr);
        printf("Before accept\n");
        newsockfd = accept(sockfd, (sockaddr *)&cliaddr, &clilen);
        printf("Initialized socket connection\n");
        // char message[BUFSIZE];
        // char paramData[512];
        // fParameters paramPacket;
        // retStruct *pRet = (retStruct *)malloc(sizeof(pRet));
        // while (1)
        // {
        //     printf("\n\n");
        //     memset(message, 0, BUFSIZE);

        //     read(newsockfd, message, BUFSIZE); //This reads the print messages to be written in stdout
        //     printf("%s\n", message);
        //     memset(message, 0, BUFSIZE);

        //     read(newsockfd, paramData, sizeof(fParameters)); //This reads the data arguments of the DSM_Entry function
        //     deserializeFParameters(paramData, &paramPacket); //Convert the serialized character array to fParameters structure
        //     pRet = switchFunction(paramPacket.Origin, paramPacket.Dest, paramPacket.DG, paramPacket.DAT, paramPacket.MSG);
        //     char retData[sizeof(retStruct)];
        //     serializeRetStruct(pRet, retData);
        //     send(newsockfd, retData, sizeof(retData), 0);
        // }

        // close(newsockfd);
        // close(sockfd);
        return 0;
    }
    else
    {
        printf("Initialized is true but still this function is called\n");
        return -1;
    }
}
int closeSocket()
{
    if (initialized == true)
    {
        initialized = false;
        close(newsockfd);
        close(sockfd);
        return 0;
    }
    else
    {
        printf("Initialized is false but still this is being called");
        return -1;
    }
}

void serializeRetStruct(retStruct *pRet, char *data)
{
    //retStruct ret;

    TW_UINT16 *p = (TW_UINT16 *)pRet;
    TW_UINT16 *dataX = (TW_UINT16 *)data;
    int temp;
    //memcpy(data, pmsgPacket, 320);
    for (int i = 0; i < sizeof(retStruct) / 2; i++)
    {

        temp = htons(*p);
        memcpy(dataX, &temp, 2);
        dataX++;
        p++;
    }
}

void serializeFParameters(fParameters *pmsgPacket, char *data)
{

    TW_UINT16 *p = (TW_UINT16 *)pmsgPacket;
    TW_UINT16 *dataX = (TW_UINT16 *)data;
    TW_UINT16 temp;
    //memcpy(data, pmsgPacket, 320);
    for (int i = 0; i < sizeof(fParameters) / 2; i++)
    {

        temp = htons(*p);
        memcpy(dataX, &temp, 2);
        dataX++;
        p++;
    }
}

void deserializeFParameters(char *data, fParameters *pmsgPacket)
{

    TW_UINT16 *p = (TW_UINT16 *)pmsgPacket;
    TW_UINT16 *dataX = (TW_UINT16 *)data;
    TW_UINT16 temp;
    //ntohl converts 4 bytes of data and so we copy 4 bytes at a time
    for (int i = 0; i < sizeof(fParameters) / 2; i++)
    {

        temp = ntohs(*dataX);
        memcpy(p, &temp, 2);
        p++; //Gets incremented by 4 bytes everytime as it is a int pointer
        dataX++;
    }

    //  printf("LINUX : The product family is: %s \n", &pmsgPacket->Origin.ProductFamily);
}

void deserializeScannerDevs(char *data, scannerDevs *pdevs)
{
    TW_UINT16 *pdevsX = (TW_UINT16 *)pdevs;
    TW_UINT16 *dataX = (TW_UINT16 *)data;
    TW_UINT16 temp;

    for (int i = 0; i < sizeof(scannerDevs) / 2; i++)
    {

        temp = ntohs(*dataX);
        memcpy(pdevsX, &temp, 2);
        dataX++;
        pdevsX++;
    }
}

void deserializeRetStruct(char *data, retStruct *pRet)
{

    TW_UINT16 *p = (TW_UINT16 *)pRet;
    TW_UINT16 *dataX = (TW_UINT16 *)data;
    int temp;
    //ntohl converts 4 bytes of data and so we copy 4 bytes at a time
    for (int i = 0; i < sizeof(retStruct) / 2; i++)
    {

        temp = ntohs(*dataX);
        memcpy(p, &temp, 2);
        p++; //Gets incremented by 2 bytes everytime as it is a int pointer
        dataX++;
    }

    //printf("CLIENT: The product family is: %s \n", &pmsgPacket->Origin.ProductFamily);
}
