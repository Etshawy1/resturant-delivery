#include "Region.h"


Region::Region(void)
{
	
	N_MotorsCnt = V_MotorsCnt = F_MotorsCnt = NOrderCount = VOrderCount = FOrderCount =wholeNorders=wholeForders=wholeVoreders= 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to insert different types motorcycles							  //
//					        	 read from the input file									  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

void Region::InsertFMotor(int id, double speed, STATUS s, ORD_TYPE t, REGION r)
{
	Motorcycle* M = new Motorcycle (id, speed, s, t, r);
	F_MotorsCnt++;
	idelFMotorQ.insert(M);
}

void Region::InsertNMotor(int id, double speed, STATUS s, ORD_TYPE t, REGION r)
{
	Motorcycle* M = new Motorcycle (id, speed, s, t, r);
	N_MotorsCnt++;
	idelNMotorQ.insert(M);
}

void Region::InsertVMotor(int id, double speed, STATUS s, ORD_TYPE t, REGION r)
{
	Motorcycle* M = new Motorcycle (id, speed, s, t, r);
	V_MotorsCnt++;
	idelVMotorQ.insert(M);
}



////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to insert different types orders								  //
//					         in the convinient list											  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


void Region::InsertFOrder(Order* F)
{
	FOrderCount++;
	FOrderQueue.enqueue(F);
}
void Region::InsertNOrder(Order* N)
{
	NOrderCount++;
	NOrderQueue.insert(NOrderQueue.getLength()+1,N);
}
void Region::InsertVOrder(Order* V)
{
	VOrderCount++;
	VOrderQueue.insert(V); 
}


////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to get the number of not served								  //
//							orders of different types									      //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


int Region::GetVOrdCnt(){ return VOrderQueue.getcount(); }
int Region::GetNOrdCnt(){ return NOrderQueue.getLength();}
int Region::GetFOrdCnt(){ return FOrderQueue.Get_count();}
bool Region::FOrdisEmpty(){ return FOrderQueue.isEmpty();}


////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to return an array of the different							  //
//							      types of orders										      //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////



Order* const* Region::GetArrVOrd()
{
	return VOrderQueue.ToArray();
}

Order* const* Region::GetArrFOrd()
{
	return FOrderQueue.ToArray();
}

Order*const* Region::GetArrNOrd()
{
	return NOrderQueue.ToArray();
}

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to delete the convinient order								  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


Order* Region::GetNOrder(int postion) 
{ 
	Order* ord ;
	NOrderQueue.remove(postion,ord);
	NOrderCount--;
	return ord; 
}

Order* Region::dequeueV() 
{ 
	Order* ord = VOrderQueue.extractMax();
	VOrderCount--;
	return ord; 
}

Order* Region::dequeueF()
{ 
	Order *ord ;
	FOrderQueue.dequeue(ord);
	FOrderCount--;
	return  ord;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to deallocate the motocycles									  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


void Region::DeleteMotors()
{

	while (idelNMotorQ.getcount() != 0)
	{
		delete idelNMotorQ.extractMax();
	}
	while (idelVMotorQ.getcount() != 0)
	{
		delete idelVMotorQ.extractMax();
	}
	while (idelFMotorQ.getcount() != 0)
	{
		delete idelFMotorQ.extractMax();
	}
	while (servMotorQ.getcount() != 0)
	{
		delete  servMotorQ.extractMax();
	}
	while(Damaged.Get_count() != 0)
	{
		Motorcycle* tmp;
		Damaged.dequeue(tmp);
		delete tmp;
	}
	while(tirFmotorQ.Get_count() != 0)
	{
		Motorcycle* tmp;
		tirFmotorQ.dequeue(tmp);
		delete tmp;
	}
	while(tirVmotorQ.Get_count() != 0)
	{
		Motorcycle* tmp;
		tirVmotorQ.dequeue(tmp);
		delete tmp;
	}
	while(tirNmotorQ.Get_count() != 0)
	{
		Motorcycle* tmp;
		tirNmotorQ.dequeue(tmp);
		delete tmp;
	}

}

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//				functions to return the whole number of different orders					  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

int Region::GetWholeFOrdCount() const{ return wholeForders; }
int Region::GetWholeNOrdCount() const{ return wholeNorders; }
int Region::GetWholeVOrdCount() const{ return wholeVoreders; }

////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to return the number of the motorcycles						  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


int Region::Get_FMotorCnt()const
{
	return F_MotorsCnt;
}


int Region::Get_NMotorCnt()const
{
	return N_MotorsCnt;
}


int Region::Get_VMotorCnt()const
{
	return V_MotorsCnt;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to Assign an order to motorcylce      						  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

bool Region::AssignOrdNMotor(int timestep , int timed , int timeT, priorityQueue<Order*>* serv, string& s)
{
	Order*tmp = nullptr;
	Motorcycle*NMotor = nullptr;
	Motorcycle*VMotor = nullptr;
	while(NOrderCount != 0 && N_MotorsCnt != 0)
	{
		if(idelNMotorQ.getcount()!=0)
		NMotor = idelNMotorQ.extractMax();
		else
			if(!tirNmotorQ.isEmpty())
				tirNmotorQ.dequeue(NMotor);
		else
			break;
		NOrderQueue.remove(1,tmp);
		NMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(NMotor);
		serv->insert(tmp);
		AppendString(s, tmp, NMotor);
		N_MotorsCnt--;
		NOrderCount--;
	}
	
	while(NOrderCount != 0 && V_MotorsCnt != 0 && VOrderCount == 0)
	{
		if(idelVMotorQ.getcount()!=0)
			VMotor = idelVMotorQ.extractMax();
		else
			if(!tirVmotorQ.isEmpty())
				tirVmotorQ.dequeue(VMotor);
		else
			break;
		NOrderQueue.remove(1,tmp);
		VMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(VMotor);
		serv->insert(tmp);
		AppendString(s, tmp, VMotor);
		V_MotorsCnt--;
		NOrderCount--;
	}

	return NOrderCount == 0  ;

}



bool Region::AssignOrdFMotor(int timestep , int timed , int timeT, priorityQueue<Order*>* serv, string& s)
{
	Order*tmp = nullptr;
	Motorcycle*FMotor = nullptr;
	while(FOrderCount != 0 && F_MotorsCnt != 0)
	{
		if(idelFMotorQ.getcount()!=0)
			FMotor = idelFMotorQ.extractMax();
		else
			if(!tirFmotorQ.isEmpty())
				tirFmotorQ.dequeue(FMotor);
		else
			break;
		FOrderQueue.dequeue(tmp);
		FMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(FMotor);
		serv->insert(tmp);
		AppendString(s, tmp, FMotor);
		F_MotorsCnt--;
		FOrderCount--;
	}

	return FOrderCount == 0 ;

}



bool Region::AssignOrdVMotor(int timestep , int timed , int timeT, priorityQueue<Order*>* serv, string& s)
{
	Order*tmp = nullptr;
	Motorcycle*VMotor = nullptr;
	Motorcycle*NMotor = nullptr;
	Motorcycle*FMotor = nullptr;
	Queue<Motorcycle*> temp ;
	while(VOrderCount != 0 && V_MotorsCnt != 0)
	{
		if(idelVMotorQ.getcount()!=0)
			VMotor = idelVMotorQ.extractMax();
		else
			if(!tirVmotorQ.isEmpty())
				tirVmotorQ.dequeue(VMotor);
		else
			break;
		tmp = VOrderQueue.extractMax();
		
		VMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(VMotor);
		serv->insert(tmp);
		AppendString(s, tmp, VMotor);
		V_MotorsCnt--;
		VOrderCount--;
	}
	while(VOrderCount != 0 && N_MotorsCnt != 0)
	{
		if(idelNMotorQ.getcount()!=0)
		NMotor = idelNMotorQ.extractMax();
		else
			if(!tirNmotorQ.isEmpty())
				tirNmotorQ.dequeue(NMotor);
		else
			break;
		tmp = VOrderQueue.extractMax();
		NMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(NMotor);
		serv->insert(tmp);
		AppendString(s, tmp, NMotor);
		N_MotorsCnt--;
		VOrderCount--;
	}
	while(VOrderCount != 0 && F_MotorsCnt != 0)
	{
		if(idelFMotorQ.getcount()!=0)
			FMotor = idelFMotorQ.extractMax();
		else
			if(!tirFmotorQ.isEmpty())
				tirFmotorQ.dequeue(FMotor);
		else
			break;
		tmp = VOrderQueue.extractMax();
		FMotor->SetAssignedOrd(tmp,timestep,timed,timeT);
		servMotorQ.insert(FMotor);
		serv->insert(tmp);
		AppendString(s, tmp, FMotor);
		F_MotorsCnt--;
		VOrderCount--;
	}
	return VOrderCount == 0 ;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to recover motorcylce				    						  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

void Region::recovered(int timestep)  
{
	Motorcycle* temp;
	while(!Damaged.isEmpty())
	{
		Damaged.peekFront(temp);
		if(!temp->Isdamaged(timestep))
		{
			Damaged.dequeue(temp);
			temp->SetDamaged(-1);
			if(temp->GetType() == TYPE_NRM)
			{
				idelNMotorQ.insert(temp);
				N_MotorsCnt++;
			}
			else
				if(temp->GetType() == TYPE_FROZ)
				{
					idelFMotorQ.insert(temp);
					F_MotorsCnt++;
				}
				else	
				{
					idelVMotorQ.insert(temp);
					V_MotorsCnt++;
				}
		}
		else	
			break ;
	}
	
	while(!tirFmotorQ.isEmpty())
	{
		tirFmotorQ.peekFront(temp);
		if(!temp->Istired(timestep))
		{
			temp->SetTired(-1);
			tirFmotorQ.dequeue(temp);
			idelFMotorQ.insert(temp);
		}
		else
			break;
	}
	while(!tirVmotorQ.isEmpty())
	{
		tirVmotorQ.peekFront(temp);
		if(!temp->Istired(timestep))
		{
			temp->SetTired(-1);
			tirVmotorQ.dequeue(temp);
			idelVMotorQ.insert(temp);
		}
		else
			break;
	}
	while(!tirNmotorQ.isEmpty())
	{
		tirNmotorQ.peekFront(temp);
		if(!temp->Istired(timestep))
		{
			temp->SetTired(-1);
			tirNmotorQ.dequeue(temp);
			idelNMotorQ.insert(temp);
		}
		else
			break;
	}
	
}



////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to Assign an order to motorcylce      						  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

bool Region::UnAssignMotors(int timestep)
{

	Motorcycle * tmpM =nullptr;
	if(servMotorQ.getcount())
		while (servMotorQ.getcount() && servMotorQ.getmax()->IsBack(timestep))
		{
			tmpM = servMotorQ.extractMax();
			tmpM->SetStatus(IDLE);
			tmpM->Changepriority(timestep);			
			ORD_TYPE motortype = tmpM->GetType(),ordertype = tmpM->getordtype();
					if(ordertype == TYPE_FROZ)
						wholeForders++;
					else
						if(ordertype == TYPE_NRM)
							wholeNorders++;
					else
						if(ordertype == TYPE_VIP)
							wholeVoreders++;

			if(tmpM->Isdamaged(timestep))
			{
				Damaged.enqueue(tmpM);
			}
			else
			{		
				if(motortype == TYPE_NRM)
				{
					N_MotorsCnt++;		
					if(tmpM->Istired(timestep))
						tirNmotorQ.enqueue(tmpM);
					else
						idelNMotorQ.insert(tmpM);
				}
				else
					if(motortype ==TYPE_FROZ)
					{
						F_MotorsCnt++;
					if(tmpM->Istired(timestep))
						tirFmotorQ.enqueue(tmpM);
					else
						idelFMotorQ.insert(tmpM);
					}
				else
				{
					V_MotorsCnt++;	
					if(tmpM->Istired(timestep))
						tirVmotorQ.enqueue(tmpM);
					else
						idelVMotorQ.insert(tmpM);
				}
			}
		}
		return servMotorQ.getcount();
}



////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to Pormote Normal orders              						  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////


void Region::Promote(int autop , int timestep)
{
	Order*const*Arrytmp =NOrderQueue.ToArray();
	Order *tmp=nullptr;
	int counter =NOrderCount;
	for (int i = 0; i < counter; i++)
	{
		if((timestep-Arrytmp[i]->GetArrTime())>=autop)
		{
			Order* VIP = new Order(Arrytmp[i]->GetArrTime() ,Arrytmp[i]->GetID(), TYPE_VIP, Arrytmp[i]->GetRegion(), Arrytmp[i]->GetDistance(), Arrytmp[i]->GetMoney());
			NOrderQueue.remove(1,tmp);
			delete tmp;
			InsertVOrder(VIP);
			NOrderCount--;
		}
		else 
			break;
	}
	delete [] Arrytmp;

}


////////////////////////////////////////////////////////////////////////////////////////////////
//																							  //
//					functions to print the assgined orders on the stats bar   				  //
//																							  //	
////////////////////////////////////////////////////////////////////////////////////////////////

void Region::AppendString(string& s, Order* O, Motorcycle* M)
{
	string action = "";
	ORD_TYPE motor = M->GetType();
	ORD_TYPE order = O->GetType();
	if(motor == TYPE_NRM)
		action += "N";
	else if(motor == TYPE_FROZ)
		action += "F";
	else 
		action += "V";
	action += to_string(M->GetID());
	action += "(";

	if(order == TYPE_NRM)
		action += "N";
	else if(order == TYPE_FROZ)
		action += "F";
	else 
		action += "V";
	action += to_string(O->GetID());

	action += ")";
	if( s.length() != 0 || s.length() != 1)
		s += "  ";
	s += action;
	s += " "; 
}



Region::~Region(void)
{
	Order*tmp;
	DeleteMotors();
	while (!DeletedOrderQueue.isEmpty())
	{
		 DeletedOrderQueue.enqueue(tmp);
		 delete tmp;
	}
}
