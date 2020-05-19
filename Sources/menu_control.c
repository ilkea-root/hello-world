/*
 * menu_control.c
 *
 *  Created on: 25 Mar 2020
 *      Author: iakgol
 */

#include "menu_control.h"
#include"menu_event.h"
#include "uilibDef.h"
#include "menu_pages.h"


struct setupChange_t *setupChange;
struct setupGlobal_t *setupG;
struct setupExceptions_t *setupExceptions;

struct menuTimeout_t  menuTimeout[NUM_OF_TIMEOUTS]={
		{0,0,0,0, ChangeModeAutoScrollTimeout},
		{0,0,0,0, ChangeModeSmileyTimeout},
		{0,0,0,0, ChangeModeSettingsTimeout},
		{0,0,0,0, NoModeChangeAutoScrollPageTimeout}
};

struct ExceptionFlow_t exceptionFlow ={
		0,0,0,0,
};




// page_t root={0,0,0};
// page_t root1={0,0,0};
// subPages_t vlnSub={0,0,0};
// //page_t vln ={NULL,0,0,0,(page_t*)&root,(subPages_t *)vlnSub};
 //page_t vll= {0,0,0,0,0,(page_t*)&root1,(subPages_t *)vlnSub};




struct menuFlowStruct_t *menuFlowSt;



/*
 * Function: Enable Timeout
 * input: timeout index
 * return: -
 */
void EnableTimeout(uiUint32_t timeoutIndex)
{
	menuTimeout[timeoutIndex].timeoutStatus = 1;
}

/*
 * Function: Disable Timeout
 * input: timeout index
 * return: -
 */
void DisableTimeout(uiUint32_t timeoutIndex)
{
	menuTimeout[timeoutIndex].timeoutStatus = 0;
	menuTimeout[timeoutIndex].timeoutCount = 0;
}

/*
 * Function: GetTimeoutStatus
 * input: timeoutIndex
 * return:
 */

int GetTimeoutStatus(uiUint32_t timeoutIndex)
{
	return (menuTimeout[timeoutIndex].timeoutStatus);
}

/*
 * Function: ResetTimeout
 * timeout sayacýný baþtan baþlatma
 * input: timeoutIndex
 * return: -
 */
//resetAllTimeouts fonksiyonu yazýlacak-mod deðiþimi olmadýðý durum için!!!!
void ResetTimeout(uiUint32_t timeoutIndex)
{
	if(GetTimeoutStatus(timeoutIndex))
		menuTimeout[timeoutIndex].timeoutCount= 0;

}


void ResetTimeouts()
{
	for(uiUint8_t i=0; i < NUM_OF_TIMEOUTS; i++)
	{
		ResetTimeout(i);
	}
}

/*
 * Function: DisableAllTimeouts
 * timeout disabling
 * input: timeoutIndex
 * return: -
 */
void DisableAllTimeouts()
{
	for(uiUint8_t i=0; i < NUM_OF_TIMEOUTS; i++)
	{
		DisableTimeout(i);
	}
}





/*
 * Function: CheckTimeoutOverFlow
 *
 * input: timeoutIndex
 * return:timeout sayacýný bir arttýrýp limite ulaþmýþsa 1, deðilse 0 döner
 */
int CheckTimeoutOverflow(uiUint32_t timeoutIndex)
{
	menuTimeout[timeoutIndex].timeoutCount++;

	if(menuTimeout[timeoutIndex].timoutLimit >= menuTimeout[timeoutIndex].timeoutCount){
		return 1;
	}
	else{
		return 0;
	}
}

uiUint32_t CheckModeChangeWithTimeouts(const struct setupChange_t* scPtr, uiUint32_t currentMode)//timeout deðiþtiði
{

	for(uiUint8_t i=0; i < NUM_OF_TIMEOUTS; i++){
		if(CheckTimeoutOverflow(i)){
			return menuTimeout[i].timeoutFuncPtr(scPtr, currentMode);//new mode fr
		}
	}
	return mode;
}

/*
 * Function :ChangeTimeoutWithCommunication
 * mod geçiþi olmadýðýnda haberleþme ayarý deðiþtiðinde
 * enable/disable edililecek timeoutlar
 */
