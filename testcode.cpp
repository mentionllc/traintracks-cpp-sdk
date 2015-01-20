	json::value obj;
	obj[L"key1"] = json::value::boolean(false);
	obj[L"key2"] = json::value::number(44);
	obj[L"key3"] = json::value::number(43.6);
	obj[L"key4"] = json::value::string(U("TestString"));

	json::value bundle;
	json::value obj2;
	for(int i=0;i<10;i++)
	{
		json::value vehicle1;
		vehicle1[L"id"] = json::value::string(L"35092_35092_353");
		vehicle1[L"x"] = json::value::number(i);
		vehicle1[L"y"] = json::value::number(50.872139);

		bundle[i] = vehicle1;

	}
	obj[L"data"] = bundle;
	//std::wcout << obj.serialize();

	json::value testObj4 = json::value::parse(U("{\"data\":[{\"id\":\"35092_35092_353\",\"x\":0,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":1,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":2,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":3,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":4,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":5,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":6,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":7,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":8,\"y\":50.872138999999997},{\"id\":\"35092_35092_353\",\"x\":9,\"y\":50.872138999999997}],\"key1\":false,\"key2\":44,\"key3\":43.600000000000001,\"key4\":\"TestString\"}"));
	//std::wcout << testObj4.serialize();

	json::value testObj5 = json::value::parse(U("{\"data\":[{\"build\": \"1.1.1\",\"clientTimestamp\": \"2012-03-14T02:33:42.416587-07:00\"}] }"));
	//std::wcout << testObj5.serialize();

	json::value testObj6 = json::value::parse(U("{ \"data\":[{\"build\": \"1.1.1\",\"clientTimestamp\": \"2012-03-14T02:33:42.416587-07:00\", \"data\": {\"winner\": {\"name\": \"typicalGamer2000\",\"experiencePoints\": \"500\",\"weapon\": \"Ragnarok\",\"money\": 2350},\"loser\": {\"name\": \"typicalLoser1000\",\"experiencePoints\": \"200\",\"weapon\": \"Buster Sword\",\"money\": 1200}} }] }"));
	//std::wcout << testObj6.serialize();

