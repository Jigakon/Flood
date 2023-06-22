#ifndef Types__H
#define Types__H

#include <bitset>

namespace Flood
{
	namespace ECS
	{
		using Entity = std::uint32_t; // int mais standardisé par le cpp en gros
		const Entity MAX_ENTITIES = 5000;
		const Entity Null = 0xFFFFFFFF;
		using ComponentType = std::uint8_t; // char mais standardisé par le cpp en gros
		const ComponentType MAX_COMPONENTS = 32;
		using Signature = std::bitset<MAX_COMPONENTS>; // c'est un masque de MAX_COMPONENT bit, il n'y a donc que 32 composants max dans cet exemple (0b11100000000000000000000000000000 si les 3 premiers components sont présents dans l'entity) 
	}
}
#endif