void ChangeTimeoutWithCommunication(const struct setupChange_t* scPtr, uiUint32_t mode)
{
	switch(mode){
		case NAVIGATION_MODE:
			if(GetTimeoutStatus(ENTER_AUTO_SCROLL_TIMEOUT_INDEX) && !scPtr->autoscrollSetting){
				DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
			}
			if(GetTimeoutStatus(ENTER_SMILEY_TIMEOUT_INDEX) && !scPtr->smileySetting){
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
			}
			if(GetTimeoutStatus(ENTER_SMILEY_TIMEOUT_INDEX) && scPtr->autoscrollSetting){
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
			}
			break;
		case AUTO_SCROLL_MODE:
			if( !scPtr->autoscrollSetting ){
				DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
				DisableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);
			}
			break;
		case NOTIFICATION_MODE:
		case SMILEY_MODE:
			if(GetTimeoutStatus(ENTER_SMILEY_TIMEOUT_INDEX) && !scPtr->smileySetting){
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
			}
			if(GetTimeoutStatus(ENTER_SMILEY_TIMEOUT_INDEX) && scPtr->autoscrollSetting){
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
			}
			break;


	};
}

/*
 * Function: ChangeTimeoutCondition
 * mod geçiþ yapýldýðýnda bir kez çaðýrýlacak
 *
 */
void ChangeTimeoutCondition(const struct setupChange_t* scPtr, uiUint32_t mode)
{
	switch(mode){
		case NAVIGATION_MODE://bunlar geçtiði modlar
			if(scPtr->autoscrollSetting){
				EnableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
				DisableTimeout(EXIT_SETTINGS_TIMEOUT_INDEX);
				DisableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);
			}
			else if(scPtr->smileySetting){
				EnableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
				DisableTimeout(EXIT_SETTINGS_TIMEOUT_INDEX);
				DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
				DisableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);

			}
			else{ // if(!scPtr->smileySetting && !scPtr->autoscrollSetting){
				DisableAllTimeouts();
			}

			break;
		case SETTINGS_MODE:
			EnableTimeout(EXIT_SETTINGS_TIMEOUT_INDEX);
			DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
			DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
			DisableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);
			break;
		case AUTO_SCROLL_MODE:
			if(scPtr->autoscrollSetting){
				EnableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);
				DisableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
				DisableTimeout(EXIT_SETTINGS_TIMEOUT_INDEX);
				DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
			}
			else{
				DisableAllTimeouts();
			}
			break;
		case NOTIFICATION_MODE:
			if(!scPtr->autoscrollSetting && scPtr->smileySetting){
				EnableTimeout(ENTER_SMILEY_TIMEOUT_INDEX);
				DisableTimeout(CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX);
				DisableTimeout(EXIT_SETTINGS_TIMEOUT_INDEX);
				DisableTimeout(ENTER_AUTO_SCROLL_TIMEOUT_INDEX);
			}
			else{
				DisableAllTimeouts();
			}
			break;
		case SMILEY_MODE:
			DisableAllTimeouts();
			break;


	};

}

uiUint32_t CheckModeChangeWithButton(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus,  uiUint32_t buttonPressed )
{
	switch(currentMode){
	case SMILEY_MODE:
		if(ANY_BUTTON == buttonPressed && !scPtr->autoscrollSetting && ALARM == alarmStatus){
			return NOTIFICATION_MODE;
		}
		return SMILEY_MODE;
		break;
	case AUTO_SCROLL_MODE:
		if(ANY_BUTTON == buttonPressed){
			return NAVIGATION_MODE;
		}
		return AUTO_SCROLL_MODE;
		break;
	case FIRST_TIME_INSTALLATION_MODE:
		// son first time installation page de yapýlýyor bu kýsým o eklenecek
//		if(OK_BUTTON == buttonPressed){
//			return NAVIGATION_MODE;
//		}
		return FIRST_TIME_INSTALLATION_MODE;
		break;
//notification da snooze veya snozeall, navigation'a dönme kýsmý var
	case NOTIFICATION_MODE:
		if(HOME_BUTTON == buttonPressed || (OK_BUTTON == buttonPressed && LAST_ALARM == alarmStatus)){// ikincisi snoozeall
			return NAVIGATION_MODE;
		}
		return NOTIFICATION_MODE;
		break;
	case TUTORIAL_MODE:
		//son tutorial sayfasýnda OK tuþuna basýlmasý
//		if(OK_BUTTON == buttonPressed){
//			return NAVIGATION_MODE;
//		}
		break;
	case NAVIGATION_MODE:
		if(TUTORIAL_BUTTON == buttonPressed){
			return TUTORIAL_MODE;
		}
		return NAVIGATION_MODE;
		break;

	};
}

