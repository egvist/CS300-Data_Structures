#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//To store (x,y), I created a struct named Point.
struct Point {
	int x;
	int y;
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	Point()
	{
		x = 0;
		y = 0;
	}

};

// This struct named Node represents city as a quadtree member. It contains Node pointer that show its directions (childrens) and position (x,y) as Point and also its city name.
struct Node
{
	string city_name;
	Point pos;

	Node* SW;
	Node* SE;
	Node* NW;
	Node* NE;


	Node()
	{
		pos = Point(0, 0);
		city_name = "";
		SW = NULL;
		SE = NULL;
		NW = NULL;
		NE = NULL;

	}
	Node(string c, int x, int y)
	{
		pos = Point(x, y);
		city_name = c;
		SW = NULL;
		SE = NULL;
		NW = NULL;
		NE = NULL;

	}


};

//
class Quadtree
{

	public:
	
		Node* root_tree;

		int edge_x, edge_y;

		
		Quadtree()
		{
			root_tree = NULL;
			edge_x = 0;
			edge_y = 0;
		}


		//Defines the quadtree's boundaries and makes it a kind of framework
		void edge(Point place)
		{
			edge_x = place.x;
			edge_y = place.y;
		}


		//Inserting insert_city to main quadtree
		void insert(Node* insert_city, Node* &quadtree)
		{
			
			if (quadtree == NULL)
			{
				quadtree = insert_city;
			}
			else
			{
				//Positional comparison to place insert_city and inserting recursively
				if (quadtree->pos.x > insert_city->pos.x)
				{
					if (quadtree->pos.y > insert_city->pos.y)
					{
						insert(insert_city, quadtree->SW);
					}
					else
					{
						insert(insert_city, quadtree->NW);
					}
				}
				else
				{
					if (quadtree->pos.y > insert_city->pos.y)
					{
						insert(insert_city, quadtree->SE);
					}
					else
					{
						insert(insert_city, quadtree->NE);
					}

				}



			}

		}

		//I created it according to given pretty_print algorithm.
		void pretty_print(Node* root, string place)
		{
			if (root != NULL)
			{
				/*cout << place << "(" << root->pos.x << "," << root->pos.y << ") -> " << root->city_name << endl;*/
				cout << root->city_name << endl;
			
				pretty_print(root->SE, "SE: ");
				pretty_print(root->SW, "SW: ");
				pretty_print(root->NE, "NE: ");
				pretty_print(root->NW, "NW: ");
			

			}
		}





