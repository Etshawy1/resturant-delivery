#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "..\InOutFile.h"
#include "..\GUI\GUI.h"
#include "..\Region.h"
#include "..\Defs.h"
#include "Order.h"

// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file
	Region R[4];
	int AutoProm;

public:
	Restaurant();
	~Restaurant();
	void AddEvent(Event* pE);	        //adds a new event to the queue of events
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void SetAutoProm(int);
	int GetAutoProm() const;
	Region* GetRegion(int);
	void RunSimulation();				//prompt the user to choose the mode to start the simulation
	bool ReadFile();					//function to read input from a file
	void PrintStatuesBar();				//prints information about different regions
	void simulate();					//the simple simulation function
	bool LoadGUI();						//to load the gui array of pointers with orders to draw
	void DeleteMax();					//delete the order which should be deleted in the current time step
};

#endif