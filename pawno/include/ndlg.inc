/*
 *  NAMED DIALOGS System
 *
 *  Version 1.2
 *
 *  By Gamer_Z
 *
 */
#if defined _NAMED_DIALOGS_INCLUDED_
	#endinput
#else
	#define _NAMED_DIALOGS_INCLUDED_
#endif

#define NDLG_INCLUDE_VERSION (12)
//#define NDLG_NO_DISABLE_CALLBACK// When defined, Use still the old callback
//#define NDLG_USE_YSI_HOOK//Use YSI hook instead of the default one

#if !defined _cstl_inc
	#error You need to have the CSTL Data container plugin include\
	to use this include (NameDialogs)\
	Please ensure you have included this include BELOW\
	your CSTL include
#endif

#if !defined isnull
	#define isnull(%1) \
				((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

new __PRIV__REGISTERED___DLGS_FS__ = 0;
new __PRIV__REGISTEREDDialogName__[32];
new __PRIV__REDIALOGID___DLGS_FS__ = -1;

//native fnDialog(DialogName)
#define fnDialog(%1) \
	forward ND%1(playerid,response,listitem,inputtext[]);\
	public ND%1(playerid,response,listitem,inputtext[])

//native nDialog(%1,%2,%3,%4)
#define nDialog:%0(%1,%2,%3,%4) \
	forward ND%0(%1,%2,%3,%4);\
	public ND%0(%1,%2,%3,%4)

//native RegisterDialog(dialogname[])
stock RegisterDialog(dialogname[])
{
    ++__PRIV__REDIALOGID___DLGS_FS__;//Add one to variable
    if(__PRIV__REDIALOGID___DLGS_FS__ > 32767)return 0;//ERROR: MAX AMOUNT OF DIALOGS HAS BEEN REACHED, SA-MP USES ONLY 2 BYTES INSTEAD OF 4 BYTES - THAT's FRIKING!
	vector_push_back(__PRIV__REGISTERED___DLGS_FS__, __PRIV__REDIALOGID___DLGS_FS__);//Push the vector to add the variable
	return vector_set_arr(__PRIV__REGISTERED___DLGS_FS__, __PRIV__REDIALOGID___DLGS_FS__, dialogname);//Add the entry name to the container element
}

//native ShowPlayerDialogName(playerid,dialogname[],style, caption[], info[], button1[], button2[])//MAX DIALOG NAME IS 30 CHARACTERS, MAX 32768 UNIQUE NAMES/DIALOGS
stock ShowPlayerDialogName(playerid,dialogname[],style, caption[], info[], button1[], button2[])//MAX DIALOG NAME IS 30 CHARACTERS, MAX 32768 UNIQUE NAMES/DIALOGS
{
	new dialogid = vector_find_arr(__PRIV__REGISTERED___DLGS_FS__,dialogname);//Try to find if the Dialog Name is registered
	if(dialogid == (-1))//if not then register it for us, instead of doing in OnGameModeInit manual registers this does the easy job
	{
	    RegisterDialog(dialogname);//Register it
	    dialogid = vector_find_arr(__PRIV__REGISTERED___DLGS_FS__,dialogname);//Find it again to get the dialog id, which is used to get the Dialog Name later
	}
	return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);//Show the dialog for the player and await response
}

#if defined NDLG_USE_YSI_HOOK
	#if defined NDLG_NO_DISABLE_CALLBACK
		Hook:NameDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])//Our callback - response
		{
			vector_get_arr(__PRIV__REGISTERED___DLGS_FS__, dialogid, __PRIV__REGISTEREDDialogName__, 30);//get the name of dialogid
			format(__PRIV__REGISTEREDDialogName__,32,"ND%s",__PRIV__REGISTEREDDialogName__);//add ND to the beginning
			if(isnull(inputtext))format(inputtext,2,"\1");//avoid server crash on NULL string in CallLocalFunction
            if(funcidx(__PRIV__REGISTERED___DLGS_FS__) != -1)
            {
				CallLocalFunction(__PRIV__REGISTEREDDialogName__,"iiis",playerid,response,listitem,inputtext);//Execute our final callback response
			}
		}
	#else
		Hook:NameDialog_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])//Our callback - response
		{
			vector_get_arr(__PRIV__REGISTERED___DLGS_FS__, dialogid, __PRIV__REGISTEREDDialogName__, 30);//get the name of dialogid
			format(__PRIV__REGISTEREDDialogName__,32,"ND%s",__PRIV__REGISTEREDDialogName__);//add ND to the beginning
			if(isnull(inputtext))format(inputtext,2,"\1");//avoid server crash on NULL string in CallLocalFunction
			CallLocalFunction(__PRIV__REGISTEREDDialogName__,"iiis",playerid,response,listitem,inputtext);//Execute our final callback response
            return -1;
		}
	#endif
#else
	#if defined NDLG_NO_DISABLE_CALLBACK
		public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])//Our callback - response
		{
			vector_get_arr(__PRIV__REGISTERED___DLGS_FS__, dialogid, __PRIV__REGISTEREDDialogName__, 30);//get the name of dialogid
			format(__PRIV__REGISTEREDDialogName__,32,"ND%s",__PRIV__REGISTEREDDialogName__);//add ND to the beginning
			if(isnull(inputtext))format(inputtext,2,"\1");//avoid server crash on NULL string in CallLocalFunction
            if(funcidx(__PRIV__REGISTERED___DLGS_FS__) != -1)
            {
				return CallLocalFunction(__PRIV__REGISTEREDDialogName__,"iiis",playerid,response,listitem,inputtext);//Execute our final callback response
			}
			return CallLocalFunction("NDLG_OnDialogResponse","iiiis",playerid,dialogid,response,listitem,inputtext);
		}
	#else
		public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])//Our callback - response
		{
			vector_get_arr(__PRIV__REGISTERED___DLGS_FS__, dialogid, __PRIV__REGISTEREDDialogName__, 30);//get the name of dialogid
			format(__PRIV__REGISTEREDDialogName__,32,"ND%s",__PRIV__REGISTEREDDialogName__);//add ND to the beginning
			if(isnull(inputtext))format(inputtext,2,"\1");//avoid server crash on NULL string in CallLocalFunction
			return CallLocalFunction(__PRIV__REGISTEREDDialogName__,"iiis",playerid,response,listitem,inputtext);//Execute our final callback response
		}
	#endif
	#if defined _ALS_OnDialogResponse
		#undef OnDialogResponse
	#else
		#define _ALS_OnDialogResponse
	#endif
	#define OnDialogResponse NDLG_OnDialogResponse
    forward FDLG_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
    
 #include <a_http>

/* below is the plugin callback support code, you really shouldn't touch this */

#if defined FILTERSCRIPT
public OnFilterScriptInit()
{
	privGPB_AddUpdatecheck("NDLG",1,NDLG_INCLUDE_VERSION);
	return CallLocalFunction("call_ndlg_OnFilterScriptInit","");
}

#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit call_ndlg_OnFilterScriptInit
forward call_ndlg_OnFilterScriptInit();

#else
public OnGameModeInit()
{
    privGPB_AddUpdatecheck("NDLG",1,NDLG_INCLUDE_VERSION);
	return CallLocalFunction("call_ndlg_OnGameModeInit","");
}

#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit call_ndlg_OnGameModeInit
forward call_ndlg_OnGameModeInit();

#endif
    #include <GPBMAIN>
#endif

