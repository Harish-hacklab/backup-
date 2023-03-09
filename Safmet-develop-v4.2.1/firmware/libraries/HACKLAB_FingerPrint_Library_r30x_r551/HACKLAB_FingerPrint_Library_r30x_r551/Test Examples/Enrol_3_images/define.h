void setupFiles()
{
#ifdef SetupPrint
  Software.println(F("Setup Files"));
#endif
  if (!SPIFFS.begin(true))
  {

#ifdef SetupPrint
    Software.println("An Error has occurred while mounting SPIFFS");
#endif
    return;
  }
  timerWrite(timer, 0);
#ifdef SetupPrint
  Software.println("SPIFFS");
#endif
  readFile(SPIFFS, "/id18.txt");
  timerWrite(timer, 0);
  if (streeng.length() == 0)
  {
    timerWrite(timer, 0);
    String ids = "1,2,3,4,5,:";
    //String ids = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269,270,271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397,398,399,400,401,402,403,404,405,406,407,408,409,410,411,412,413,414,415,416,417,418,419,420,421,422,423,424,425,426,427,428,429,430,431,432,433,434,435,436,437,438,439,440,441,442,443,444,445,446,447,448,449,450,451,452,453,454,455,456,457,458,459,460,461,462,463,464,465,466,467,468,469,470,471,472,473,474,475,476,477,478,479,480,481,482,483,484,485,486,487,488,489,490,491,492,493,494,495,496,497,498,499,500,501,502,503,504,505,506,507,508,509,510,511,512,513,514,515,516,517,518,519,520,521,522,523,524,525,526,527,528,529,530,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643,644,645,646,647,648,649,650,651,652,653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688,689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788,789,790,791,792,793,794,795,796,797,798,799,800,801,802,803,804,805,806,807,808,809,810,811,812,813,814,815,816,817,818,819,820,821,822,823,824,825,826,827,828,829,830,831,832,833,834,835,836,837,838,839,840,841,842,843,844,845,846,847,848,849,850,851,852,853,854,855,856,857,858,859,860,861,862,863,864,865,866,867,868,869,870,871,872,873,874,875,876,877,878,879,880,881,882,883,884,885,886,887,888,889,890,891,892,893,894,895,896,897,898,899,900,901,902,903,904,905,906,907,908,909,910,911,912,913,914,915,916,917,918,919,920,921,922,923,924,925,926,927,928,929,930,931,932,933,934,935,936,937,938,939,940,941,942,943,944,945,946,947,948,949,950,951,952,953,954,955,956,957,958,959,960,961,962,963,964,965,966,967,968,969,970,971,972,973,974,975,976,977,978,979,980,981,982,983,984,985,986,987,988,989,990,991,992,993,994,995,996,997,998,999,:";
    writeFile(SPIFFS, "/id18.txt", ids);
    readFile(SPIFFS, "/id18.txt");
#ifdef SetupPrint
    Software.print("streeng new:"); Software.println(streeng);
#endif
  }
  timerWrite(timer, 0);
}

void IRAM_ATTR resetModule()
{
  Software.println("Reboot coz of wdt");
  ets_printf("rebootin due to power switch \n");
  esp_restart();
}

void updateCheck()
{
  const char* update_ssid = "HACKLAB";
  const char* update_password = "UPDATE12";
  WiFi.begin(update_ssid, update_password);
  bool connect_flag = false;
  uint32_t prev_millis = 0;
  while ((WiFi.status() != WL_CONNECTED) && (millis() - prev_millis < 4000)) {
    Software.print(".");
    //prev_millis = millis();
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    timer = timerBegin(1, 80, true);                  //timer 1, div 80
    timerAttachInterrupt(timer, &resetModule, true);  //attach callback
    timerAlarmWrite(timer, 100000 * 1000, false); //set time in us
    timerAlarmEnable(timer);                          //enable interrupt

    connect_flag = true;
#ifdef SetupPrint
    Software.print("Connected. Upload at ");
    Software.println(WiFi.localIP());
#endif
  }
  else
  {
#ifdef SetupPrint
    Software.println("timer_expired");
#endif
  }
  while (connect_flag)
  {
    //digitalWrite(4,HIGH);
    timerWrite(timer, 0);
    ArduinoOTA.begin();
    ArduinoOTA.handle();
  }
}

void readFile(fs::FS &fs, const char * path)
{
#ifdef ENROLprint
  Software.printf("Reading file: %s\r\n", path);
#endif

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
#ifdef ENROLprint
    Software.println("- failed to open file for reading");
#endif
  }

