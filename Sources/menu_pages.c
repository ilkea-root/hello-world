/*
 * menu_pages.c
 *
 *  Created on: 15 Nis 2020
 *      Author: iakgol
 */

#include "menu_obj.h"
#include "menu_pages.h"


static viewPage_t viewPage ={
		0, UpButtonActionNav,DownButtonActionNav,OkButtonActionNav,HomeButtonActionNav

};

void *pageFeatureView = (viewPage_t *)&viewPage;
page_t measure[]={&vln,};
page_t vln={(viewPage_t*)pageFeatureView,&root};



viewPage_t* GetPageFeatureView(page_t* page)
{
	return (viewPage_t*) page->pageFeature;
}

void SetPageFeatureView(page_t* page)
{
	viewPage->viewInfo.Bits.viewPage = 1;
	viewPage->buttonActionNavUP = UpButtonActionNav;
	viewPage->buttonActionNavDOWN = DownButtonActionNav;
	viewPage->buttonActionNavOK = OkButtonActionNav;
	viewPage->buttonActionNavHOME = HomeButtonActionNav;
	page->pageFeature = (viewPage_t *)&viewPage;

}

void SetPageFeatureWizardView(page_t* page)
{
	viewPage->viewInfo.Bits.viewPage = 1;
	viewPage.viewInfo.Bits.wizardPage = 1;
	viewPage.buttonActionNavUP = UpButtonActionWizard;
	viewPage.buttonActionNavDOWN = DownButtonActionWizard;
	viewPage.buttonActionNavOK = OkButtonActionWizard;
	viewPage.buttonActionNavHOME = HomeButtonActionWizard;
	page->pageFeature = (viewPage_t *)&viewPage;

}
void SetPageFeatureSettingView(page_t* page)
{
	viewPage.viewInfo.Bits.settingViewPage = 1;
	viewPage->buttonActionNavUP = UpButtonActionNav;
	viewPage->buttonActionNavDOWN = DownButtonActionNav;
	viewPage->buttonActionNavOK = OkButtonActionNav;
	viewPage->buttonActionNavHOME = HomeButtonActionNav;
	page->pageFeature = (viewPage_t *)&viewPage;

}

void SetPageFeatureSettingWizardView(page_t* page)
{
	viewPage.viewInfo.Bits.settingViewPage = 1;
	viewPage.viewInfo.Bits.wizardPage = 1;
	viewPage->buttonActionNavUP = UpButtonActionNav;
	viewPage->buttonActionNavDOWN = DownButtonActionNav;
	viewPage->buttonActionNavOK = OkButtonActionNav;
	viewPage->buttonActionNavHOME = HomeButtonActionNav;
	page->pageFeature = (viewPage_t *)&viewPage;

}

void SetPageFeatureSettingRootPage(page_t* page)
{
	viewPage.viewInfo.Bits.settingRootPage = 1;
	viewPage->buttonActionNavUP = UpButtonActionSettingRoot;
	viewPage->buttonActionNavDOWN = DownButtonActionSettingRoot;
	viewPage->buttonActionNavOK = OkButtonActionSettingRoot;
	viewPage->buttonActionNavHOME = HomeButtonActionSettingRoot;
	page->pageFeature = (viewPage_t *)&viewPage;
}



