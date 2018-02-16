#pragma once

#include "twain.h"
#include <string>


std::string StringFromDg(const TW_UINT32  _DG)
{
	switch (_DG)
	{
	case DG_CONTROL:
		return "DG_CONTROL";
		//break;

	case DG_IMAGE:
		return "DG_IMAGE";

	case DG_AUDIO:
		return "DG_AUDIO";

	default:
		return "DONTKNOW";
	}
}



/*
* Convert a DAT_ data argument type numerical value to a string...
*/
std::string StringFromDat(const TW_UINT16 _DAT)
{
	switch (_DAT)
	{

	case DAT_NULL:
		return "DAT_NULL";

	case DAT_CUSTOMBASE:
		return "DAT_CUSTOMBASE";

	case DAT_CAPABILITY:
		return "DAT_CAPABILITY";

	case DAT_EVENT:
		return "DAT_EVENT";

	case DAT_IDENTITY:
		return "DAT_IDENTITY";

	case DAT_PARENT:
		return "DAT_PARENT";

	case DAT_PENDINGXFERS:
		return "DAT_PENDINGXFERS";

	case DAT_SETUPMEMXFER:
		return "DAT_SETUPMEMXFER";

	case DAT_SETUPFILEXFER:
		return "DAT_SETUPFILEXFER";

	case DAT_STATUS:
		return "DAT_STATUS";

	case DAT_USERINTERFACE:
		return "DAT_USERINTERFACE";

	case DAT_XFERGROUP:
		return "DAT_XFERGROUP";

	case DAT_TWUNKIDENTITY:
		return "DAT_TWUNKIDENTITY";

	case DAT_CUSTOMDSDATA:
		return "DAT_CUSTOMDSDATA";

	case DAT_DEVICEEVENT:
		return "DAT_DEVICEEVENT";

	case DAT_FILESYSTEM:
		return "DAT_FILESYSTEM";

	case DAT_PASSTHRU:
		return "DAT_PASSTHRU";

	case DAT_CALLBACK:
		return "DAT_CALLBACK";

	case DAT_STATUSUTF8:
		return "DAT_STATUSUTF8";

	case DAT_IMAGEINFO:
		return "DAT_IMAGEINFO";

	case DAT_IMAGELAYOUT:
		return "DAT_IMAGELAYOUT";

	case DAT_IMAGEMEMXFER:
		return "DAT_IMAGEMEMXFER";

	case DAT_IMAGENATIVEXFER:
		return "DAT_IMAGENATIVEXFER";

	case DAT_IMAGEFILEXFER:
		return "DAT_IMAGEFILEXFER";

	case DAT_CIECOLOR:
		return "DAT_CIECOLOR";

	case DAT_GRAYRESPONSE:
		return "DAT_GRAYRESPONSE";

	case DAT_RGBRESPONSE:
		return "DAT_RGBRESPONSE";

	case DAT_JPEGCOMPRESSION:
		return "DAT_JPEGCOMPRESSION";

	case DAT_PALETTE8:
		return "DAT_PALETTE8";

	case DAT_EXTIMAGEINFO:
		return "DAT_EXTIMAGEINFO";

	case DAT_AUDIOFILEXFER:
		return "DAT_AUDIOFILEXFER";

	case DAT_AUDIOINFO:
		return "DAT_AUDIOINFO";

	case DAT_AUDIONATIVEXFER:
		return "DAT_AUDIONATIVEXFER";

	case DAT_ICCPROFILE:
		return "DAT_ICCPROFILE";

	case DAT_IMAGEMEMFILEXFER:
		return "DAT_IMAGEMEMFILEXFER";

	case DAT_ENTRYPOINT:
		return "DAT_ENTRYPOINT";
	default:
		return "DONTKNOW";
	}
}



/*
* Convert a MSG_ message numerical value to a string...
*/
std::string StringFromMsg(const TW_UINT16 _MSG)
{
	switch (_MSG)
	{
	case MSG_NULL:
		return "MSG_NULL";

	case MSG_CUSTOMBASE:
		return "MSG_CUSTOMBASE";

	case MSG_GET:
		return "MSG_GET";

	case MSG_GETCURRENT:
		return "MSG_GETCURRENT";

	case MSG_GETDEFAULT:
		return "MSG_GETDEFAULT";

	case MSG_GETFIRST:
		return "MSG_GETFIRST";

	case MSG_GETNEXT:
		return "MSG_GETNEXT";

	case MSG_SET:
		return "MSG_SET";

	case MSG_RESET:
		return "MSG_RESET";

	case MSG_QUERYSUPPORT:
		return "MSG_QUERYSUPPORT";

	case MSG_GETHELP:
		return "MSG_GETHELP";

	case MSG_GETLABEL:
		return "MSG_GETLABEL";

	case MSG_GETLABELENUM:
		return "MSG_GETLABELENUM";

	case MSG_XFERREADY:
		return "MSG_XFERREADY";

	case MSG_CLOSEDSREQ:
		return "MSG_CLOSEDSREQ";

	case MSG_CLOSEDSOK:
		return "MSG_CLOSEDSOK";

	case MSG_DEVICEEVENT:
		return "MSG_DEVICEEVENT";
	case MSG_CHECKSTATUS:
		return "MSG_CHECKSTATUS";
	case MSG_OPENDSM:
		return "MSG_OPENDSM";

	case MSG_CLOSEDSM:
		return "MSG_CLOSEDSM";

	case MSG_OPENDS:
		return "MSG_OPENDS";

	case MSG_CLOSEDS:
		return "MSG_CLOSEDS";

	case MSG_USERSELECT:
		return "MSG_USERSELECT";

	case MSG_DISABLEDS:
		return "MSG_DISABLEDS";

	case MSG_ENABLEDS:
		return "MSG_ENABLEDS";

	case MSG_ENABLEDSUIONLY:
		return "MSG_ENABLEDSUIONLY";

	case MSG_PROCESSEVENT:
		return "MSG_PROCESSEVENT";

	case MSG_ENDXFER:
		return "MSG_ENDXFER";

	case MSG_CHANGEDIRECTORY:
		return "MSG_CHANGEDIRECTORY";

	case MSG_CREATEDIRECTORY:
		return "MSG_CREATEDIRECTORY";

	case MSG_DELETE:
		return "MSG_DELETE";

	case MSG_FORMATMEDIA:
		return "MSG_FORMATMEDIA";

	case MSG_GETCLOSE:
		return "MSG_GETCLOSE";

	case MSG_GETFIRSTFILE:
		return "MSG_GETFIRSTFILE";

	case MSG_GETINFO:
		return "MSG_GETINFO";

	case MSG_GETNEXTFILE:
		return "MSG_GETNEXTFILE";

	case MSG_RENAME:
		return "MSG_RENAME";

	case MSG_PASSTHRU:
		return "MSG_PASSTHRU";

	case MSG_REGISTER_CALLBACK:
		return "MSG_REGISTER_CALLBACK";

	case MSG_RESETALL:
		return "MSG_RESETALL";
	default:
		return "DONTKNOW";
		

	}
}