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
	DESERT,
	CLAY = 1,
	MOUNTAIN = 2,
	FORREST = 3,
	PASTURE = 4,
	FIELD = 5,
	SEA = 6,
	TRADE = 7
};

UENUM(BlueprintType)
enum class EResource : uint8 {
	NONE,
	BRICKS = 1,
	ORE = 2,
	WOOD = 3,
	WOOL = 4,
	WHEAT = 5,
};

class THE_SETTLERS_API ENUMS{
public:
	ENUMS();
};