uiUint32_t CheckModeChangeWithCommunication(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus)
{
	switch(currentMode){
			case SMILEY_MODE:
				if(!scPtr->smileySetting && !scPtr->autoscrollSetting){
					if(alarmStatus == ALARM)
						return NOTIFICATION_MODE;
					else
						return NAVIGATION_MODE;
				}
				if(scPtr->autoscrollSetting){
					return AUTO_SCROLL_MODE;
				}
				return SMILEY_MODE;

				break;
			case AUTO_SCROLL_MODE:
				if(!scPtr->autoscrollSetting){
					return NAVIGATION_MODE;
				}
				return AUTO_SCROLL_MODE;
				break;
			case NOTIFICATION_MODE:

				if(scPtr->autoscrollSetting){
					return AUTO_SCROLL_MODE;
				}
				return NOTIFICATION_MODE;
				break;
	};
}


/**************************************************
 * Timeout ile mod geçiþ fonksiyonlarý
 */
uiUint32_t ChangeModeAutoScrollTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode)
{

	if(NAVIGATION_MODE == currentMode && scPtr->autoscrollSetting){
					return AUTO_SCROLL_MODE;
	}
}

uiUint32_t ChangeModeSmileyTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode)
{
	if(NOTIFICATION_MODE == currentMode && !scPtr->autoscrollSetting && scPtr->smileySetting){
		return SMILEY_MODE;
	}
	if(NAVIGATION_MODE == currentMode && !scPtr->autoscrollSetting && scPtr->smileySetting){
		return SMILEY_MODE;
	}

}

uiUint32_t ChangeModeSettingsTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode)
{
	return NAVIGATION_MODE;// deðiþiklik yoksa discard fonksiyonu çaðýrýlabilir.
}

uiUint32_t NoModeChangeAutoScrollPageTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode)
{
	return 0;
}

/**********************************
 * Alarma baðlý mod geçiþ durumlarý
 *
 */

uiUint32_t CheckModeChangeWithAlarm(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus)
{
	switch(currentMode){
		case NOTIFICATION_MODE:
			if(NO_ALARM == alarmStatus || ALARM_RECOVERY == alarmStatus){//simdilik boyle uydurmaca alarm dönüþ kýsmý fonksiyon geri dönüþü ile olmalý
				return NAVIGATION_MODE;
			}
			break;
		case NAVIGATION_MODE:
			if(ALARM == alarmStatus && !scPtr->autoscrollSetting){
				return NOTIFICATION_MODE;
			}
			break;

	};
}

/*
 * Function: Change Mode
 * input: currentMode
 * return : newMode
 */
void ChangeMode()
{
	currentMode = newMode;
}
/////START---------------------------EXTERNAL FUNTIONS OF LIBRARY COMING FROM PROJECT
void FillSetupChangeStruct()
{
	setupChange->autoscrollSetting= setupG->autoscroll;
	setupChange->backlightSetting = setupG->backlight;
	setupChange->smileySetting = setupG->smiley;

}


void FillExceptions()
{
	setupExceptions->alarm = setupG->alarm;
	setupExceptions->password = setupG->password;
	deviceTypeExceptions->Harmonics = deviceType->HARMONICS;
	deviceTypeExceptions->Modbus = deviceType->MODBUS;
}
/*
 * Function: SetExceptions
 */
void SetExceptionsFlow(struct menuFlowStruct_t * menuFlowSt, struct setupExceptions_t *sePtr, struct deviceTypeExceptions_t *dePtr, uiUint32_t currentMode, uiUint32_t buttonID)
{
	switch(currentMode){
		case NAVIGATION_MODE:
			if(sePtr->alarm){
				exceptionFlow.deviceTypeExceptionFlow = 0;
				exceptionFlow.settingChangeExceptionFlow = PAGE_SEQ_IN_NAV;

				if(menuFlowSt->currentPage == &root){
					exceptionFlow.exceptionTrigger = UP_BUTTON;
					exceptionFlow.ExceptionFlowStruct.currentPage = menuFlowSt->currentPage;
					exceptionFlow.ExceptionFlowStruct.nextPage = &vln;
					exceptionFlow.ExceptionFlowStruct.prevPage = &vll;
				}
				if(menuFlowSt->currentPage == &vll){
					exceptionFlow.exceptionTrigger = DOWN_BUTTON;
					exceptionFlow.ExceptionFlowStruct.currentPage = menuFlowSt->currentPage;
					exceptionFlow.ExceptionFlowStruct.nextPage = &root;
					exceptionFlow.ExceptionFlowStruct.prevPage = &vln;
				}

			}
			if(dePtr->Harmonics){
				exceptionFlow.deviceTypeExceptionFlow = PAGE_SEQ_IN_NAV;
				exceptionFlow.exceptionTrigger = buttonID;
				exceptionFlow.ExceptionFlowStruct.backupPage = &root;
				exceptionFlow.ExceptionFlowStruct.currentPage = &vln;
			}
			break;
		case SETTINGS_MODE:
			if(sePtr->password){
				exceptionFlow.settingChangeExceptionFlow = PAGE_SEQ_IN_NAV;
				exceptionFlow.exceptionTrigger = buttonID;
			}
			if(sePtr->foo){
				exceptionFlow.settingChangeExceptionFlow = PAGE_SEQ_IN_SETTING;
				if(menuFlowSt->currentPage == &vll){
					exceptionFlow.exceptionTrigger = UP_BUTTON;
					exceptionFlow.ExceptionFlowStruct.currentPage = menuFlowSt->currentPage;
					exceptionFlow.ExceptionFlowStruct.nextPage = &root;
					exceptionFlow.ExceptionFlowStruct.prevPage = &vln;
				}
			}

			break;
	};

}

