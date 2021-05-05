#include "..\include\phiPortSettings.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Windows.h"
#include "stdio.h"
#include "string.h"
#include "tchar.h"
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// general settings on Serial Port connection on WINDOWS API

BOOL phiSpInitialization(phiSerialPortParametersPtr ptrPSP,
                         TCHAR *portName,
                         const char *openFormat,
                         DWORD phiBaudrate,
                         BYTE phiByteSize,
                         BYTE phiStopBit,
                         BYTE phiNoParity)
{
    // initialize the general settings on COM PORT
    phiGeneralInitialize(ptrPSP);
    int selectionForOpeningFormat = 0;

    if (strcmp(openFormat, "r") == 0)
    {
        selectionForOpeningFormat = PHI_READING_FORMAT;
    }

    if (strcmp(openFormat, "w") == 0)
    {
        selectionForOpeningFormat = PHI_WRITING_FORMAT;
    }

    switch (selectionForOpeningFormat)
    {
    case PHI_READING_FORMAT:
        phiCreatePortForReading(ptrPSP, portName);

        phiDCBSerialParameterSettings(ptrPSP,
                                      phiBaudrate,
                                      phiByteSize,
                                      phiStopBit,
                                      phiNoParity);

        phiTimeOutsParametersSettings(ptrPSP, 10, 10, 10, 10, 10);
        break;

    case PHI_WRITING_FORMAT:
        phiCreatePortForWriting(ptrPSP, portName);

        phiDCBSerialParameterSettings(ptrPSP,
                                      phiBaudrate,
                                      phiByteSize,
                                      phiStopBit,
                                      phiNoParity);

        phiTimeOutsParametersSettings(ptrPSP, 10, 10, 10, 10, 10);
        break;

    default:
        ptrPSP->lastError = ERROR_OPEN_FILE_FORMAT;
        break;
    }

    return (ptrPSP->lastError == PHI_OK);
}

BOOL phiStartSerialConnection(phiSerialPortParametersPtr ptrPSP)
{
    // Setting receive mask to detect any events for a given port
    ptrPSP->Status = SetCommMask(ptrPSP->phiComm, EV_RXCHAR);

    if (ptrPSP->Status == FALSE)
    {
        ptrPSP->lastError = ERROR_SET_COMM_MASK;
        return FALSE;
    }

    ptrPSP->Status = WaitCommEvent(ptrPSP->phiComm, &(ptrPSP->phiDwEventMask), NULL);
    if (ptrPSP->Status == FALSE)
    {
        ptrPSP->lastError = ERROR_WAIT_COMM_MASK;
        return FALSE;
    }

	return TRUE;
}

BOOL phiReadData(phiSerialPortParametersPtr ptrPSP)
{
    // opening a write file
    ptrPSP->inputFile = fopen(ptrPSP->fileName, "w");

    if (ptrPSP->inputFile == NULL)
    {
        printf("Text file cannot be opened!\n");
        exit(EXIT_FAILURE);
    }

    int counterLoop = 0;
    while (counterLoop < 10000)
    {
        do
        {
            ptrPSP->Status = ReadFile(ptrPSP->phiComm,
                                      &(ptrPSP->phiReadData),
                                      sizeof(ptrPSP->phiReadData),
                                      &(ptrPSP->phiFinalBytesRead),
                                      NULL);

            ptrPSP->phiBuffer[ptrPSP->loop] = ptrPSP->phiReadData;
            ++(ptrPSP->loop);

        } while ((ptrPSP->phiFinalBytesRead > 0) && (ptrPSP->phiReadData != '\n'));

        // actual data
        --(ptrPSP->loop);

        int index = 0;
        for (index = 0; index < ptrPSP->loop; ++index)
        {
            printf_s("%c", ptrPSP->phiBuffer[index]);

            // skipping the first wrong term due to the transmission begin!
            if (counterLoop > 1)
            {
                fprintf(ptrPSP->inputFile, "%c", ptrPSP->phiBuffer[index]);
            }
        }
        printf_s("\n");

        ptrPSP->loop = 0;
        counterLoop++;
    }

    fclose(ptrPSP->inputFile);
}

BOOL phiWriteData(phiSerialPortParametersPtr ptrPSP)
{
    //Writing data to Serial Port
    ptrPSP->Status = WriteFile(ptrPSP->phiComm,
                               ptrPSP->phiBuffer,
                               sizeof(ptrPSP->phiBuffer),
                               &(ptrPSP->phiByteWritten),
                               NULL);

    if (ptrPSP->Status == FALSE)
    {
        ptrPSP->lastError = ERROR_WRITE_DATA;
    }
    return (ptrPSP->lastError == PHI_OK);
}

/////////////////////////////////////////////////////////

BOOL phiCreatePortForReading(phiSerialPortParametersPtr ptrPSP, TCHAR *portName)
{
    ptrPSP->phiComm = INVALID_HANDLE_VALUE;

    printf("Phi Communication Port is waiting...\n");

    ptrPSP->pcCommPort = portName;

    while (ptrPSP->phiComm == INVALID_HANDLE_VALUE)
    {
        // creating serial com port
        ptrPSP->phiComm = CreateFile(ptrPSP->pcCommPort,
                                     GENERIC_READ,
                                     0,
                                     NULL,
                                     OPEN_EXISTING,
                                     0,
                                     NULL);
    }

    printf("Phi Communication Port is open...\n");

    return TRUE;
}

