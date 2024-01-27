#include "APossessable.h"
#include "IncreaseCommand.h"
#include "DefineVar.h"

using namespace std;

APossessable::APossessable()
{

}

APossessable::~APossessable()
{
	for (unsigned __int64 code = 0; code < EKeyCode::NUM_KEY; ++code)
	{
		AutoDelete(pCommand[code]);
	}
}