#ifdef ENROLprint
  Software.println("- read from file:");
#endif
  String abc;
  while (file.available())
  {
    char inChar = file.read();
    abc += (char)inChar;
  }
  streeng = abc;
}

void writeFile(fs::FS &fs, const char * path, String message)
{
#ifdef ENROLprint
  Software.printf("Writing file: %s\r\n", path);
#endif
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
#ifdef ENROLprint
    Software.println("- failed to open file for writing");
#endif
  }
  if (file.print(message))
  {
#ifdef ENROLprint
    Software.println("- file written");
#endif
  }
  else
  {
#ifdef ENROLprint
    Software.println("- frite failed");
#endif
  }
}

void handleList()
{
  File listFile = SPIFFS.open("/config7.json", "r");
  if (listFile)
  {
#ifdef ENROLprint
    Software.println("opened");
#endif
  }
  size_t size = listFile.size();
#ifdef ENROLprint
  Software.print("Size is ");
  Software.println(size);
#endif
  if (size < 3)
  {
#ifdef WebPage
    //no one enrolled....enrol now
    String content = "<html><body><H2><center>No One Enrolled</H2></body></html>";
    server.send(200, "text/html", content);
#endif

#ifdef Bluetooth
    String content = "No One Enrolled";
    ESP_BT.println(content);
#endif
  }
  if (size > 1024)
  {
#ifdef ENROLprint
    Software.println("Config file size is too large");
#endif
  }
  std::unique_ptr<char[]> buf(new char[size]);
  listFile.readBytes(buf.get(), size);
#ifdef ENROLprint
  Software.println("buf");
  for (int i = 0; i < size; i++)
  {
    Software.print(buf[i]);
  }
  Software.println();
#endif
  JsonObject& jsonRead = jsonBuffer.parseObject(buf.get());
  if (!jsonRead.success())
  {
#ifdef ENROLprint
    Software.println("Failed to parse JSON file");
#endif
  }
  else
  {
#ifdef ENROLprint
    Software.println("Listing");
    jsonRead.prettyPrintTo(Software);
#endif
  }

#ifdef WebPage
  String content = "<html><body><H2><center>List of Employees Enrolled</H2><br><br>";
  for (auto kv : jsonRead)
  {
#ifdef ENROLprint
    Software.println(kv.key);
#endif
    content += "<H3><center>" + String(kv.key) + "</H3>";
    //Software.println(kv.value.as<char*>());
  }
  server.send(200, "text/html", content);
#endif

#ifdef Bluetooth
  String content = "";  //"List of Employees Enrolled";
  for (auto kv : jsonRead)
  {
#ifdef ENROLprint
    Software.println(kv.key);
#endif
    content += "," + String(kv.key);
  }
  content += ":";
  ESP_BT.println(content);
#endif
}

bool is_authentified()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
#ifdef ENROLprint
  Software.println("Enter is_authentified");
#endif
  if (server.hasHeader("Cookie"))
  {
    String cookie = server.header("Cookie");
#ifdef ENROLprint
    Software.println(); Software.print("Found cookie: ");
    Software.println(cookie);
#endif
    if (cookie.indexOf("ESPSESSIONID=1") != -1)
    {
#ifdef ENROLprint
      Software.println("Authentification Successful");
#endif
      return true;
    }
  }
#ifdef ENROLprint
  Software.println("Authentification Failed");
#endif
  return false;
}


void handleLogin()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
  String msg;
  if (server.hasHeader("Cookie"))
  {
    String cookie = server.header("Cookie");
#ifdef ENROLprint
    Software.println(); Software.print("Found cookie: ");
    Software.println(cookie);
#endif
  }
  if (server.hasArg("DISCONNECT"))
  {
#ifdef DEBUG
    Software.println("Disconnection");
#endif
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD"))
  {
    if (server.arg("USERNAME") == "hacklab" &&  server.arg("PASSWORD") == "HACK@LAB")
    {
      server.sendHeader("Location", "/option");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
#ifdef ENROLprint
      Software.println("Log in Successful");
#endif
      return;
    }
    msg = "Wrong username/password! try again.";
#ifdef ENROLprint
    Software.println("Log in Failed");
#endif
  }
  String content = "<html><body><center><form action='/login' method='POST'>Log In<br><br>";
  content += "    Username:<input type='text' name='USERNAME' placeholder='user name' required><br><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password' required><br><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>";
  //content += "You also can go <a href='/inline'>here</a></body></html>";
  server.send(200, "text/html", content);
}

