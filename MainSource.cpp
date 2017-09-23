//////library
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "serial2.h" //using My serial library
using namespace std;

/////library


// Do some basic settings
//0.
//////DATA
int sizeInGraph = 17; //It is num of point + 1, and mean that how many size of point
char** whereConnectToPoint = new char*[sizeInGraph]; //It is array that  connect of point to point
int**lengthPointToPoint = new int*[sizeInGraph]; //It mean that the length of point to point
int* counterOfPoint = new int[sizeInGraph]; // It mean that the num of one point to all point
int* exitGoing = new int[sizeInGraph]; //index of exiting
int mode = 3;//It is for change to showing root of one point or all point
bool check = false; //It is for checking the fact of we have root with that point
string root1 = "";
string knowing1 = "";


vector<int> caseOfGraph;
struct point {
	point* nextaddress;
	point** nextaddress2; //the points of current point are pluarity
	int value = 0; // the length of point to point
	char tag; // the point's name of current point
	list<char> line; // it's for root
	list<char>::iterator itor = line.begin();
};



point *Super; //It is for the shortest root
//////DATA
void continuing(int N, char alpa, char beta); // It will be used in MAIN
void analysis1(string knowing)
{
	int j = 0;
	int find = knowing.find("#"); //in this time, knowing mena that arduino' demand
	knowing = knowing.substr(find+1, find + 8);
	int find2 = knowing.find("#");
  knowing = knowing.substr(0,find2);
	cout << "analysis1 start:" << endl;
	int size = knowing.length();
	int n,alpa = '@' ,beta = '@';

	if(size <= 0)
	return;

  else{
    for(int i = 0; i < size ; i++)
    {
			if(knowing[i] != ' ')
			{
				switch(i)
				{
					case 0:
						n = (int)knowing[i] - 48;
					  break;

					case 2:
						alpa = knowing[i];
						break;

					case 4:
							beta = knowing[i];
							break;
				}
			}
    }

}

if(n > 8 || n < 1)
return;

else
{
cout << n;
continuing(n,alpa,beta); //go in mode
}
}

//1.
//////////MODIFY GRAPH///////////////////////////
void addingValueInGraph_(int index, char alpa)  //alpa is mean that where to go(point by point) and beta mean that what is that length
{
	if (counterOfPoint[index] < sizeInGraph) {
		whereConnectToPoint[index][counterOfPoint[index]] = alpa;
		lengthPointToPoint[index][counterOfPoint[index]] = 1;
		counterOfPoint[index]++;
	}
}

void addingValueInGraph_(char alpa, char beta, int length)  //alpa is mean that where to go(point by point) and beta mean that what is that length
{
	//length is  mean that length of alpa to beta
	//connect beta to alpa

	if (alpa == '@' || beta == '@')
	{
		if (alpa == '@' && beta == '@')
		{
			cout << "error, Invaild\n";
			return;
		}

		if (alpa == '@' && counterOfPoint[(int)(beta - 'A')] < sizeInGraph)
		{
			whereConnectToPoint[(int)(beta - 'A')][counterOfPoint[(int)(beta - 'A')]] = '@';
			exitGoing[(int)(beta - 'A')] = counterOfPoint[(int)(beta - 'A')];
			lengthPointToPoint[(int)(beta - 'A')][counterOfPoint[(int)(beta - 'A')]] = length;
			counterOfPoint[(int)(beta - 'A')]++;
		}

		if (beta == '@' && counterOfPoint[(int)(alpa - 'A')] < sizeInGraph)
		{
			whereConnectToPoint[(int)(alpa - 'A')][counterOfPoint[(int)(alpa - 'A')]] = '@';
			exitGoing[(int)(alpa - 'A')] = counterOfPoint[(int)(alpa - 'A')];
			lengthPointToPoint[(int)(alpa - 'A')][counterOfPoint[(int)(alpa - 'A')]] = length;
			counterOfPoint[(int)(alpa - 'A')]++;
		}

		else
		{
			cout << "error\n";
			return;
		}
	}


	else {
		int index = (int)(alpa - 'A'); //It mean that A->0 , B->1 likely that

		if (counterOfPoint[index] < sizeInGraph) {
			whereConnectToPoint[index][counterOfPoint[index]] = beta;
			lengthPointToPoint[index][counterOfPoint[index]] = length;
			counterOfPoint[index]++;
		}


		//connect alpa to beta
		index = (int)(beta - 'A');
		if (counterOfPoint[index] < sizeInGraph) {
			whereConnectToPoint[index][counterOfPoint[index]] = alpa;
			lengthPointToPoint[index][counterOfPoint[index]] = length;
			counterOfPoint[index]++;
		}
	}
}

