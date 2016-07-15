// 
// 
// 

#include "ReadIdsState.h"
#include "ReadDataOfIdsState.h"
#include "BuildsFailedAndRunningLightStrategy.h"
#include "BuildsFailedLightStrategy.h"
#include "BuildsSuccessLightStrategy.h"
#include "TestConnectToWifiState.h"


ReadDataOfIdsState::ReadDataOfIdsState() {}

ReadDataOfIdsState::~ReadDataOfIdsState() {

}

void ReadDataOfIdsState::process() {
	Serial.println(F("---ReadDataOfIdsState---"));

	WifiUtils.runScript(F("get_stat.lua"));
	// choose next state
	String resp = WifiUtils.readResponce(15000);
	if (resp.equalsIgnoreCase("OK")) {
		String stat = WifiUtils.readResponce();

		delayMs = 5000; // msec if all good
		nextState = new ReadIdsState();

		// change light strategy
		if (stat.equalsIgnoreCase(F("FAIL")))
		{
			lightStrategy = new BuildsFailedLightStrategy();
			SoundManager.playBadSound();
		}
		else if (stat.equalsIgnoreCase(F("RUN")))
		{
			lightStrategy = new BuildsFailedAndRunningLightStrategy();
			SoundManager.clearPrevPlayedSoundType();
		}
		else
		{
			lightStrategy = new BuildsSuccessLightStrategy();
			SoundManager.playGoodSound();
		}
	}
	else {
		SystemUtils.printError(resp);

		nextState = new TestConnectToWiFiState(); 
		delayMs = 8000;
	}
}