void handleOption()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
  String content = "<html><body><center><H2><a href='/user'>Enrol</a><br><br>";
  content += "<a href='/list'>List of Operators</a><br><br>";
  content += "<a href='/delete'>Delete</a></H2></body></html>";
  server.send(200, "text/html", content);
}

void handleUser()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
  String msg;
  String k;
  if (server.hasHeader("Cookie"))
  {
    String cookie = server.header("Cookie");
#ifdef DEBUG
    Software.println(); Software.print("Found cookie: ");
    Software.println(cookie);
#endif
  }

  if (server.hasArg("EMPLOYEE") && server.hasArg("EMPLOYEE_ID") && server.hasArg("FINGER") && server.hasArg("ROLE"))
  {
    employeeName = String(server.arg("EMPLOYEE"));
    String employeeID = String(server.arg("EMPLOYEE_ID"));
    String employeeFinger = String(server.arg("FINGER"));
    String employeeRole = String(server.arg("ROLE"));
    Software.print("employeeID: "); Software.println(employeeID);

    File configFile = SPIFFS.open("/config7.json", "r");
    if (configFile)
    {
#ifdef ENROLprint
      Software.println("opened to enrol");
#endif
    }
    size_t size = configFile.size();
#ifdef DEBUG
    Software.println("Size is");
    Software.println(size);
#endif

    if (size == 0)
    {
      JsonObject& jsonNew = jsonBuffer.createObject();
      JsonArray& levels = jsonNew.createNestedArray(employeeName);
      levels.add(employeeID);
      levels.add(id);
      levels.add(employeeFinger);
      levels.add(employeeRole);
      configFile = SPIFFS.open("/config7.json", "w");
      jsonNew.printTo(configFile);
      configFile.close();
      jsonNew.prettyPrintTo(k);
    }
    else
    {
      readFile(SPIFFS, "/id18.txt");
#ifdef ENROLprint
      Software.print("streeng before assigning 'i': "); Software.println(streeng);
#endif
      int startp = 0;
      int endp = 1;
      int comma1p = streeng.indexOf(',', startp);
      endp = streeng.indexOf(':', comma1p + 1);

      String identity = streeng.substring(startp, comma1p);
      id = identity.toInt();
      if (enroll(id) == FINGERPRINT_OK) //----------------------------------------------
      {
        String restIDs = streeng.substring(comma1p + 1, endp + 1);
        if (id != 0)
        {
          if (size > 1024)
          {
#ifdef ENROLprint
            Software.println("Config file size is too large");
#endif
          }
          std::unique_ptr<char[]> buf(new char[size]);
          configFile.readBytes(buf.get(), size);
#ifdef ENROLprint
          Software.println("buf");
          for (int i = 0; i < size; i++)
          {
            Software.print(buf[i]);
          }
#endif
          JsonObject& jsonRead = jsonBuffer.parseObject(buf.get());
          if (!jsonRead.success())
          {
#ifdef ENROLprint
            Software.println("Failed to parse JSON file");
#endif
          }
          else
          {
#ifdef ENROLprint
            Software.println();
            Software.println("Reading before enrolling");
            jsonRead.prettyPrintTo(Software);
#endif
          }

          writeFile(SPIFFS, "/id18.txt", restIDs);
#ifdef ENROLprint
          Software.print("restIDs:"); Software.println(restIDs);
          Software.print("id: "); Software.println(id);
#endif
          JsonArray& levels = jsonRead.createNestedArray(employeeName);
          levels.add(employeeID);
          levels.add(id);   //need to be changed if there is chance of enrolling might reach 1000, even if many of them are deleted
          levels.add(String(server.arg("FINGER")));
          levels.add(String(server.arg("ROLE")));
          configFile = SPIFFS.open("/config7.json", "w");
          jsonRead.printTo(configFile);
          configFile.close();
          jsonRead.prettyPrintTo(k);
#ifdef ENROLprint
          Software.println();
          Software.println("----------------------------------------------------------------------------");
          Software.println("Updated List After Enrolling");
          Software.println(k);
          Software.println();
          Software.println("----------------------------------------------------------------------------");
          Software.println();
          Software.print("Enrolled "); Software.println(employeeName);
#endif
        }
        else// if (restIDs.length() == 1)
        {
#ifdef ENROLprint
          Software.print("Unable to enrol "); Software.print(employeeName); Software.println(" as the limit is reached");
#endif
          String content = "<html><body><H2><center>Limit reached. To enrol '" + employeeName + "' delete at least one of the irrelevant employees.<br><br> <center><a href='/delete'>Delete</a></H3></body></html>";
          server.send(200, "text/html", content);
        }
      }
      else
      {
        String content = "<html><body><H2><center>Unable to enrol '" + employeeName + "'. Please make sure you are placing your finger properly on the Fingerprint Sensor. Please try again.<br><br> <center><a href='/user'>Enrol</a></H3></body></html>";
        server.send(200, "text/html", content);
      }
      setupWatchdog(wdtTimeout);
    }

    server.sendHeader("Location", "/");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
    server.send(301);
#ifdef ENROLprint
    Software.print("Enrolled "); Software.println(employeeName);
#endif
  }

  String content = "<html><body><H2><center><form action='/user' method='POST'>Enter Employee Details</H2><br>";
  content += "<H3><center>Employee  <input type='text' name='EMPLOYEE' placeholder='Name' required><br><br>";
  content += "<H3><center>Employee ID  <input type='text' name='EMPLOYEE_ID' placeholder='ID' required><br><br>";
  content += "Finger  <select name='FINGER'><option value='RT'>Right Thumb</option><option value='RI'>Right Index</option><option value='LT'>Left Thumb</option><option value='LI'>Left Index</option></select><br><br>";
  content += "Role  <select name='ROLE'><option value='operator'>Operator</option><option value='safety'>Safety</option><option value='maintenance'>Maintenance</option><option value='production'>Production</option></select><br><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form></H3><br>";
  server.send(200, "text/html", content);
}

