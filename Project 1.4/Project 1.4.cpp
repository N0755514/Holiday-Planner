// Project1.4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream> 
#include <string>


using namespace std;

//Declaration of functions not uses in classes

double calcPrice(double budget, double rating);
double calcPriceLength(double destLength, double chosenHotelPrice);
double flights(int destRatingNew);
double upgradedFlights(int flights);

class Destination //Destination Class
{
public:
	Destination(string dName, int dRating);
	Destination();
	void display();
	bool getDestination(string userInput);
	string getDestinationName();
	int getRating();

private:
	string DestinationName;
	double DestinationRating;
	friend class Hotel; //allows hotel to access private members
};

class Hotel//Hotel Class 
{
public:
	Hotel(string hName, int hPrice);
	Hotel();
	void hdisplay();
	bool canAfford(int budget, int maxRealPriceNew);
	string getName();
	double getDestinationRating(Destination DestinationRating);
	double getPrice();
	void setPrice(double realPrice);
	bool getHotel(string userInput, int hotelPrice, int budget);
	double getRealPrice();

private:
	string HotelName;
	int HotelPrice;
	int Budget;
	int RealPrice;

};



Hotel::Hotel()
{

};
//Contructor

Destination::Destination()
{

};
//Constructor

//Main

int main()
{
	const int numHotels = 5;

	Destination destinationlist[3] = { { "Paris",80 }, //Contructing Array
	{ "Rome",85 },
	{ "Glasgow",40 }
	};

	Hotel hotellist[numHotels] = { Hotel("Hostel", 200),
		Hotel("Motel",600),
		Hotel("PremierInn",1000),
		Hotel("Hilton",3000),
		Hotel("FourSeasons",7500)
	};


	//Destination

	cout << "Destination    Rating\n" << endl;

	for (int i = 0; i<3; i++)
	{
		destinationlist[i].display(); //Displaying each destination
	}

	cout << "\n\nPlease type in your chosen destination" << endl; //Prompt
	string ChosenDest; //User chosen destination
	cin >> ChosenDest;
	bool Success = false;
	Destination destratingObj;
	Hotel destRatingObj;
	int maxRating = 0;
	string destName;
	while (Success == false)
	{
		for (int i = 0; i < 3; i++) //Displaying each line
		{
			Success = destinationlist[i].getDestination(ChosenDest); //checks valid input
			destName = destinationlist[i].getDestinationName();
			for (int i = 0; i < 3; i++)
			{
				if (maxRating < (destinationlist[i].getRating())) //finds max rating of all destinations
				{
					int y = destinationlist[i].getRating();
					maxRating = y;
				}
			}
			//interating through array
			if (Success == true)									// and checking against user 
			{														//input, using getDestination
				destratingObj = destinationlist[i];
				//breaks as soon as it finds a match
				break;
			}
			else if ((Success == false) && (destName == "Glasgow")) //checks end of array
			{														//WHEN LOADING FILE NEEDS TO CHECK LAST DESTINATION
				cout << "Please enter a valid Destination" << endl;
				cin >> ChosenDest;
			}
		}
	}

	int destRating = 0;
	double hotelPrice;
	destRating = destRatingObj.getDestinationRating(destratingObj); //obtaining destination ratin
	double realPrice;

	//Printing Hotels

	cout << "\nHotel		Price\n" << endl;
	for (int i = 0; i<5; i++)
	{
		hotelPrice = hotellist[i].getPrice();
		realPrice = calcPrice(hotelPrice, destRating);
		hotellist[i].setPrice(realPrice);
		hotellist[i].hdisplay(); //Displaying each hotel
	}

	int maxRealPrice; //realPrice indicates hotels price based on destination
	int maxPrice;

	maxPrice = hotellist[4].getPrice();
	maxRealPrice = calcPrice(maxPrice, destRating);

	//Budget //BREAKS IF YOU ENTER STRING

	double budget;
	string hotelname;
	double holidayTotal;
	cout << "\nPlease enter your budget for the holiday. \n Bare in mind, optional extras are still available so dont spend all \n your money on the best hotel." << endl;
	cin >> budget;
	while (budget <= 0)
	{
		cout << "Please enter a valid budget" << endl;
		cin >> budget;
	}
	holidayTotal = budget;
	cout << "Your budget is " << budget << endl;

	//Affordable Hotels

	cout << "These are the hotels within your budget!\n" << endl;
	bool canAfford = false;
	while (canAfford == false)
	{
		for (int i = 0; i < 5;i++)
		{
			canAfford = hotellist[i].canAfford(budget, maxRealPrice);
			hotelname = hotellist[i].getName();

			if ((canAfford == true) && (hotelname == "Four Seaons")) //to stop  looping forever
			{
				break;
			}
		}
	}

	//Choosing a Hotel

	cout << "Please choose the Hotel you would like to stay in" << endl;
	string chosenHotel;
	cin >> chosenHotel;
	int chosenHotelPrice;
	bool checkHotel = false;
	while (checkHotel == false)
	{
		for (int i = 0; i < 5; i++)
		{
			hotelPrice = hotellist[i].getRealPrice();
			checkHotel = hotellist[i].getHotel(chosenHotel, hotelPrice, budget);
			hotelname = hotellist[i].getName();
			if (checkHotel == true)
			{
				chosenHotelPrice = hotellist[i].getRealPrice();
				break;
			}
			else if ((checkHotel == false) && (hotelname == "FourSeasons")) //chechking  valid input 
			{
				cout << "Please enter a valid Hotel" << endl;
				cin >> chosenHotel;
			}
		}
		//ABLE TO CHOSE HOTEL YOU CANT AFFORD // Fixed
	}

	//Holiday Length

	cout << "How many weeks would you like to stay at your destination for? \n This will of course effect the price of your hotel." << endl;
	double destLength;
	double runningPrice;
	bool validInputHotel = false;
	cin >> destLength; //ABLE TO PICK -5 WEEKS //Fixed
	while (validInputHotel == false)
	{
		if (destLength > 0)
		{
			runningPrice = calcPriceLength(destLength, chosenHotelPrice);
			holidayTotal = holidayTotal - runningPrice;
			validInputHotel = true;
		}
		else
		{
			cout << "Please enter a valid number" << endl;
			cin >> destLength;
		}
	}

	while (holidayTotal < 0)
	{
		cout << "You dont have enough budget for this!" << endl;
		holidayTotal = budget;
		cin >> destLength;
		runningPrice = calcPriceLength(destLength, chosenHotelPrice);
		holidayTotal = holidayTotal - runningPrice;
	}

	//Flights

	double flightsPrice = flights(destRating);
	cout << "You have " << holidayTotal << " remaining" << endl;
	cout << "Would you like to upgrade your flights?" << endl;
	cout << "Upgrading your flights will cost " << upgradedFlights(flightsPrice) << endl;
	cout << "\n Yes or No?" << endl;
	string choiceFlights;
	cin >> choiceFlights;
	bool validInputFlights = false;
	string upgradedFlightsStr;
	while ((validInputFlights == false) && (holidayTotal > 0))
	{
		if (choiceFlights == "Yes")
		{
			holidayTotal = holidayTotal - upgradedFlights(flightsPrice); //adding price of upgraded flights
			cout << "You have " << holidayTotal << " remaining" << endl;
			validInputFlights = true;
			upgradedFlightsStr = "Yes";
			break;
		}
		else if (choiceFlights == "No")
		{
			validInputFlights = true;
			upgradedFlightsStr = "No";
		}
		else
		{
			cout << "\nYou may not have enough money for that!\nEnter another response\n" << endl;
			holidayTotal = holidayTotal + upgradedFlights(flightsPrice);
			cout << "You have " << holidayTotal << " remaining" << endl;
			upgradedFlightsStr = "No";
			cin >> choiceFlights;
		}


	}

	//FINAL PACKAGE 
	//Could make into a packagedisplay() function?

	cout << "\n\nYour final Holiday Package!" << endl;
	cout << "Destination : " << destName << endl;
	cout << "Hotel : " << chosenHotel << endl;
	cout << "Holiday Length: " << destLength << " weeks" << endl;
	cout << "Upgraded flights? : " << upgradedFlightsStr << endl;
	cout << "Price : " << budget - holidayTotal << " (Budget : " << budget << ")" << endl;

	//Would you like to save your package?
	//Would you like to make plan a new holiday?



	string exit;
	cin >> exit;
	return 0;
}

