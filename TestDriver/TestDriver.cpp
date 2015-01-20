// TestDriver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "server_types.h"
#include "SdkClass.h"

using namespace Traintracks;

int _tmain(int argc, _TCHAR* argv[])
{
	SdkClass myclass;

	const string_t cProductKey = L"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
	const string_t cSecretProductKey = L"bbbbbbbb-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
	const string_t cSessionId = L"fff324c6-30a2-11e3-ad80-485d60066bda";
	const string_t cAppVersion = L"1.1.1";
	const string_t cDevice = L"SDKTestTool";
	const string_t cUserId = L"cf23679c-534e-11e3-90c7-14109fe0f6b1";
	const string_t cUserName = L"typicalGamer2000";
	const string_t cTraintracksURL = L"http://localhost:8080";

	const string_t cEventName = L"battle.won";
	const string_t cClientTimestamp = L"2012-03-14T02:33:42.416587-07:00";
	const string_t cDataField = L"{ \"winner\": { \"name\": \"typicalGamer2000\", \"experiencePoints\": 500, \"weapon\": \"Ragnarok\", \"money\": 2350 }, \"loser\": { \"name\": \"typicalLoser1000\", \"experiencePoints\": 200, \"weapon\": \"Buster Sword\", \"money\": 1200 }, \"battleTime\": 52030 }";

	SdkClass::SetProductKey(cProductKey);
	SdkClass::SetSecretProductKey(cSecretProductKey);
	SdkClass::SetSessionId(cSessionId);
	SdkClass::SetAppVersion(cAppVersion);
	SdkClass::SetDevice(cDevice);
	SdkClass::SetUserId(cUserId);
	SdkClass::SetUserName(cUserName);
	SdkClass::SetTraintracksURL(cTraintracksURL);
	SdkClass::SetBlockSize(5);

	for(int idx = 0;idx<50;++idx)
	{
		if(myclass.PostEvent(cEventName, cClientTimestamp, cDataField) < 0)
		{
			// Error
		}
	}

	return 0;
}