void handleDelete()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
  File listFile = SPIFFS.open("/config7.json", "r");
  if (listFile)
  {
#ifdef ENROLprint
    Software.println();
    Software.println("opened to delete");
#endif
  }
  size_t size = listFile.size();
#ifdef ENROLprint
  Software.print("Size is ");
  Software.println(size);
#endif
  if (size == 0)
  {
    //no one enrolled....enrol now
    String content = "<html><body><H2><center>No one is enrolled to delete???</H2></body></html>";
    server.send(200, "text/html", content);
  }
  if (size > 1024)
  {
#ifdef ENROLprint
    Software.println("Config file size is too large");
#endif
  }
  std::unique_ptr<char[]> buf(new char[size]);
  listFile.readBytes(buf.get(), size);
#ifdef ENROLprint
  Software.print("buf: ");
  for (int i = 0; i < size; i++)
  {
    Software.print(buf[i]);
  }
  Software.println();
#endif
  JsonObject& jsonRead = jsonBuffer.parseObject(buf.get());
  if (!jsonRead.success())
  {
#ifdef ENROLprint
    Software.println("Failed to parse JSON file");
#endif
  }
  else
  {
#ifdef ENROLprint
    Software.println("Deleting");
    jsonRead.prettyPrintTo(Software);
#endif
  }
#ifdef ENROLprint
  Software.println();
  Software.println("List of Employees Enrolled");
#endif
  String content = "<html><body><H2><center><form action='/removal' method='POST'>List of Employees Enrolled</H2><br><br>";
  for (auto kv : jsonRead)
  {
    String delName = String(kv.key);
#ifdef ENROLprint
    Software.print(delName);
    Software.print(", ");
#endif
    content += "<H3><center><input type='radio' name='deleteName' value='" + delName + "' required>" + delName + "</H3><br>";
    //    <input type='radio' name='FINGER' value='female'> Female<br><br>
  }
  content += "<H3><center><input type='submit' name='SUBMIT' value='Submit'></form><br>";
  server.send(200, "text/html", content);
}

void handleRemoval()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
#ifdef ENROLprint
  Software.println();
  Software.println("Enter handleRemoval");
