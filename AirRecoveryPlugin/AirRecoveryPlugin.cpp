// AirRecoveryPlugin.cpp : Defines the exported functions for the DLL application.
//

#include "AirRecoveryPlugin.h"
#include "utils\parser.h"


BAKKESMOD_PLUGIN(AirRecoveryPlugin, "Tutorial plugin", "1.0", PLUGINTYPE_FREEPLAY)

void AirRecoveryPlugin::onLoad()
{
	cvarManager->registerCvar("airrecovery_bumpspeed_angular", "(0, 5.5)", "How hard you will get thrown rotationally", true, true, 0.0f, true, 10.0f);
	cvarManager->registerCvar("airrecovery_bumpspeed_linear", "(500, 1000)", "How hard you will get thrown", true, true, 0.f, true, 2300.0f);
	cvarManager->registerCvar("airrecovery_bumpspeed_linear_z", "(-1000, 500)", "How hard you will get thrown (height)", true, true, -2300.0f, true, 2300.0f);
	cvarManager->registerCvar("airrecovery_cooldown", "(3000, 6000)", "Minimum time to wait after a bump", true, true, 0.f, true, 120000.f);

	cvarManager->registerNotifier("airrecovery_start", [this](std::vector<string> params) {
		if (!gameWrapper->IsInGame())
		{
			cvarManager->log("You need to be in freeplay to use this plugin.");
			return;
		}
		lastCooldownTime = cvarManager->getCvar("airrecovery_cooldown").getFloatValue() / 1000;
		this->recoveryEnabled = true;
		this->CheckForBump();
	}, "Starts the recovery mode which will bump you with the given settings.", PERMISSION_FREEPLAY);

	cvarManager->registerNotifier("airrecovery_stop", [this](std::vector<string> params) {
		this->recoveryEnabled = false;
	}, "Stops the recovery plugin.", PERMISSION_ALL);
}

void AirRecoveryPlugin::onUnload()
{
}


void AirRecoveryPlugin::CheckForBump()
{
	if (!recoveryEnabled || !gameWrapper->IsInGame())
		return; //Player stopped recovery training or left freeplay

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->CheckForBump();
	}, this->GetBumpTimeout());
}

float AirRecoveryPlugin::GetBumpTimeout()
{
	if (!gameWrapper->IsInGame() || !recoveryEnabled)
		return .5f;
	ServerWrapper training = gameWrapper->GetGameEventAsServer();
	float lastBump = training.GetSecondsElapsed() - lastBumpTime;
	if (lastBump > lastCooldownTime)
	{
		auto cars = training.GetCars();
		if (cars.Count() == 0)
		{
			return random(.2f, 1.f);
		}
		auto gameCar = cars.Get(0);

		if (!gameCar.IsOnGround() && !gameCar.IsOnWall()) //player is in the air
		{
			ExecuteBump();
			return lastCooldownTime;
		}
		return random(.2f, 1.f);
	}
	return lastCooldownTime - lastBump + 0.1f;
}

void AirRecoveryPlugin::ExecuteBump()
{
	if (!gameWrapper->IsInGame() || !recoveryEnabled)
		return;
	auto server = gameWrapper->GetGameEventAsServer();



	Vector angularBump = {
		cvarManager->getCvar("airrecovery_bumpspeed_angular").getFloatValue(),
		cvarManager->getCvar("airrecovery_bumpspeed_angular").getFloatValue(),
		cvarManager->getCvar("airrecovery_bumpspeed_angular").getFloatValue()
	};
	Vector linearBump = {
		cvarManager->getCvar("airrecovery_bumpspeed_linear").getFloatValue(),
		cvarManager->getCvar("airrecovery_bumpspeed_linear").getFloatValue(),
		cvarManager->getCvar("airrecovery_bumpspeed_linear_z").getFloatValue()
	};


	angularBump.X = random(0, 1) == 1 ? angularBump.X : -angularBump.X;
	angularBump.Y = random(0, 1) == 1 ? angularBump.Y : -angularBump.Y;
	angularBump.Z = random(0, 1) == 1 ? angularBump.Z : -angularBump.Z;

	linearBump.X = random(0, 1) == 1 ? linearBump.X : -linearBump.X;
	linearBump.Y = random(0, 1) == 1 ? linearBump.Y : -linearBump.Y;

	auto cars = server.GetCars();
	if (cars.Count() == 0)
	{
		return;
	}
	auto car = cars.Get(0);
	car.SetAngularVelocity(angularBump, 1);
	car.AddVelocity(linearBump);


	lastBumpTime = server.GetSecondsElapsed();
	lastCooldownTime = cvarManager->getCvar("airrecovery_cooldown").getFloatValue() / 1000;
}