void disConnectValueInGraph_(char alpa, char beta) //It is for situration with fire, and mean that can't not use the root
{
	bool starting = false;
	int j = 0;

	//disconnect beta to alpa
	int index = (int)(beta - 'A');
	for (int i = 0; i < counterOfPoint[index]; i++)
	{
		if (starting == true && i > 0)
		{
			whereConnectToPoint[index][j] = whereConnectToPoint[index][i];
			lengthPointToPoint[index][j] = lengthPointToPoint[index][i];
			j++;
		}

		if (whereConnectToPoint[index][i] == alpa) //it is similarily to shift
		{
			j = i;
			starting = true;
		}
	}
	counterOfPoint[index]--;


	//disconnect alpa to beta
	starting = false;
	j = 0;

	index = (int)(alpa - 'A');
	for (int i = 0; i < counterOfPoint[index]; i++)
	{
		if (starting == true && i > 0)
		{
			whereConnectToPoint[index][j] = whereConnectToPoint[index][i];
			lengthPointToPoint[index][j] = lengthPointToPoint[index][i];
			j++;
		}

		if (whereConnectToPoint[index][i] == beta)
		{
			j = i;
			starting = true;
		}
	}
	counterOfPoint[index]--;
}
//////////MODIFY GRAPH///////////////////////////