void SetButtonActionNav(struct menuFlowStruct_t * menuFlowSt, struct buttonAction_t* buttonActionSt, uiUint32_t currentMode, uiUint32_t buttonID)
{
	switch(currentMode){
		case NAVIGATION_MODE:
			if(menuFlowSt->currentPage->pageFeature){

			}
			break;
	};
}


///// END--------------------EXTERNAL FUNTIONS OF LIBRARY COMING FROM PROJECT

struct menuFlowStruct_t * ChangePageSeq(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID)
{
	 struct menuFlowStruct_t *menuFlow = menuFlowSt;

	if (menuFlowSt->currentPage == seFlowPtr->ExceptionFlowStruct.currentPage){
		if(seFlowPtr->exceptionTrigger == buttonID){
			menuFlowSt->nextPage = seFlowPtr->ExceptionFlowStruct.nextPage;
			menuFlowSt->prevPage = seFlowPtr->ExceptionFlowStruct.prevPage;
			menuFlow = &(seFlowPtr->ExceptionFlowStruct);
		}
	}

	return menuFlow;// çözüm bulunmalý lokal deðiþken yerine global tanýmlama yapýlmalý scope deðiþtiði için problem olacak!!!!

}
struct menuFlowStruct_t * CheckExceptionsPageSeqInNav(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr, uiUint32_t buttonID)
{
	if(seFlowPtr->settingChangeExceptionFlow == PAGE_SEQ_IN_NAV){
		return ChangePageSeq(menuFlowSt, seFlowPtr, buttonID);
	}

}



struct menuFlowStruct_t * CheckExceptionsPageSeqInSetting(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID)
{

	if(seFlowPtr->settingChangeExceptionFlow == PAGE_SEQ_IN_SETTING){
		return ChangePageSeq(menuFlowSt, seFlowPtr, buttonID);
	}


}

struct menuFlowStruct_t * CheckExceptionsPageSeqInCommissioning(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID)
{
	if(seFlowPtr->settingChangeExceptionFlow == PAGE_SEQ_IN_COMMISSION){
		return ChangePageSeq(menuFlowSt, seFlowPtr, buttonID);
	}
}

///Page Item Check functions before Changing page()
struct menuFlowStruct_t * CheckExceptionPageItemInNav(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID)
{
	if(exceptionFlow.settingChangeExceptionPageItem == PAGE_ITEM_IN_NAV){
		return ChangePageItem(menuFlowSt, seFlowPtr);
	}
}

struct menuFlowStruct_t * ChangePageItem(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr)
{
	struct menuFlowStruct_t *menuFlow = menuFlowSt;

	if (menuFlowSt->currentPage == seFlowPtr->ExceptionFlowStruct.currentPage){
		// will be considered later !!menuFlowSt->currentPage->exceptionItem = seFlowPtr->ExceptionFlowStruct.currentPage->exceptionItem;
		menuFlow = &(seFlowPtr->ExceptionFlowStruct);
	}
	return menuFlow;

}

/*------------START BUTTON ACTION DETERMINATIONS
 *
 */



/*------------END BUTTON ACTION DETERMINATIONS
 *
 *
 */

/*
 * Function : ChangePage
 *
 */



/********
 * Function: ButtonFuncWithoutModeChange
 */
void ButtonFuncWithoutModeChange(uiUint32_t currentMode, uiUint32_t buttonID)
{
	switch(currentMode){
		case NAVIGATION_MODE:
			CheckExceptionsPageSeqInNav(menuFlowSt, exceptionFlow, buttonID);
			if(UP_BUTTON == buttonID){
				//ChangePage(menuFlow, UP_BUTTON);
			}
			if(DOWN_BUTTON == buttonID){
				//ChangePage(menuFlow, DOWN_BUTTON);
			}
			if(OK_BUTTON == buttonID){
				//ChangePage(menuFlow, OK_BUTTON);
			}
			break;

	};
}