Destination::Destination(string dName, int dRating) //contructor
{
	DestinationName = dName;
	DestinationRating = dRating;
}

Hotel::Hotel(string hName, int hPrice) //contructor
{
	HotelName = hName;
	HotelPrice = hPrice;

}

int Destination::getRating() // outputs desintaiton name
{
	return DestinationRating;
}

string Destination::getDestinationName()
{
	return DestinationName;
}

void Destination::display() //Displays destination array. As seen in Session 10.
{
	cout << setfill(' ') << setw(10) << DestinationName << setfill(' ') << setw(10) << DestinationRating << "\n" << endl;
}

bool Destination::getDestination(string userInput) //gets user chosen destination
{
	if (userInput == DestinationName)
	{
		cout << "Success!\n" << "You have chosen " << DestinationName << endl;
		return true;
	}
	else
	{
		return false;
	}
}


void Hotel::hdisplay() //Displays hotel array
{
	cout << setfill(' ') << setw(10) << HotelName << setfill(' ') << setw(10) << RealPrice << "\n" << endl;
}

void Hotel::setPrice(double realPriceNew)
{
	RealPrice = realPriceNew;
}


bool Hotel::canAfford(int budget, int maxRealPriceNew)
{

	if ((budget > RealPrice) && (budget < maxRealPriceNew))
	{
		cout << "\t" << HotelName << "\n" << endl;

		return false;
	}
	else if (budget > maxRealPriceNew) //needs changing to a variable maxPrice to take into account most expensive destination and duration
	{
		cout << "\t" << HotelName << "\n" << endl;
		return true;
	}

}
string Hotel::getName()
{
	return HotelName;
}


