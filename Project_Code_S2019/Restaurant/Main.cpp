#include "Rest\Restaurant.h"
//#include "vld.h"
using namespace std;
int main()
{
	Restaurant* pRest = new Restaurant;
	pRest->RunSimulation();
	delete pRest;
	return 0;
}
