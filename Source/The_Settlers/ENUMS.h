#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECards :uint8 {
	NONE,
	KNIGHT,
	FREEROAD,
	VICTORYPOINT,
	YEAROFPLENTY,
	MONOPOLY
};

UENUM(BlueprintType)
enum class EPlayer : uint8 {
	NONE,
	PLAYER1 = 1,
	PLAYER2 = 2,
	PLAYER3 = 3,
	PLAYER4 = 4
};

UENUM(BlueprintType)
enum class EHexTile : uint8 {
	CLAY = 0,
	MOUNTAIN = 1,
	FORREST = 2,
	PASTURE = 3,
	FIELD = 4,
	DESERT = 5,
	SEA = 6,
	TRADE = 7
};

UENUM(BlueprintType)
enum class EResource : uint8 {
	BRICKS = 0,
	ORE = 1,
	WOOD = 2,
	WOOL = 3,
	WHEAT = 4,
};

class THE_SETTLERS_API ENUMS{
public:
	ENUMS();
};