#endif
  if (server.hasHeader("Cookie"))
  {
    String cookie = server.header("Cookie");
#ifdef ENROLprint
    Software.println();   Software.print("Found cookie: ");
    Software.println(cookie);
#endif
  }
  if (server.hasArg("DISCONNECT"))
  {
#ifdef ENROLprint
    Software.println("Disconnection");
#endif
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("deleteName"))
  {
    File removalFile = SPIFFS.open("/config7.json", "r");
    if (removalFile)
    {
#ifdef ENROLprint
      Software.println();
      Software.println("opened to delete");
#endif
    }
    size_t size = removalFile.size();
#ifdef ENROLprint
    Software.print("Size is ");
    Software.println(size);
#endif
    if (size < 3)
    {
      //no one enrolled....enrol now
      String content = "<html><body><H2><center>No one is enrolled.</H2></body></html>";
      server.send(200, "text/html", content);
    }
    if (size > 1024)
    {
#ifdef ENROLprint
      Software.println("Config file size is too large");
#endif
    }
    std::unique_ptr<char[]> buf(new char[size]);
    removalFile.readBytes(buf.get(), size);
#ifdef ENROLprint
    Software.print("buf: ");
    for (int i = 0; i < size; i++)
    {
      Software.print(buf[i]);
    }
    Software.println();
#endif
    JsonObject& jsonRemoval = jsonBuffer.parseObject(buf.get());
    if (!jsonRemoval.success())
    {
#ifdef ENROLprint
      Software.println("Failed to parse JSON file");
#endif
    }
    else
    {
#ifdef ENROLprint
      Software.println("Before Deleting");
      jsonRemoval.prettyPrintTo(Software);
#endif
    }
#ifdef ENROLprint
    Software.println();
    Software.println("List of Employees Enrolled");
#endif
    oldname = String(server.arg("deleteName"));
#ifdef ENROLprint
    Software.println();
    Software.println("In for Deleting");
    Software.print("Deleting ");  Software.println(String(oldname));
#endif
    int identity = jsonRemoval[oldname][1];
    jsonRemoval.remove(oldname);
    deleteFingerprint(identity); //fingerprint function

    File updatedRFile = SPIFFS.open("/config7.json", "w");
    if (!updatedRFile)
    {
#ifdef ENROLprint
      Software.println();
      Software.println("Unable to open config7.json to delete the name");
#endif
    }
    else
    {
      jsonRemoval.printTo(updatedRFile);
      updatedRFile.close();
#ifdef ENROLprint
      Software.print("Deleted ");  Software.println(String(oldname));
#endif
      //adding back the id number that has been freed to id18.txt ---------------------------------------------------
      readFile(SPIFFS, "/id18.txt");
      int startp = 0;
      int endp = 1;
      endp = streeng.indexOf(':', 0);
      String restIDs = streeng.substring(0, endp );
      String ideees = restIDs + String(identity) + ",:";
#ifdef ENROLprint
      Software.print("id18.txt before delete:"); Software.println(streeng);
      Software.print("After deleting employee, restIDs:"); Software.println(ideees);
#endif
      writeFile(SPIFFS, "/id18.txt", ideees);
      readFile(SPIFFS, "/id18.txt");
#ifdef ENROLprint
      Software.print("id18.txt after delete:"); Software.println(streeng);
#endif

      String content = "<html><body><H2><center>You successfully deleted " + oldname + " !</H2><br><H3><center><a href='/user'>Enrol</a><br><br><center><a href='/list'>List of Operators</a><br><br><center><a href='/delete'>Delete</a></H3></body></html>";
      server.send(200, "text/html", content);
    }
  }
}

//void handlef1()
//{
//  String content = "<html><body><H2><center>Place your finger on the Fingerprint Sensor</body></html>";
//  //String content = "<html><body><H1><center><br><br></body></html>";
//  server.send(200, "text/html", content);
//}
//
//void handlef2()
//{
//  String content = "<html><body><H2><center>Remove your finger</body></html>";
//  //String content = "<html><body><H1><center><br><br></body></html>";
//  server.send(200, "text/html", content);
//}
//
//void handlef3()
//{
//  String content = "<html><body><H2><center>Place same finger again</body></html>";
//  //String content = "<html><body><H1><center><br><br></body></html>";
//  server.send(200, "text/html", content);
//}
//
//
//void handlef4()
//{
//  String content = "<html><body><H2><center>Remove Finger</body></html>";
//  //String content = "<html><body><H1><center><br><br></body></html>";
//  server.send(200, "text/html", content);
//}

void handleRoot()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
#ifdef ENROLprint
  Software.println("Enter handleRoot");
#endif
  String header;
  if (!is_authentified())
  {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }
  String content = "<html><body><H2><center>You successfully enrolled " + employeeName + " !</H2><br><H3><center><a href='/user'>Enrol</a><br><br><center><a href='/list'>List of Operators</a><br><br><center><a href='/delete'>Delete</a></H3></body></html>";
  server.send(200, "text/html", content);
}