//2.
//////FUNCTION START, AND MAKING AND SETTING GRAPH ITSELF///////
void making_gettingValueInGraph_() //It's for reset map
{
	whereConnectToPoint = new char*[sizeInGraph]; //It is array that  connect of point to point
	lengthPointToPoint = new int*[sizeInGraph]; //It mean that the length of point to point
	counterOfPoint = new int[sizeInGraph]; // It mean that the num of one point to all point
	mode = 3;//It is for change to showing root of one point or all point
	check = false; //It is for checking the fact of we have root with that point
	exitGoing = new int[sizeInGraph];

	caseOfGraph.assign(sizeInGraph, 0); //I will make CaseOfGraph for root

	for (int i = 0; i < sizeInGraph; i++)
	{
		whereConnectToPoint[i] = new char[sizeInGraph];
		lengthPointToPoint[i] = new int[sizeInGraph];
		counterOfPoint[i] = 0;
		exitGoing[i] = -1;
	}

	//exit mean that '@' , under content is original map
	addingValueInGraph_(0, 'D');
	addingValueInGraph_(0, 'H');
	addingValueInGraph_(0, 'E');
	addingValueInGraph_(0, '@'); // '0' mean that node 'A'
	exitGoing[0] = 3;


	addingValueInGraph_(1, 'F');
	addingValueInGraph_(1, 'C'); // '1' mean that node 'B'

	addingValueInGraph_(2, 'B');
	addingValueInGraph_(2, 'P'); // '2' mean that node 'C'

	addingValueInGraph_(3, 'A');
	addingValueInGraph_(3, 'G'); // '3' mean that node 'D'

	addingValueInGraph_(4, 'A');
	addingValueInGraph_(4, 'F');
	addingValueInGraph_(4, 'I'); // '4' mean that node 'E'

	addingValueInGraph_(5, 'B');
	addingValueInGraph_(5, 'E');
	addingValueInGraph_(5, 'P'); // '5' mean that node 'F'

	addingValueInGraph_(6, 'J');
	addingValueInGraph_(6, 'H');
	addingValueInGraph_(6, 'D'); // '6' mean that node 'G'

	addingValueInGraph_(7, 'A');
	addingValueInGraph_(7, 'G'); // '7' mean that node 'H'

	addingValueInGraph_(8, 'P');
	addingValueInGraph_(8, 'L');
	addingValueInGraph_(8, 'E');
	addingValueInGraph_(8, '@'); // '8' mean that node 'I'
	exitGoing[8] = 3;

	addingValueInGraph_(9, 'M');
	addingValueInGraph_(9, 'K');
	addingValueInGraph_(9, 'G'); // '9' mean that node 'J'

	addingValueInGraph_(10, 'N');
	addingValueInGraph_(10, 'J');
	addingValueInGraph_(10, 'L'); // '10' mean that node 'K'

	addingValueInGraph_(11, 'O');
	addingValueInGraph_(11, 'K');
	addingValueInGraph_(11, 'I'); // '11' mean that node 'L'

	addingValueInGraph_(12, 'N');
	addingValueInGraph_(12, 'J');
	addingValueInGraph_(12, '@'); // '12' mean that node 'M'
	exitGoing[12] = 2;

	addingValueInGraph_(13, 'M');
	addingValueInGraph_(13, 'K');
	addingValueInGraph_(13, 'O');
	addingValueInGraph_(13, '@'); // '13' mean that node 'N'
	exitGoing[13] = 3;

	addingValueInGraph_(14, 'N');
	addingValueInGraph_(14, 'L');
	// '14' mean that node 'O'

	addingValueInGraph_(15, 'C');
	addingValueInGraph_(15, 'F');
	addingValueInGraph_(15, 'I');// '15' mean that node 'P'
								 //////////////////////////
}


point* settingValueInGraph(point *alpa, char next, int length)  //lINKED LIST'S INFORMATION SETTING
{
	point* forReturn = alpa;
	(*forReturn).tag = next;
	(*forReturn).value = length;
	forReturn = (*forReturn).nextaddress;

	return forReturn;
}

vector<int> checkingExit(vector<int> checkInGraph) //SETTING TO CHECK POINT TO CONNECTED EXIT
{
	for (int i = 0; i < sizeInGraph; i++)
	{
		if (exitGoing[i] > -1)
		{
			checkInGraph[i] = 2;
		}
	}

	return checkInGraph;
}

void settingSuperRoot(point *link)
{
	if (Super == 0)
	{
		Super = link;
		(*Super).value = (*link).value;
	}

	else
	{
		int value = (*link).value;
		if ((*Super).value > value)
		{
			Super = link;
			(*Super).value = (*link).value;
		}
	}
}


void showingLinkedList(point *link);
void toMode_(point* link)
{
	if (mode == 3)
		showingLinkedList(link);  //IN MODE 3, YOU CAN SEE ALL ROOT FROM CURRETN POINT

	else
	{
		settingSuperRoot(link); //IN MODE 4, YOU CAN SEE THE SHORTEST ROOT FROM CURRENT POINT
	}
}

void goingToCalculate(point* forGoing, char alpa, int nodeLength)
{
	forGoing = settingValueInGraph(forGoing, alpa, nodeLength);
}


//////FUNCTION START, AND MAKING GRAPH ITSELF///////



////3.SHOWING//////
void showOfTheGraph_(int index) //It is for check what is points connected with current point
{
	for (int i = 0; i < counterOfPoint[index]; i++)
	{
		char a = whereConnectToPoint[index][i];
		cout << a << " ";
	}
	cout << endl;
}

void showOfAllGraph_() //It is for check releation all point
{
	for (int i = 0; i < sizeInGraph; i++)
	{
		cout << (char)(i + 65) << " : ";
		showOfTheGraph_(i);
	}
}