BOOL phiCreatePortForWriting(phiSerialPortParametersPtr ptrPSP, TCHAR *portName)
{
    ptrPSP->phiComm = INVALID_HANDLE_VALUE;

    printf("Phi Communication Port is waiting...\n");

    ptrPSP->pcCommPort = portName;

    while (ptrPSP->phiComm == INVALID_HANDLE_VALUE)
    {
        // creating serial com port
        ptrPSP->phiComm = CreateFile(ptrPSP->pcCommPort,
                                     GENERIC_WRITE,
                                     0,
                                     NULL,
                                     OPEN_EXISTING,
                                     0,
                                     NULL);
    }

    printf("Phi Communication Port is open...\n");

    return TRUE;
}

/////////////////////////////////////////////////////////
BOOL phiDCBSerialParameterSettings(phiSerialPortParametersPtr ptrPSP,
                                   DWORD phiBaudrate,
                                   BYTE phiByteSize,
                                   BYTE phiStopBit,
                                   BYTE phiNoParity)
{

    // initialize DCB parameters
    ptrPSP->phiDcbSerialParams.DCBlength = sizeof(ptrPSP->phiDcbSerialParams);

    ptrPSP->Status = GetCommState(ptrPSP->phiComm, &(ptrPSP->phiDcbSerialParams));

    if (ptrPSP->Status == FALSE)
    {
        ptrPSP->lastError = COM_GET_DCB_ERROR;
    	return FALSE;
    }

    // assigning the dcb serial parameters
    ptrPSP->phiDcbSerialParams.BaudRate = phiBaudrate;
    ptrPSP->phiDcbSerialParams.ByteSize = phiByteSize;
    ptrPSP->phiDcbSerialParams.StopBits = phiStopBit;
    ptrPSP->phiDcbSerialParams.Parity = phiNoParity;

    // setting the whole parameters for a given DCB parameter

    ptrPSP->Status = SetCommState(ptrPSP->phiComm, &(ptrPSP->phiDcbSerialParams));

    if (ptrPSP->Status == FALSE)
    {
        ptrPSP->lastError = COM_GET_DCB_ASSIGNING_ERROR;
    }

    return (ptrPSP->lastError == PHI_OK);
}

BOOL phiTimeOutsParametersSettings(phiSerialPortParametersPtr ptrPSP,
                                   int ReadIntervalTimeout,
                                   int ReadTotalTimeOutConstant,
                                   int ReadTotalTimeoutMultiplier,
                                   int WriteTotalTimeoutConstant,
                                   int WriteTotalTimeoutMultiplier)
{
    ptrPSP->timeouts.ReadIntervalTimeout = ReadIntervalTimeout;
    ptrPSP->timeouts.ReadTotalTimeoutConstant = ReadTotalTimeOutConstant;
    ptrPSP->timeouts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
    ptrPSP->timeouts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
    ptrPSP->timeouts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;

    ptrPSP->Status = SetCommTimeouts(ptrPSP->phiComm, &(ptrPSP->timeouts));

    if (ptrPSP->Status == FALSE)
    {
		ptrPSP->lastError = COM_TIME_OUT_ERROR;
    }

    return (ptrPSP->lastError == PHI_OK);
}

void phiGeneralInitialize(phiSerialPortParametersPtr ptrPSP)
{
    // remember that these terms maybe initialized with zero!
    ptrPSP->phiDcbSerialParams;
    ptrPSP->timeouts;

    ptrPSP->loop = 0;
}

// general settings on Serial Port connection on WINDOWS API
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
// error handler code

// 


const char* phiGetErrorDescription(PHI_ERROR errorType){
	switch (errorType)
    {
    case ERROR_OPEN_FILE_FORMAT:
        return "File format is wrong to connect serial port. Please select -r- or -w- !!";
    case COM_GET_DCB_ERROR:
        return "Communication settings state on DCB cannot be assigned!";
    case COM_GET_DCB_ASSIGNING_ERROR:
        return "Serial Port Setting cannot get its states!";
    case COM_TIME_OUT_ERROR:
        return "Serial Port Time out Setting cannot be done!";
    case ERROR_WAIT_COMM_MASK:
        return "Serial Port Wait port setting cannot be done!";
    case ERROR_SET_COMM_MASK:
        return "Serial Port Mask Setting cannot be done!";
    case ERROR_WRITE_DATA:
        return "Program cannot write the data to the port!";
    default:
    	return "Error code doesn't have associated description!";
    }
       
    return "FATAL: That value must be inaccessible";
}

void phiErrorHandler(phiSerialPortParametersPtr ptrPSP, PHI_ERROR error)
{		 
	if(error == PHI_OK){
		// not an error, early termination.
		return;	
	}

	fprintf(stderr, "%s", phiGetErrorDescription(error));
	// optional, not suggested tho.
	// CloseHandle(ptrPSP->phiComm);
}

// error handler code
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
