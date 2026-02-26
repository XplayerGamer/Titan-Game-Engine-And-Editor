#pragma once

#ifdef TITANENGINE_EXPORTS
#define TITAN_API __declspec(dllexport)
#else
#define TITAN_API __declspec(dllimport)
#endif