void showingLinkedList(point *link) //SHOWING LINKED LIST' ROOT
{
	check = true;
	int value = (*link).value;
	cout << "showing start" << endl;
	cout << "root value is " << value << endl;

	cout << "root is : ";
	for (list<char>::iterator i = (*link).line.begin(); i != (*link).line.end(); i++)
	{
		cout << *i << " ";
	}

	cout << endl;
	//cout << "@" << endl << endl;
}

void showingSuper() //showing the shortest ROOT
{
	root1 = "#";
	int value = (*Super).value;
	cout << "showing start" << endl;
	cout << "root value is " << value << endl;

	cout << "root is : ";
	for (list<char>::iterator i = (*Super).line.begin(); i != (*Super).line.end(); i++)
	{
		cout << *i << " ";
		root1 += *i;
		root1 += " ";
	}

	root1 += "#";
	cout << endl;
}

void showingNoFinding() // It will be shown in no ROOT case
{
	if (check == false)
		cout << "no Root Finding : -1" << endl;
}

///////3.SHOWING//////

////4.LINKED LIST/////
point* makingLinkedList() //It's for making linked list, In other way, preparation
{
	point *sample1 = new point;
	point *forreturn = sample1;

	for (int i = 0; i < sizeInGraph; i++)
	{
		(*sample1).nextaddress = new point;
		(*sample1).value = 0;
		sample1 = (*sample1).nextaddress;
	}
	return forreturn;
}

//////FOR LINKED LIST'S DATA/////
point* showing[1000];
int pointerCount = 0;
//////FOR LINKED LIST'S DATA/////

/////LINKED LIST//////


//////////////5.Calculating///////////////

void calculatingGraph3(point* forgoing, int alpa, vector<int> checkInHere);
void calculatingGraph2(char beta)
{

	try {
		check = false;
		int alpa = (int)(beta - 'A');
		int linkedListCount = 0;

		vector<int> checkInHere;
		checkInHere.assign(sizeInGraph * 10, 0);
		//checkInHere = checkingExit(checkInHere);

		if (counterOfPoint[alpa] > 0)
		{
			checkInHere[alpa]++;
			showing[pointerCount] = new point;
			(*showing[pointerCount]).tag = (char)(alpa + 'A');
			(*showing[pointerCount]).line.push_back((*showing[pointerCount]).tag);
			(*showing[pointerCount]).nextaddress2 = new point*[counterOfPoint[alpa]];



			for (int i = 0; i < counterOfPoint[alpa]; i++)
			{
				//cout << whereConnectToPoint[alpa][i] << endl << endl;

				if (whereConnectToPoint[alpa][i] == '@')
				{
					((*showing[pointerCount]).nextaddress2[linkedListCount]) = new point;
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).value = (*showing[pointerCount]).value + lengthPointToPoint[alpa][i];
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).tag = whereConnectToPoint[alpa][i];
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).line.push_back((*showing[pointerCount]).tag);
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).line.push_back(whereConnectToPoint[alpa][i]);
					check = true;
					toMode_(((*showing[pointerCount]).nextaddress2[linkedListCount]));
				}

				else if ((*showing[pointerCount]).value < sizeInGraph && (checkInHere[(int)(whereConnectToPoint[alpa][i] - 'A')] == 0))
				{
					((*showing[pointerCount]).nextaddress2[linkedListCount]) = new point;
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).value = (*showing[pointerCount]).value + lengthPointToPoint[alpa][i];
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).tag = whereConnectToPoint[alpa][i];
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).line.push_back((*showing[pointerCount]).tag);
					(*(*showing[pointerCount]).nextaddress2[linkedListCount]).line.push_back(whereConnectToPoint[alpa][i]);
					checkInHere[(int)(whereConnectToPoint[alpa][i] - 'A')]++;
					calculatingGraph3((((*showing[pointerCount]).nextaddress2[linkedListCount])), (*(*showing[pointerCount]).nextaddress2[linkedListCount]).tag - 'A', checkInHere);
					linkedListCount++;
				}
			}
		}
	}
	catch (int i)
	{
		cerr << "error";
	}
}