		//Kind of a radar system. This function needs lots of comparison to work perfectly :)
		void track_q(int x, int y, int radius, Node* Quad, vector<string> &cities_on_radar, vector<string> &path_to)
		{
			// After reaching the end of a quadtree, this func ends but (because of recursive use) it continues where it left off.
			// Also according to homework document the searching sequence must be SE->SW->NE->NW. 
			if (Quad != NULL)
			{
				//It pushes city_name while going for the main tracking location. Therefore, Chicago is always in the path_to vector because it is the root of the Quadtree.
				path_to.push_back(Quad->city_name);

				//Checking whether (x,y) on the radar. If not it will be searched according to positional comparisons
				if (sqrt(pow((x - Quad->pos.x), 2) + pow((y - Quad->pos.y), 2)) <= radius)
				{
					cities_on_radar.push_back(Quad->city_name);
					track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
					track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
					track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
					track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);


				}

				//Positional comparisons to find which direction (x,y) located.
				else
				{
					//Creating radars with radius. For every conditions.
					if (x < Quad->pos.x)
					{
						
						if (Quad->pos.x > (x + radius))
						{
							if (Quad->pos.y > y)
							{
								// Locates on SW but with radius it exceeds to NW
								if (Quad->pos.y < (y + radius))
								{
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);

								}
								// Locates on SW. No exceeding
								else
								{
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
								}
							}
							else if (Quad->pos.y < y)
							{
								// Locates on NW but with radius it exceeds to SW
								if (Quad->pos.y > (y - radius))
								{
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);

								}
								// Locates on NW. No exceeding
								else
								{
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);
								}

							}
							// If on the line that is separating NW and SW
							else
							{
								track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
								track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);
							}
						}

						// Tracking close to East and West border.
						else if (Quad->pos.x <= (x + radius))
						{
							if (y > Quad->pos.y)
							{
								// Because of x+radius exceeds the East-West border and also y-radius exceeds North-South border
								if (Quad->pos.y > (y - radius))
								{
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);


								}
								// No exceeds North-South border. But still have East-West
								else
								{
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);

								}

							}
							else if (y < Quad->pos.y)
							{
								//Because of x + radius exceeds the East - West border and also y + radius exceeds North - South border
								if (Quad->pos.y < (y + radius))
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);

								}
								//// No exceeds North-South border. But still have East-West
								else
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);

								}

							}



						}



					}

					else
					{
						// Tracking away from the East-West border.
						if (Quad->pos.x < (x - radius))
						{
							if (Quad->pos.y > y)
							{
								// Tracking close to Norht-South border but mainly located on SE. Because of y+radius exceeds border, I have to search SE and NE.
								if (Quad->pos.y < (y + radius))
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
								}
								// No exceeding therefore only SE
								else
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
								}
							}
							else if (Quad->pos.y < y)
							{
								// Tracking close to Norht-South border but mainly located on NE. Because of y-radius exceeds border, I have to search SE and NE.
								if (Quad->pos.y > (y - radius))
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);

								}
								// No exceeding therefore only NE
								else
								{
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
								}
							}

							// If on the line that is separating NE and SE
							else
							{
								track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
								track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
							}

						}
						// Tracking close to the East-West border.
						else if (Quad->pos.x >= (x - radius))
						{
							if (Quad->pos.y > y)
							{
								// Tracking close to Norht-South border but mainly located on SE. Because of y+radius exceeds border, I also have to search NE and because of the x-radius exceeds East-West border SW.

								if (Quad->pos.y < (y + radius))
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
								}
								// Only x-radius exceeds so SE and SW.
								else
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->SW, cities_on_radar, path_to);
								}

							}
							else if (Quad->pos.y < y)
							{
								// Tracking close to Norht-South border but mainly located on NE. Because of y-radius exceeds border, I also have to search SE and because of the x-radius exceeds East-West border NW.
								if (Quad->pos.y > (y - radius))
								{
									track_q(x, y, radius, Quad->SE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);

								}
								// Only x-radius exceeds so NE and NW.
								else
								{
									track_q(x, y, radius, Quad->NE, cities_on_radar, path_to);
									track_q(x, y, radius, Quad->NW, cities_on_radar, path_to);
								}

							}


						}




					}





				}


			}
		}

		void query_track_and_print(int x, int y, int radius)
		{
			vector <string> cities_on_radar;
			vector <string> path_to;
			track_q(x, y, radius, root_tree, cities_on_radar, path_to);
			print_q(cities_on_radar);
			print_q(path_to);



		}



		void print_q(vector<string> &res_querry)
		{
			if (res_querry.size() == 0)
			{
				cout << "<None>" << endl;
			}
			else
			{
				for (int i = 0; i < res_querry.size(); i++)
				{
					if (i != res_querry.size() - 1)
					{
						cout << res_querry[i] << ", ";
					}
					else
					{
						cout << res_querry[i] << endl;
					}
				}
			}
		}


};





int main()
{	
	//Cities

	ifstream cities_file;
	cities_file.open("cities.txt");



	int x, y;

	string city_name;
	string data;



	getline(cities_file, data);
	istringstream first_iss(data);
	first_iss >> x >> y;

	Quadtree Quad;
	Point max(x, y);
	Quad.edge(max);

	while (!cities_file.eof())
	{
		getline(cities_file, data);
		istringstream iss(data);
		iss >> city_name >> x >> y;

		Node* city_location = new Node(city_name, x, y);

		//cout << "(" << city_location->pos.x << "," << city_location->pos.y << ") -> " << city_location->city_name << endl;

		if ((x < max.x && x > 0) && (y < max.y && y > 0))
		{
			Quad.insert(city_location, Quad.root_tree);
		}
	}


	Quad.pretty_print(Quad.root_tree, "Root: ");

	cities_file.close();


	//Querry

	ifstream querry_file;
	querry_file.open("queries0.txt");

	string data_q;

	int q_x, q_y;
	int radius;


	
	while (!querry_file.eof())
	{
		int i = 0;
		getline(querry_file, data_q);
		stringstream ss(data_q);
		vector<string> v;

		while (ss.good())
		{
			string sub_str;
			getline(ss, sub_str, ',');
			v.push_back(sub_str);

		}
		q_x = stoi(v[0]);
		q_y = stoi(v[1]);
		radius = stoi(v[2]);
		//cout << "(" << q_x << "," << q_y << ") -> " << radius << endl;
		cout << endl;
		Quad.query_track_and_print(q_x, q_y, radius);
		cout << endl;



	}


	querry_file.close();

}