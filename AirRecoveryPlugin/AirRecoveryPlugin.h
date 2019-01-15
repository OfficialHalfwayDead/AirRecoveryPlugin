#pragma once
#include "bakkesmod/plugin/bakkesmodplugin.h"
#pragma comment( lib, "bakkesmod.lib" )


class AirRecoveryPlugin : public BakkesMod::Plugin::BakkesModPlugin
{
private:
	bool recoveryEnabled = false;
	float lastBumpTime = 0.0f;
	float lastCooldownTime = 0.0f;
public:
	virtual void onLoad();
	virtual void onUnload();
	void CheckForBump();
	float GetBumpTimeout();
	void ExecuteBump();
};