double Hotel::getPrice()
{
	return HotelPrice;
}
double Hotel::getRealPrice()
{
	return RealPrice;
}

double Hotel::getDestinationRating(Destination ratingObject)
{
	return ratingObject.DestinationRating;
}

bool Hotel::getHotel(string userInput, int hotelPrice, int budget) //gets user chosen destination
{
	if ((userInput == HotelName) && (hotelPrice <= budget))
	{
		cout << "Success!\n" << "You have chosen to stay at " << HotelName << endl;
		return true;
	}
	else
	{
		return false;
	}
}


double calcPrice(double hotelPrice, double rating)
{
	rating = rating / 100 + 1;
	double realPrice;
	realPrice = hotelPrice * rating;
	return realPrice;
}

double calcPriceLength(double weeksMultiply, double hotelPrice)
{
	double realPrice2;
	if (weeksMultiply != 1)
	{
		weeksMultiply = weeksMultiply / 10 + 1;
		realPrice2 = hotelPrice * weeksMultiply;
		return realPrice2;
	}
	else
	{
		return hotelPrice;
	}

}

double upgradedFlights(int flights)
{
	double price;
	price = flights * 3;
	return price;
}


double flights(int destRatingNew)
{
	double multiplier = 0.0;
	double price = 0.0;
	int baseFlights = 200;
	double smallNumber = 0;
	smallNumber = (destRatingNew / 400.0);
	multiplier = smallNumber + 1.0;
	price = multiplier * baseFlights;
	return price;
}




//void Destination::load() //opening destinations.txt file to load into variables
//{
//	ifstream destfile("destinations.txt");
//	destfile.open("destinations.txt");
//	if (destfile.is_open())
//	{
//		cout << "Your file was opened " << endl;
//		cout << destfile.rdbuf(); //print out whole file
//	}
//	else
//	{
//		cout << "Your file was not found" << endl;
//	}
//	
//	destfile >> DestinationName >> DestinationRating;
//}