void calculatingGraph3(point* forgoing, int alpa, vector<int> checkInHere)
{
	try {
		int linkedListCount = 0;
		bool checkCounter = true;

		if (checkInHere[alpa] >= 2)
		{
			toMode_(showing[pointerCount]);
			return;
		}


		if (counterOfPoint[alpa] > 0 && (*forgoing).value < sizeInGraph)
		{
			(*forgoing).nextaddress2 = new point*[counterOfPoint[alpa]];


			for (int i = 0; i < counterOfPoint[alpa]; i++)
			{
				checkCounter = true;

				if (whereConnectToPoint[alpa][i] == '@')
				{
					(*forgoing).nextaddress2[linkedListCount] = new point;
					(*(*forgoing).nextaddress2[linkedListCount]).value = (*forgoing).value + lengthPointToPoint[alpa][i];
					(*(*forgoing).nextaddress2[linkedListCount]).tag = whereConnectToPoint[alpa][i];
					(*(*forgoing).nextaddress2[linkedListCount]).line = (*forgoing).line;
					(*(*forgoing).nextaddress2[linkedListCount]).line.push_back(whereConnectToPoint[alpa][i]);
					check = true;
					toMode_(((*forgoing).nextaddress2)[linkedListCount]);
					checkCounter = false;
					linkedListCount++;
				}


				else if ((*forgoing).value < sizeInGraph && checkInHere[(int)(whereConnectToPoint[alpa][i] - 'A')] == 0 && checkCounter)
				{
					(*forgoing).nextaddress2[linkedListCount] = new point;
					(*(*forgoing).nextaddress2[linkedListCount]).value = (*forgoing).value + lengthPointToPoint[alpa][i];
					(*(*forgoing).nextaddress2[linkedListCount]).tag = whereConnectToPoint[alpa][i];
					(*(*forgoing).nextaddress2[linkedListCount]).line = (*forgoing).line;
					(*(*forgoing).nextaddress2[linkedListCount]).line.push_back(whereConnectToPoint[alpa][i]);
					checkInHere[(int)(whereConnectToPoint[alpa][i] - 'A')]++;
					calculatingGraph3((*forgoing).nextaddress2[linkedListCount], (int)(whereConnectToPoint[alpa][i] - 'A'), checkInHere);
					linkedListCount++;
				}
			}
		}


		else {
			return;
		}
	}
	catch (int i)
	{
		cerr << "error";
	}
}
//////////////5.Calculating///////////////




////6.MAIN/////

void continuing(int N) // It will be used in MAIN
{


	int length = 1;

	switch (N)
	{
	case 1://adding root(on map changing)
		cout << "input start point, end point, and length ";
		char alpa, beta;

		cin >> alpa >> beta >> length;
		addingValueInGraph_(alpa, beta, length);
		break;


	case 2: //delete root(situlation on fire)
		cout << "input start point, end point ";
		cin >> alpa >> beta;
		disConnectValueInGraph_(alpa, beta);
		break;

	case 3: //show all root
		cout << "input starting point" << endl;
		cin >> alpa;
		mode = N;
		calculatingGraph2(alpa);
		showingNoFinding();
		break;

	case 4: //show only the shortest root
		Super = 0;
		cout << "input starting point(Super)" << endl;
		cin >> alpa;
		mode = N;
		calculatingGraph2(alpa);
		showingSuper();
		break;

	case 5: //show all root of all point
		mode = 3;
		alpa = 'A';
		for (int i = 0; i < sizeInGraph - 1; i++)
		{
			calculatingGraph2(alpa);
			alpa += 1;
			cout << endl;
		}
		break;


	case 6: //show  only the shortest root
		mode = 4;
		alpa = 'A';

		for (int i = 0; i < sizeInGraph - 1; i++)
		{
			Super = 0;
			calculatingGraph2(alpa);
			showingSuper();
			alpa += 1;
			cout << endl;
		}
		break;


	case 7:
		making_gettingValueInGraph_();
		break;

	case 8:
	Super = 0;
	cout << "input starting point(Super)" << endl;
	//cin >> alpa;
	mode = 4;
	calculatingGraph2('B');
	showingSuper();

	cout << "sending this" << root1 << endl;

	Dataline[0] = root1; //root1 is string, and minmum root
	writting(0); //writting start
	analysis1(knowing);
	break;

	case 9:
	reading(0);
	analysis1(knowing);
	break;


	default: //exit program
		return;
		break;
	}

	cout << endl;
}

