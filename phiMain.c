///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// serial port application
#include "include\phiPortSettings.h"

int main()
{
    phiSerialPortParameters phiSP;

    phiSP.fileName = "portReading.txt";

    phiSpInitialization(&phiSP, "COM3", "r", CBR_115200, 8, ONESTOPBIT, NOPARITY);

    phiStartSerialConnection(&phiSP);

    phiReadData(&phiSP);

    return 0;
}

// serial port application
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
