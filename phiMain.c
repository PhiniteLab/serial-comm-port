///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
// serial port application
#include "include\phiPortSettings.h"

int main()
{
    phiSerialPortParameters phiSP;

    phiSP.fileName = "portReading.txt";

    if(TRUE == phiSpInitialization(&phiSP, "COM3", "r", CBR_115200, 8, ONESTOPBIT, NOPARITY)){
		if(TRUE == phiStartSerialConnection(&phiSP)){
			if(TRUE == phiReadData(&phiSP)){
				// success!				
			}
			else{
				fprintf(stdout, "Unable to read serial port, error desc : %s\n", phiGetErrorDescription(phiSP.lastError));				
			}
		}
		else{
			 fprintf(stdout, "Unable to start serial connection, error desc : %s\n", phiGetErrorDescription(phiSP.lastError)); 
		}
    	
    }
    else{
    	fprintf(stdout, "Error occured, desc : %s\n", phiGetErrorDescription(phiSP.lastError));
    }


    return 0;
}

// serial port application
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