void continuing(int N, char alpa, char beta) // It menu for arduino
{
	int length = 1;

	switch (N)
	{
	case 1://adding root(on map changing)
		cout << "input start point, end point, and length ";
		//char alpa, beta;

		//cin >> alpa >> beta >> length;
		addingValueInGraph_(alpa, beta, length);
		break;


	case 2: //delete root(situlation on fire)
		cout << "input start point, end point ";
		//cin >> alpa >> beta;
		disConnectValueInGraph_(alpa, beta);
		break;

	case 3: //show all root
		cout << "input starting point" << endl;
		//cin >> alpa;
		mode = N;
		calculatingGraph2(alpa);
		showingNoFinding();
		break;

	case 4: //show only the shortest root
		Super = 0;
		cout << "input starting point(Super)" << endl;
		//cin >> alpa;
		mode = N;
		calculatingGraph2(alpa);
		showingSuper();
		Dataline[0] = root1;
		cout << "sending this" << root1 << endl;
		writting(0);
		analysis1(knowing);
		//reading(0);
		break;

	case 5: //show all root of all point
		mode = 3;
		alpa = 'A';
		for (int i = 0; i < sizeInGraph - 1; i++)
		{
			calculatingGraph2(alpa);
			alpa += 1;
			cout << endl;
		}
		break;


	case 6: //show  only the shortest root
		mode = 4;
		alpa = 'A';

		for (int i = 0; i < sizeInGraph - 1; i++)
		{
			Super = 0;
			calculatingGraph2(alpa);
			showingSuper();
			alpa += 1;
			cout << endl;
		}
		break;


	case 7:
		making_gettingValueInGraph_();
		break;

	case 8:
	Super = 0;
	cout << "input starting point(Super)" << endl;
	//cin >> alpa;
	mode = 4;
	calculatingGraph2('D');
	showingSuper();
	cout << "sending this" << root1 << endl;

	Dataline[0] = root1;
	writting(0);
	//reading(0);
	analysis1(knowing);
	//closeDevice(0);
	break;

	default: //exit program
		return;
		break;
	}

	cout << endl;
}

int main(void)
{
	cout << endl << endl;
	int N = 0;
	making_gettingValueInGraph_();
	int count = 0;

	while (1)
	{
		openingSerial();
		terminal_connect();
		cout << "the point's num is " << sizeInGraph - 1 << endl;
		cout << "input 1 is for using adding node\n";
		cout << "input 2 is for using disconnect node\n";
		cout << "input 3 checking all root of selected point\n";
		cout << "input 4 checking the shortest root of selected point\n";
		cout << "input 5 checking all root of all point\n";
		cout << "input 6 checking the shortest root of all point\n";
		cout << "input 7 reset all graph\n else is out in this program";
		cout << "input 8 using serial connection using this algorithm";
		cout << "input 9 reading Serial" << endl;

		if(count <= 1)
		{
			reading(0);
			analysis1(knowing);
			count++;
		}

		else
		{
		showOfAllGraph_();
		cout << "Input : ";
		cin >> N;

		if (N <= 9)
			continuing(N);
		else
			return 0;
		}
	}
}

/////6,MAIN/////
