#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>


using namespace std;

struct buildingPoint {
	buildingPoint(){}
	buildingPoint (int x_i, int y, bool iS, int l)
	{
			x = x_i;
			height = y;
			isStart = iS;
			label = l;
	}
	int x;
	int height;
	int label;
	bool isStart; // Is it a start or end point
};

class MPQ // Modified Priority Queue Class
{
public:

	struct MPQobj
	{
		MPQobj ()
		{
		
		}

		MPQobj (int v, int l)
		{
			value = v; 
			label = l;
		}
		int value;
		int label;
	};

	MPQ(const int pqSize)
	{
		heap = new MPQobj[pqSize];
		Location = new int[pqSize];
		PQ_SIZE= pqSize; 
		currentSize = 0;
	}



	void insert(int x, int label)
	{
		currentSize++;
		int hole = currentSize;

		while (hole > 1 && x > heap[hole/2].value) // Max heap
		{ 
			heap[hole] = heap[hole/2]; // Parents goes to down in the tree
			Location[heap[hole/2].label] = hole; // Location information of the ex parent is now downside of the tree
			hole = hole / 2;
		}

		heap[hole].value = x;
		heap[hole].label = label;	
		Location[label] = hole;

	}

	int GetMax ()
	{
		if (!IsEmpty())
		{ 
			return heap[1].value; 
		}
		else 
		{ 
			return -1; 
		}
	}


	// Similar to lecture percolate down method
	void percolateDown(int p)
	{
		int child;
		int tmp = heap[p].value;
		int tmplabel = heap[p].label;

		while (p*2 <= currentSize)
		{
			child = p*2;

			if (child != currentSize && heap[child+1].value > heap[child].value) // Finding biggest child
			{ 
				child++; 
			}	
			
			if (heap[child].value > tmp) // If it is bigger than p's value
			{ 
				heap[p] = heap[child]; // Getting child to up because it is bigger 
				Location[heap[child].label] = p; // Updating child's location
			
			}
			else
			{ 
				break; 
			}

			p = child; // Lowering the p (replace with ex-child)
		}

		heap[p].value = tmp; 
		heap[p].label =  tmplabel;
		Location[tmplabel] = p; // Updating location information
	}

	int Remove(int label)
	{
		int label_location = Location[label]; // Getting location of the requested height value in the heap
		int tmp = heap[label_location].value;

		heap[label_location] = heap[currentSize--]; // Getting the last child to the removed position
		
		Location[heap[currentSize+1].label] = label_location; // Updating the child's location

        percolateDown(label_location);

		max_value = heap[1].value;
		return tmp;
	}

	//Just checking for the current size to determine whether MPQ empty or not
	bool IsEmpty() 
	{ 
		return (currentSize == 0);  
	}

	//Destructor
	~MPQ ()
	{
		delete [] Location;
	}

private:

	int PQ_SIZE;
	int currentSize;
	MPQobj * heap;
	int max_value;
	int * Location;

};


// Simple comparisons in (operator <) to determine which building will be the next
bool operator<(const buildingPoint & lhs, const buildingPoint &rhs) {
    
	if (lhs.x < rhs.x)
	{ 
		return true; 
	}

	if (lhs.x == rhs.x)
	{		
		if (lhs.isStart && rhs.isStart) // if two STARTs are compared then higher height building should be the first
		{ 
			return (lhs.height > rhs.height); 
		}

		if (!lhs.isStart && !rhs.isStart) // if two ENDs are compared then lower height building should be the first
		{ 
			return (lhs.height < rhs.height); 
		}
		
		return lhs.isStart;  // if one STARTS and END compared, then START should be the first
	}

		return false;
}

int main ()
{
	ifstream input("input.txt"); 

	while(!input.is_open())
	{
		cout << "Error: unable to open input file\n";
		return 1;

	}

	int buildingNumber;
	int x1, y, x2;
	
	//The first line determine how many building do we have
	input >> buildingNumber;

	int pointSize = (buildingNumber*2)+1; // There is a +1 because of the initial 0 0 no building point.
	// It will be mulptiplied by 2 because there are 2 times of start and end points
	buildingPoint * buildingPoints = new buildingPoint [pointSize]; 
	buildingPoint * mBuildingPoint;
	int label, height, prevmax;

	mBuildingPoint = new buildingPoint(0, 0, true, 0); // Starting point
	buildingPoints[0] = *mBuildingPoint;

	int bpIndex = 1; // It will start from 1 because there is a 0 0 point in the first index

	// Putting into buildingPoints array
	for (int i = 0; i < buildingNumber; i++)
	{
		input >> x1 >> y >> x2; // Taking x1, height and x2 values
		label = bpIndex; // Identifying number for that building

		mBuildingPoint = new buildingPoint(x1, y, true, label); // Starting point
		buildingPoints[bpIndex] = *mBuildingPoint;

		mBuildingPoint = new buildingPoint(x2, y, false, label); // End point
		buildingPoints[bpIndex+1] = *mBuildingPoint;

		bpIndex = bpIndex + 2;
	}


	sort(buildingPoints, buildingPoints+pointSize); // Sorting of the array

	MPQ mpq(pointSize);

	for (int i = 0; i < pointSize; i++)
	{

		if (buildingPoints[i].isStart)
		{
			height = buildingPoints[i].height;
			prevmax = mpq.GetMax();
			mpq.insert(height, buildingPoints[i].label);		

			if(prevmax != mpq.GetMax()) // If max of the priority queue changes output that building 
			{ cout << buildingPoints[i].x << " " << height << endl;	}
		}

		else 
		{
		
			height = buildingPoints[i].height;
			prevmax = mpq.GetMax();
			mpq.Remove(buildingPoints[i].label);

			if(mpq.GetMax() != prevmax) // If max of the priority queue changes output that building 
			{ cout << buildingPoints[i].x << " " << mpq.GetMax() << endl; }
			
		}

	}
	
	input.close();
	system("pause");

	return 0;
}