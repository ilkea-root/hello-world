/*
 * menu_control.h
 *
 *  Created on: 25 Mar 2020
 *      Author: iakgol
 *      Condition definitions that has effect on menu flow change
 */

#ifndef SOURCES_MENU_CONTROL_H_
#define SOURCES_MENU_CONTROL_H_

#include "uilibDef.h"
#include "menu_obj.h"


//Global Mode variable definitions
uiUint32_t currentMode, newMode;
//Timeout declarations
#define ENTER_AUTO_SCROLL_TIMEOUT_INDEX			0
#define ENTER_SMILEY_TIMEOUT_INDEX				1
#define EXIT_SETTINGS_TIMEOUT_INDEX				2
#define CHANGE_AUTO_SCROLL_PAGE_TIMEOUT_INDEX	3
#define NUM_OF_TIMEOUTS							4

#define PAGE_SEQ_IN_NAV							0
#define PAGE_SEQ_IN_SETTING						1
#define PAGE_SEQ_IN_COMMISSION					2
#define PAGE_ITEM_IN_NAV						3
#define PAGE_ITEM_IN_SETTING					4
#define PAGE_ITEM_IN_COMMISSION					5
#define PAGE_ITEM_LIST							6
#define NUM_OF_EXCEPTIONS						7

/********
 * �imdilik alarm durumu tan�mlamas�
 */
enum{
	NO_ALARM,
	ALARM_RECOVERY,
	ALARM,
	LAST_ALARM
}alarmStatus;

/******
 * �imdilik buton definitions
 */
enum{
	NO_BUTTON,
	ANY_BUTTON,
	HOME_BUTTON,
	UP_BUTTON,
	DOWN_BUTTON,
	OK_BUTTON,
	TUTORIAL_BUTTON

}buttonStatus;

enum {
	NAVIGATION_MODE,
	SETTINGS_MODE,
	SMILEY_MODE,
	AUTO_SCROLL_MODE,
	NOTIFICATION_MODE,
	TUTORIAL_MODE,
	FIRST_TIME_INSTALLATION_MODE
}mode;

struct setupChange_t{
	uiUint32_t backlightSetting; 					//g�ncel backlight ayar�
	uiUint32_t smileySetting; 					//g�ncel smiley ayar�
	uiUint32_t autoscrollSetting; 				//g�ncel otoscroll ayar�
};

struct setupExceptions_t{
	uiUint32_t password;
	uiUint32_t alarm;
	uiUint32_t foo;
};

struct deviceTypeExceptions_t{
	uiUint32_t Modbus;
	uiUint32_t Harmonics;
};

//////////////////////////move to globals-related to project
struct setupGlobal_t{
	uiUint32_t backlight;
	uiUint32_t smiley;
	uiUint32_t autoscroll;
	uiUint32_t password;
	uiUint32_t alarm;
	uiUint32_t foo;
};

extern struct setupGlobal_t *setupG;
struct deviceType_t{
	uiUint32_t MODBUS;
	uiUint32_t HARMONICS;
};
extern struct deviceType_t *deviceType;

extern struct setupChange_t *setupChange;
extern struct setupExceptions_t *setupExceptions;
extern struct deviceTypeExceptions_t *deviceTypeExceptions;

//typedef uiUint32_t (*timeoutFuncPtr)(const struct setupChange_t * scPtr, uiUint32_t currentMode);

struct menuTimeout_t{
	uiUint32_t timeoutStatus;						//timeout'un enable olup olmad���
	uiUint32_t timeoutFlag; 						//timeout'un bitwise flagi
	uiUint32_t timeoutCount; 	//timeout sayac�n�n mevcut de�eri
	uiUint32_t timoutLimit; 		//timeout sayac�n�n limit de�eri
	//timeoutFuncPtr ModeTimeouts;				//timeout de�eri doldu�unda �a��r�lacak olan fonksiyon
	uiUint32_t (*timeoutFuncPtr)(const struct setupChange_t * scPtr, uiUint32_t currentMode);
};

uiUint32_t ChangeModeAutoScrollTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode);
uiUint32_t ChangeModeSmileyTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode);
uiUint32_t ChangeModeSettingsTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode);
uiUint32_t NoModeChangeAutoScrollPageTimeout(const struct setupChange_t * scPtr, uiUint32_t currentMode);

//timeoutFuncPtr ModeTimeouts[NUM_OF_TIMEOUTS]= {
//		ChangeModeAutoScrollTimeout,
//		ChangeModeSmileyTimeout,
//		ChangeModeSettingsTimeout,
//		NoModeChangeAutoScrollPageTimeout
//
//};