void handleNotFound()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupServer()
{
  //  if (!enrol_flag)
  //  {
  //    enrol_flag = true;
  //    enrol_time = millis();
  //  }
#ifdef SetupPrint
  Software.println(F("Setup Server"));
#endif
#ifndef Internet
  Software.println("Configuring access point...");
  const char *ssid = "Forklift_Enroller";
  const char *password = "HACKLAB321";
  // You can remove the password parameter if you want the AP to be open.
  WiFi.softAP(ssid, password);
  delay(500);
  IPAddress ip(192, 168, 0, 10);
  IPAddress gateway(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(ip, gateway, subnet);
  IPAddress myIP = WiFi.softAPIP();
  Software.print("AP IP address: ");
  Software.println(myIP);
  server.begin();
  Software.println("Server started");
#endif
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  timerWrite(timer, 0);
  server.on("/option", handleOption);
  server.on("/user", handleUser);
  timerWrite(timer, 0);
  server.on("/list", handleList);
  server.on("/delete", handleDelete);
  server.on("/removal", handleRemoval);
  //    server.on("/f1", handlef1);
  //  server.on("/f2", handlef2);
  //  server.on("/f3", handlef3);
  //  server.on("/f4", handlef4);
  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works without need of authentification");
  });
  timerWrite(timer, 0);
  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  server.begin();
  timerWrite(timer, 0);
#ifdef ENROLprint
  Software.println("HTTP server started");
#endif
}



uint8_t bytesReceived[534]; // 2 data packets
//WebServer server(80);
uint8_t p = -1;

uint8_t deleteFingerprint(int id)
{
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK)
  {
#ifdef FPprint
    Software.println("Deleted on the sensor!");
#endif
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
#ifdef FPprint
    Software.println("Communication error..p:" + p);
#endif
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
#ifdef FPprint
    Software.println("Could not delete in that location..p:" + p);
#endif
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
#ifdef FPprint
    Software.println("Error writing to flash..p:" + p);
#endif
    return p;
  }
  else
  {
#ifdef FPprint
    Software.print("Unknown error: 0x"); Software.println(p);
#endif
    return p;
  }
}

uint8_t getFingerprintEnroll(int enroll_ID)
{
  int p = -1;
#ifdef FPprint
  Software.print("Waiting for valid finger to enroll as #"); Software.println(enroll_ID);
#endif
  //handlef1();

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
#ifdef FPprint
        Software.println("Image taken..p:" + p);
#endif
        break;
      case FINGERPRINT_NOFINGER:
#ifdef FPprint
        Software.print(".");
#endif
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FPprint
        Software.println("Communication error..p:" + p);
#endif
        break;
      case FINGERPRINT_IMAGEFAIL:
#ifdef FPprint
        Software.println("Imaging error..p:" + p);
#endif
        break;
      default:
#ifdef FPprint
        Software.println("Unknown error..p:" + p);
#endif
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
#ifdef FPprint
      Software.println("Image converted..p:" + p);
#endif
      break;
    case FINGERPRINT_IMAGEMESS:
#ifdef FPprint
      Software.println("Image too messy..p:" + p);
#endif
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FPprint
      Software.println("Communication error..p:" + p);
#endif
      return p;
    case FINGERPRINT_FEATUREFAIL:
#ifdef FPprint
      Software.println("Could not find fingerprint features..p:" + p);
#endif
      return p;
    case FINGERPRINT_INVALIDIMAGE:
#ifdef FPprint
      Software.println("Could not find fingerprint features..p:" + p);
#endif
      return p;
    default:
#ifdef FPprint
      Software.println("Unknown error..p:" + p);
#endif
      return p;
  }
#ifdef FPprint
  Software.println("Remove finger");
#endif
  //handlef2();

  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
#ifdef FPprint
  Software.print("enroll_ID "); Software.println(enroll_ID);
#endif
  p = -1;
#ifdef FPprint
  Software.println("Place same finger again");
#endif
  //handlef3();

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
#ifdef FPprint
        Software.println("Image taken..p:" + p);
#endif
        break;
      case FINGERPRINT_NOFINGER:
        Software.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FPprint
        Software.println("Communication error..p:" + p);
#endif
        break;
      case FINGERPRINT_IMAGEFAIL:
#ifdef FPprint
        Software.println("Imaging error..p:" + p);
#endif
        break;
      default:
#ifdef FPprint
        Software.println("Unknown error..p:" + p);
#endif
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
#ifdef FPprint
      Software.println("Image converted..p:" + p);
#endif
      break;
    case FINGERPRINT_IMAGEMESS:
#ifdef FPprint
      Software.println("Image too messy..p:" + p);
#endif
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FPprint
      Software.println("Communication error..p:" + p);
#endif
      return p;
    case FINGERPRINT_FEATUREFAIL:
#ifdef FPprint
      Software.println("Could not find fingerprint features..p:" + p);
