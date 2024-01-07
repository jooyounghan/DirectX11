#include "APossessable.h"
#include "IncreaseCommand.h"

using namespace std;

APossessable::APossessable()
{

}

APossessable::~APossessable()
{
	for (unsigned __int64 code = 0; code < KeyCode::NUM_KEY; ++code)
	{
		if (pCommand[code]) delete pCommand[code]; pCommand[code] = nullptr;
	}
}
