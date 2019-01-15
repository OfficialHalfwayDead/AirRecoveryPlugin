# AirRecoveryPlugin
BakkesMod plugin intended to practice recovery in the air.

## Installation

1. Copy `AirRecoveryPlugin.dll` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins`
2. Copy `airrecoveryplugin.set` to `<steam_directory>/steamapps/common/rocketleague/Binaries/Win32/bakkesmod/plugins/settings`
3. In the game console run `plugin load airrecoveryplugin`

## Commands

**airrecovery_start** - Starts airrecovery minigame.

**airrecovery_stop** - Stops airrecovery minigame.

**airrecovery_bumpspeed_angular** - Sets the strength of the torque applied to the car in a bump.

**airrecovery_bumpspeed_linear** - Set the strength of the horizontal force aplied to the car in a bump.

**airrecovery_bumpspeed_linear_z** - Set the strength of the vertical force aplied to the car in a bump. (Negative values=down, positive=up)

**airrecovery_cooldown** - Set the time between bumps in ms.
