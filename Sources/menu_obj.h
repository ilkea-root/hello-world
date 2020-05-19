/*
 * menu_obj.h
 *
 *  Created on: 27 Oca 2020
 *      Author: iakgol
 */

#ifndef SOURCES_MENU_OBJ_H_
#define SOURCES_MENU_OBJ_H_

#include "uilibDef.h"





typedef union{
	uiUint32_t UInt;
	struct{
		uiUint32_t eICoefDependence	:1;
//		uiUint32_t eIUnitDependence	:1;
//		uiUint32_t eISign			:1;
		uiUint32_t eISignSym		:1;//iþaret sembol veya dijitle mi gösterilecek
		uiUint32_t eIkmg			:1;
	}Bits;
}editIntegerInfo_t;

typedef union{
	uiUint32_t UInt;
	struct{

		uiUint32_t eLAlignment			:1;
//		uiUint32_t eLUnitDependence		:1;//
//		uiUint32_t eLSign				:1;
		uiUint32_t eLTextSelect			:1;//tek baþýna metin seçimi yapýlabilir
		uiUint32_t eLSymSelect			:1;//tek baþýna ikon, sembol seçimi yapýlabilir.
		uiUint32_t eLTextAndSymSelect	:1;// hem ikon hem metin seçimi yapýlabilir.
	};
}editListInfo_t;

typedef union{
	uiUint32_t UInt;
	struct{
//		uiUint32_t eFUnitDependence		:1;//for special alarm parameters
//		uiUint32_t eFSign				:1;
		uiUint32_t eFSegmentSignSelect	:1;//sign selection in 7-segments 0 or '-'
		uiUint32_t eFCapEndSignSelect	:1;//sign selection in custom lcds
		uiUint32_t eFSaveWithMinus		:1;// multiply with '-' during saving operation
	}Bits;
}editFloatInfo_t;

typedef union{
	uiUint32_t UInt;
	struct{
		uiUint32_t eUnitDependence		:1;//for special alarm parameters
		uiUint32_t eSign				:1;
		uiUint32_t eSegmentSignSelect	:1;//sign selection in 7-segments 0 or '-'
		uiUint32_t eLCDSignSelect		:1;//sign selection in custom lcds
		uiUint32_t eSaveWithMinus		:1;// multiply with '-' during saving operation

	}Bits;
}editGeneralInfo_t;

typedef struct{
	void *eIDataType;
	uiInt32_t	eIMin;
	uiInt32_t	eIMax;
	uiInt32_t eIMinMaxCoef;//multiply min, max values by AT, VT
	uiInt32_t eIDivider;
	editIntegerInfo_t editIntegerInfo;
	uiUint8_t eIDigitNum;
	uiUint8_t eIUnit	;

}editIntegerObject_t;

typedef struct{
	void *eFDataType;
	float eFMin;
	float eFMax;
	float eFMinMaxCoef;
	float eFCoefDependence;//multiply value by ATxVT or xVT
	uiUint8_t eFDigitNum;
	uiUint8_t eFUnit;
	editFloatInfo_t editFloatInfo;

}editFloatObject_t;


typedef struct{
	void *eLDataType;
	uiUint8_t eLMin;
	uiUint8_t eLMax;
	uiUint8_t eLUnit;
	editListInfo_t editListInfo;

}editListObject_t;

typedef union viewInfo_t{
	uiUint32_t UInt;
	struct{
		uiUint32_t viewPage: 1;
		uiUint32_t wizardPage:1;
		uiUint32_t settingViewPage:1;
		uiUint32_t settingRootPage:1;
		uiUint32_t messagePage:1;
		uiUint32_t recursivePage:1;
		uiUint32_t passwordProtectionPage:1;

	}Bits;

}viewInfo_t;


typedef struct viewPage_t{
//	uiUint32_t viewPage;
//	uiUint32_t wizardPage;
//	uiUint32_t settingViewPage;
//	uiUint32_t settingRootPage;
//	uiUint32_t messagePage;
//	uiUint32_t recursivePage;
//	uiUint32_t passwordProtectionPage;
	viewInfo_t viewInfo;
	void (*buttonActionNavUP)(void);//up
	void (*buttonActionNavDOWN)(void);//dwn
	void (*buttonActionNavOK)(void);//ok
	void (*buttonActionNavHOME)(void);//home
} viewPage_t;

void UpButtonActionNav();
void DownButtonActionNav();
void OkButtonActionNav();
void HomeButtonActionNav();

void UpButtonActionWizard();
void DownButtonActionWaizard();
void OkButtonActionWizard();
void HomeButtonActionWizard();

void UpButtonActionSettingRoot();
void DownButtonActioSettingRoot();
void OkButtonActionSettingRoot();



typedef union editInfo_t{
	uiUint32_t UInt;
	struct{
		uiUint32_t valueEditPage: 1;
		uiUint32_t listSelectPage:1;
		uiUint32_t wizardEditPage:1;
		uiUint32_t settingEditPage:1;
		uiUint32_t flowChangePage:1;
		uiUint32_t passwordChangePage:1;
		uiUint32_t passwordEnterPage:1;
		uiUint32_t passwordReEnterPage:1;
	}Bits;

}editInfo_t;

typedef struct editPage_t{
//	uiUint32_t valueEditPage;
//	uiUint32_t listSelectPage;
//	uiUint32_t wizardEditPage;//gerekli olup olmamasýna bakýlacak
//	uiUint32_t settingEditPage;
//	uiUint32_t flowChangePage; // bu sayfada listeden seçilen indekse göre hangi sayfaya gideceði belirleniyor.
//	uiUint32_t passwordChangePage;
//	uiUint32_t passwordEnterPage;
//	uiUint32_t passwordReEnterPage;
	editInfo_t editInfo;
	void (*buttonActionData)(uiUint32_t data);//
	void (*buttonActionList)(uiUint32_t data);//

} editPage_t;

void UpButtonActionList(uiUint32_t list);
void DownButtonActionList(uiUint32_t list);
void HomeButtonActionList(uiUint32_t list);
void OkButtonActionList(uiUint32_t list);
void UpButtonActionData(uiUint32_t data);
void DownButtonActionData(uiUint32_t data);
void HomeButtonActionData(uiUint32_t data);
void OkButtonActionData(uiUint32_t data);

//typedef struct PageFeature_t{
//	void *pageTypeInfo;
//	void *pageType;
//}PageFeature_t;

typedef struct page__{

//	pPageData_t pPageData;
//	viewPage_t viewPage;
//	editPage_t editPage;

	void *pageFeature;
//	uiUint32_t showIcons; // think about later
//	uiUint32_t exceptionItem;
//	uiUint32_t exceptionItemList;
	struct page__ *pBackPage;
	struct page__ *(*pSubPages)[];


} page_t;

typedef page_t *subPages_t[];
typedef subPages_t *pSubPages_t;

#endif /* SOURCES_MENU_OBJ_H_ */