#endif
      return p;
    case FINGERPRINT_INVALIDIMAGE:
#ifdef FPprint
      Software.println("Could not find fingerprint features..p:" + p);
#endif
      return p;
    default:
#ifdef FPprint
      Software.println("Unknown error..p:" + p);
#endif
      return p;
  }

  //handlef4();

  // OK converted!
#ifdef FPprint
  Software.print("Creating model for #");  Software.println(enroll_ID);
#endif

  p = finger.createModel();
  if (p == FINGERPRINT_OK)
  {
#ifdef FPprint
    Software.println("Prints matched!");
#endif
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
#ifdef FPprint
    Software.println("Communication error..p:" + p);
#endif
    return p;
  }
  else if (p == FINGERPRINT_ENROLLMISMATCH)
  {
#ifdef FPprint
    Software.print("Fingerprints did not match..p:"); Software.println(p, HEX);
#endif
    //return p;
  }
  else
  {
#ifdef FPprint
    Software.println("Unknown error..p:" + p);
#endif
    return p;
  }
#ifdef FPprint
  Software.print("enroll_ID "); Software.println(enroll_ID);
#endif


  //..................................................................................................................
  if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    delay(2000);
    Software.println("Place same finger again");
    //    digitalWrite(green, HIGH);   digitalWrite(red, HIGH); digitalWrite(blue, LOW);
    //    handlef3();
    //    String content = "<html><body><H2><center>Place same finger again</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);

    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Software.println("Image taken..p:" + p);
          break;
        case FINGERPRINT_NOFINGER:
          Software.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Software.println("Communication error..p:" + p);
          break;
        case FINGERPRINT_IMAGEFAIL:
          Software.println("Imaging error..p:" + p);
          break;
        default:
          Software.println("Unknown error..p:" + p);
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Software.println("Image converted..p:" + p);
        break;
      case FINGERPRINT_IMAGEMESS:
        Software.println("Image too messy..p:" + p);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Software.println("Communication error..p:" + p);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      default:
        Software.println("Unknown error..p:" + p);
        return p;
    }
    //    digitalWrite(green, LOW);   digitalWrite(red, LOW); digitalWrite(blue, LOW);

    //    handlef4();
    //    content = "<html><body><H2><center>Remove Finger</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);

    // OK converted!
    Software.print("Creating model for #");  Software.println(enroll_ID);

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Software.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Software.println("Communication error..p:" + p);
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
#ifdef DEBUG_IMP
      Software.print("Fingerprints did not match..p:"); Software.println(p, HEX);
#endif
      //return p;
    } else {
      Software.println("Unknown error..p:" + p);
      return p;
    }
  }

  if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    delay(2000);
    //    digitalWrite(green, HIGH);   digitalWrite(red, HIGH); digitalWrite(blue, LOW);
    Software.println("Place same finger again");
    //    handlef3();
    //String content = "<html><body><H2><center>Place same finger again</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);
    while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
        case FINGERPRINT_OK:
          Software.println("Image taken..p:" + p);
          break;
        case FINGERPRINT_NOFINGER:
          Software.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Software.println("Communication error..p:" + p);
          break;
        case FINGERPRINT_IMAGEFAIL:
          Software.println("Imaging error..p:" + p);
          break;
        default:
          Software.println("Unknown error..p:" + p);
          break;
      }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Software.println("Image converted..p:" + p);
        break;
      case FINGERPRINT_IMAGEMESS:
        Software.println("Image too messy..p:" + p);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Software.println("Communication error..p:" + p);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      default:
        Software.println("Unknown error..p:" + p);
        return p;
    }
    //    digitalWrite(green, LOW);   digitalWrite(red, LOW); digitalWrite(blue, LOW);

    //    handlef4();
    //    String content = "<html><body><H2><center>Remove Finger</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);
    // OK converted!
    Software.print("Creating model for #");  Software.println(enroll_ID);

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Software.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Software.println("Communication error..p:" + p);
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
#ifdef DEBUG_IMP
      Software.print("Fingerprints did not match..p:"); Software.println(p, HEX);