struct menuFlowStruct_t{

	page_t *currentPage; // page struct tan�mlamalar� yap�lacak!!
	page_t *backupPage;
	page_t *nextPage;
	page_t *prevPage;

}menuFlowStruct;

extern struct menuFlowStruct_t *menuFlowSt;


struct ExceptionFlow_t{

	uiUint32_t deviceTypeExceptionFlow;
	uiUint32_t settingChangeExceptionFlow;
	uiUint32_t deviceTypeExceptionPageItem;
	uiUint32_t settingChangeExceptionPageItem;
	uiUint32_t deviceTypeExceptionPageItemList;
	uiUint32_t settingChangeExceptionPageItemList;
	uiUint32_t pageItemList;
	uiUint32_t exceptionTrigger;
	struct menuFlowStruct_t ExceptionFlowStruct;
};


void (*ExceptionFuncPtr)(struct menuFlowStruct_t* mfPtr, uiUint32_t buttonID);

struct menuFlowStruct_t * ChangePageSeq(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID);

void ChangePageItemInNav();
void ChangePageItemInSetting();
void ChangePageItemInCommissioning();
void ChangePageItemList();

typedef struct buttonAction_t{
	uiUint32_t actionTypeNavigation;
	uiUint32_t actionTypeData;
	uiUint32_t specialActionTypeNavigation;
	uiUint32_t specialActionTypeData;
	void (*buttonUpPtr)();
	void (*buttonDownPtr)();
	void (*buttonOkPtr)();
	void (*buttonHomePtr)();

}buttonAction_t;



void EnableTimeout(uiUint32_t timeoutIndex);  	//timeout ba�latma
void DisableTimeout(uiUint32_t timeoutIndex); 	//timeout durdurma
void ResetTimeout(uiUint32_t timeoutIndex);		//timeout sayac�n� ba�tan ba�latma
void ResetTimeouts(void);//t�m timeoutlar� s�f�rlama
int GetTimeoutStatus(uiUint32_t timeoutIndex);	//timeout durumu (enable m� disable m�?) return: timeoutStatus
void ChangeTimeoutCondition(const struct setupChange_t * scPtr, uiUint32_t mode);	//mevcut moda g�re ba�lat�lmas� ya da durdurulmas� gereken timeout var m�? Varsa ba�lat ya da durdur.
uiInt32_t CheckTimeoutOverflow(uiUint32_t timeoutIndex);//timeout sayac�n� bir artt�r�p limite ula�m��sa 1, de�ilse 0 d�ner
void DisableAllTimeouts();
uiUint32_t ChangeMenuMode(uiUint32_t eventID);
void EnterSmileyMode();
void EnterAutoScrollMode();
uiUint32_t CheckModeChangeWithCommunication(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus); // haberle�me ayar�na ba�l� mod de�i�ikli�i varsa bu fonk �a��r�l�r. mod de�i�ikli�i sonucu timeout de�i�ikli�i varsa
//ChangeTimeoutCondition �a�r�l�r
uiUint32_t CheckModeChangeWithTimeouts(const struct setupChange_t* scPtr, uiUint32_t currentMode);

void ChangeTimeoutWithCommunication(const struct setupChange_t * scPtr, uiUint32_t currentMode);// mod ge�i�i olmad��� durumda, haberle�me ayar� sonras�nda timeout g�ncellenmesi
uiUint32_t CheckModeChangeWithAlarm(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus);
uiUint32_t CheckModeChangeWithButton(const struct setupChange_t * scPtr, uiUint32_t currentMode, uiUint32_t alarmStatus,  uiUint32_t buttonID);
void ChangeMode(void);// return new mode

void FillSetupChangeStruct();

void ButtonFuncWithoutModeChange(uiUint32_t currentMode, uiUint32_t buttonID);
void SetExceptionsFlow(struct menuFlowStruct_t * menuFlowSt, struct setupExceptions_t *sePtr, struct deviceTypeExceptions_t *dePtr, uiUint32_t currentMode, uiUint32_t buttonID);
struct menuFlowStruct_t * CheckExceptionsPageSeqInNav(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr, uiUint32_t buttonID);
struct menuFlowStruct_t * CheckExceptionsPageSeqInSetting(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID);
struct menuFlowStruct_t * CheckExceptionsPageSeqInCommissioning(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr,  uiUint32_t buttonID);
struct menuFlowStruct_t * ChangePageItem(struct menuFlowStruct_t * menuFlowSt, struct ExceptionFlow_t *seFlowPtr);
#endif /* SOURCES_MENU_CONTROL_H_ */
