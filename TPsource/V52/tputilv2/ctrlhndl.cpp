// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <windows.h>

BOOL CtrlHandler(DWORD fdwCtrlType) 
{     
	switch (fdwCtrlType) { 
        // Handle the CTRL+C signal.
		case CTRL_C_EVENT:   
        case CTRL_BREAK_EVENT:
			return TRUE;  
        // CTRL+CLOSE: confirm that the user wants to exit.  
        case CTRL_CLOSE_EVENT:
			return TRUE;  
        // Pass other signals to the next handler. 
		case CTRL_LOGOFF_EVENT:  
        case CTRL_SHUTDOWN_EVENT:
		default:
			return FALSE; 
    }
}

/*
void main(void) 
{
	BOOL fSuccess;  
    
	fSuccess = SetConsoleCtrlHandler( 
        (PHANDLER_ROUTINE) CtrlHandler,  // handler function 
        TRUE);                           // add to list     
	if (! fSuccess) 
        MyErrorExit("Could not set control handler"); 
}
*/