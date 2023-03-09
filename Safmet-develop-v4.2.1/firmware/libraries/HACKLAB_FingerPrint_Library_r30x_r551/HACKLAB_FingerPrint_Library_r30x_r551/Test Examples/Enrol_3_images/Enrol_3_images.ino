#include "config.h"
#include "declare.h"
#include "define.h"

void setup(void)
{
  Software.begin(115200);
  Software.println("Biometric Access Controller");
  updateCheck();
  setupWatchdog(wdtTimeout);
  setupServer();
  timerWrite(timer, 0);
  setupFiles();
  setupFingerPrint();
  Software.println(F("FP Setup"));
  timerWrite(timer, 0);
  Software.println("end of setup");
  looptime = millis();
}

void loop(void)
{
  if (millis() - looptime > 1000)
  {
#ifdef MainPrint
    Software.print(".");
#endif
    looptime = millis();
  }
  server.handleClient();
  fp_flag = checkFingerPrint();
  if (fp_flag)
  {

  }

}