#endif
      //return p;
    } else {
      Software.println("Unknown error..p:" + p);
      return p;
    }
  }

  if (p == FINGERPRINT_ENROLLMISMATCH)
  {
    delay(2000);
    Software.println("Place same finger again");
    //    digitalWrite(green, HIGH);   digitalWrite(red, HIGH); digitalWrite(blue, LOW);
    //    handlef3();
    //    String content = "<html><body><H2><center>Place same finger again</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);
    while (p != FINGERPRINT_OK)
    {
      p = finger.getImage();
      switch (p)
      {
        case FINGERPRINT_OK:
          Software.println("Image taken..p:" + p);
          break;
        case FINGERPRINT_NOFINGER:
          Software.print(".");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Software.println("Communication error..p:" + p);
          break;
        case FINGERPRINT_IMAGEFAIL:
          Software.println("Imaging error..p:" + p);
          break;
        default:
          Software.println("Unknown error..p:" + p);
          break;
      }
    }


    // OK success!

    p = finger.image2Tz(2);
    switch (p)
    {
      case FINGERPRINT_OK:
        Software.println("Image converted..p:" + p);
        break;
      case FINGERPRINT_IMAGEMESS:
        Software.println("Image too messy..p:" + p);
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Software.println("Communication error..p:" + p);
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Software.println("Could not find fingerprint features..p:" + p);
        return p;
      default:
        Software.println("Unknown error..p:" + p);
        return p;
    }
    //    digitalWrite(green, LOW);   digitalWrite(red, LOW); digitalWrite(blue, LOW);

    //    handlef4();
    //    String content = "<html><body><H2><center>Remove Finger</body></html>";
    //    //content = "<html><body><H1><center><br><br></body></html>";
    //    server.send(200, "text/html", content);
    // OK converted!
    Software.print("Creating model for #");  Software.println(enroll_ID);

    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Software.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Software.println("Communication error..p:" + p);
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
#ifdef DEBUG_IMP
      Software.print("Fingerprints did not match..p:"); Software.println(p, HEX);
#endif
      //      digitalWrite(red, HIGH); digitalWrite(blue, LOW); digitalWrite(green, LOW);
      delay(500);
      //      digitalWrite(red, LOW);
      delay(500);
      //      digitalWrite(red, HIGH);
      delay(500);
      //      digitalWrite(red, LOW);
      delay(500);
      //      digitalWrite(red, HIGH);
      delay(500);
      //      digitalWrite(red, LOW);
      delay(2000);
      //      digitalWrite(blue, HIGH); digitalWrite(red, LOW); digitalWrite(green, LOW);
      return p;
    } else {
      Software.println("Unknown error..p:" + p);
      return p;
    }
  }
  //................................................................................................


  p = finger.storeModel(enroll_ID);
  if (p == FINGERPRINT_OK)
  {
#ifdef FPprint
    Software.println("Stored!");
#endif
    return p;
  }
  else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
#ifdef FPprint
    Software.println("Communication error..p:" + p);
#endif
    return p;
  }
  else if (p == FINGERPRINT_BADLOCATION)
  {
#ifdef FPprint
    Software.println("Could not store in that location..p:" + p);
#endif
    return p;
  }
  else if (p == FINGERPRINT_FLASHERR)
  {
#ifdef FPprint
    Software.println("Error writing to flash..p:" + p);
#endif
    return p;
  }
  else
  {
#ifdef FPprint
    Software.println("Unknown error..p:" + p);
#endif
    return p;
  }
}


bool checkFingerPrint()
{
  delay(10);

  //return true;
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return false;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return false;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
#ifdef MainPrint
    Software.println(); Software.println(); Software.println(); Software.println(); Software.println();
    Software.println("Found print not a match"); Software.println("Found print not a match"); Software.println("Found print not a match");
    Software.println(); Software.println(); Software.println(); Software.println(); Software.println();
#endif
    return false;
  }
  else
  {
    operatorID = finger.fingerID;
#ifdef MainPrint
    Software.println();
    Software.print("Found ID #"); Software.print(finger.fingerID);
    Software.print(" with confidence of "); Software.println(finger.confidence);
#endif
    return true;
  }
  return false;
}

void setupWatchdog(const int wdtTimer)
{
#ifdef SetupPrint
  Software.println(F("Setup Watchdog"));
#endif
  timer = timerBegin(1, 80, true);                  //timer 1, div 80
  timerAttachInterrupt(timer, &resetModule, true);  //attach callback
  timerAlarmWrite(timer, wdtTimer * 1000, false); //set time in us
  timerAlarmEnable(timer);                          //enable interrupt
}


bool enroll(int enroll_ID)
{
  setupWatchdog(100000);
#ifdef FPprint
  Software.println("Enroll");
#endif
  finger.getTemplateCount();

#ifdef FPprint
  Software.println("exiting Enroll");
#endif
  return getFingerprintEnroll(enroll_ID);
}
