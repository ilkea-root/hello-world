/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKL46Z4.h"
#include "menu_event.h"
#include "menu_control.h"

uiUint32_t eventID, buttonID;

int main(void)
{


    /* Write your code here */

    /* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;) {
    	if(BUTTON_EVENT == eventID){
    		switch(buttonID){
    		case OK_BUTTON:
    			newMode = CheckModeChangeWithButton(setupChange,currentMode, alarmStatus, OK_BUTTON);
    			break;
    		case HOME_BUTTON:
    			newMode = CheckModeChangeWithButton(setupChange,currentMode, alarmStatus, HOME_BUTTON);
    			break;
    		case ANY_BUTTON:
    			newMode = CheckModeChangeWithButton(setupChange,currentMode, alarmStatus, ANY_BUTTON);
    			break;
    			//þimdilik up/down yok gerekirse ilave olur


				if(currentMode == newMode){
					//reset timeouts
					ResetTimeouts();
					//button function without modechange
					SetExceptionsFlow(menuFlowSt,setupExceptions, deviceTypeExceptions, currentMode, buttonID);
					ButtonFuncWithoutModeChange(currentMode, buttonID);
				}
				else
				{
					if(currentMode != newMode){
						ChangeTimeoutCondition(setupChange, newMode);
						ChangeMode();//modu deðiþtir hangi modda ne yapýlacaksa içine yazýlacak
					}
				}
    		};

    	}
    	else if(COMM_EVENT == eventID){

    		if(currentMode == CheckModeChangeWithCommunication(setupChange, currentMode, alarmStatus)){//if mode is not changed
    			//change timeout with communication
    			ChangeTimeoutWithCommunication(setupChange, currentMode);
    			//check alarm if it causes mode change
    			if(currentMode == CheckModeChangeWithAlarm(setupChange, currentMode, alarmStatus)){
    				if(currentMode == CheckModeChangeWithTimeouts(setupChange, currentMode)){
    					//refresh screen
    				}
    				else{//mode change with timeoutut
    					ChangeTimeoutCondition(setupChange, newMode);
						ChangeMode();//modu deðiþtir hangi modda ne yapýlacaksa içine yazýlacak
    				}
    			}
    			else{//mode change with alarm
    				ChangeTimeoutCondition(setupChange, newMode);
					ChangeMode();//modu deðiþtir hangi modda ne yapýlacaksa içine yazýlacak
    			}
    		}
    		else{//mode change with comm
    			ChangeTimeoutCondition(setupChange, newMode);
				ChangeMode();//modu deðiþtir hangi modda ne yapýlacaksa içine yazýlacak

    		}
    	}



    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
