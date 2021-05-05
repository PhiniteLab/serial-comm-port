#ifndef __PHI_SERIAL_PORT_LIB_H__
#define __PHI_SERIAL_PORT_LIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

    ////////////////////////////////////////////////////////////
    // including some libraries for using input/output functions
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "Windows.h"
#include "stdio.h"
#include "string.h"
#include "tchar.h"

#include "phiPortSettings.h"
    // including some libraries for using input/output functions
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // define error types

typedef enum{
	PHI_OK = 0,
	ERROR_OPEN_FILE_FORMAT,
	COM_GET_DCB_ERROR,
	COM_GET_DCB_ASSIGNING_ERROR,
	COM_TIME_OUT_ERROR,
	ERROR_SET_COMM_MASK,
	ERROR_WAIT_COMM_MASK,
	ERROR_WRITE_DATA,
	COUNT
}PHI_ERROR;

    // define error types
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // define some constants related to the usage of Port

#define PHI_WRITING_FORMAT 99
#define PHI_READING_FORMAT 98

    // define some constants related to the usage of Port
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // port settings and storage structure

    typedef struct phiserialportparameters
    {

        //////////////////////////////////////////////////////////
        // windows API settings

        HANDLE phiComm; /*
                         * storing handle type of physical serial port
                         */

        BOOL Status; /*
                         * storing true or false value
                         * this is used to check whether operation is done or not!
                         */

        // port setting parameters
        DCB phiDcbSerialParams; /*
                                   * setting the Data/Device control block 
                                   */

        COMMTIMEOUTS timeouts; /*
                                     * setting the time out parameters
                                     */

        // windows API settings
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // read parameters in Windows API

        DWORD phiDwEventMask; /*
                               * wait communication event mask
                               */

        DWORD phiFinalBytesRead; /*
                                  * threshold for reading operation
                                  */

        TCHAR *pcCommPort; /*
                            * storing port name
                            */

        // read parameters in Windows API
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // user read operation variables

        unsigned char loop; /*
                             * storing how many elements are read in serial port
                             */

        char phiBuffer[500]; /*
                             * storing every element for a given array
                             */

        char phiReadData; /*
                          * storing instantaneous variable on serial port
                          */

        DWORD phiByteWritten; /*
                          * storing instantaneous variable on serial port
                          */

        // user read operation variables
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // writing text file

        FILE *inputFile; /*
                          * creating an inputfile to read the file
                          */
        char *fileName;  /*
                          * storing the name of file
                          */

		PHI_ERROR lastError;
        // writing text file
        //////////////////////////////////////////////////////////

    } phiSerialPortParameters;

    typedef phiSerialPortParameters *phiSerialPortParametersPtr;

    // port settings and storage structure
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // function prototypes

    BOOL phiSpInitialization(phiSerialPortParametersPtr ptrPSP,
                             TCHAR *portName,
                             const char *openFormat,
                             DWORD phiBaudrate,
                             BYTE phiByteSize,
                             BYTE phiStopBit,
                             BYTE phiNoParity);

    BOOL phiStartSerialConnection(phiSerialPortParametersPtr ptrPSP);

    BOOL phiReadData(phiSerialPortParametersPtr ptrPSP);

    ///////////////////////////////////////////////////////////

    BOOL phiCreatePortForReading(phiSerialPortParametersPtr ptrPSP, TCHAR *portName);

    BOOL phiCreatePortForWriting(phiSerialPortParametersPtr ptrPSP, TCHAR *portName);

    BOOL phiDCBSerialParameterSettings(phiSerialPortParametersPtr ptrPSP,
                                       DWORD phiBaudrate,
                                       BYTE phiByteSize,
                                       BYTE phiStopBit,
                                       BYTE phiNoParity);

    BOOL phiTimeOutsParametersSettings(phiSerialPortParametersPtr ptrPSP,
                                       int ReadIntervalTimeout,
                                       int ReadTotalTimeOutConstant,
                                       int ReadTotalTimeoutMultiplier,
                                       int WriteTotalTimeoutConstant,
                                       int WriteTotalTimeoutMultiplier);

    void phiGeneralInitialize(phiSerialPortParametersPtr ptrPSP);

    // function prototypes
    ////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////
    // error handler code

    /*
    * returns description of given error, if one is provided in table. 
    */
    const char* phiGetErrorDescription(PHI_ERROR errorType);


    void phiErrorHandler(phiSerialPortParametersPtr ptrPSP, PHI_ERROR error);
    /*
    * return error type
    * output -> return nothing
    * input -> address of phiSerialPortParametersPtr
    *          value of error type
    */

    // error handler code
    ////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif