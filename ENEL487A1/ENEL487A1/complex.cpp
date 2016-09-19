/**
Programmer: Chris Pikula
Project: Making a Complex Number calculator
Date: 2016-9-13

Description: This is a simple program to reintroduce us to the 
concepts of programming in c++
It can calculate (+,-,/,*) of two complex numbers
*/

#include <iostream>  
#include <string>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cmath>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::cin;
using std::ofstream;
using std::ifstream;
using std::streambuf;
using std::istringstream;

struct Complex 
/**
This is our data-structure that holds our complex number, 
as well as holding the operator functions that perform the four basic
arithmetic operations on them.
*/
{
	double real;
	double img;
	struct Complex& operator+=(const Complex& rhs)
	{
		real += rhs.real;
		img += rhs.img;
		return *this;
	}
	struct Complex& operator-=(const Complex& rhs)
	{
		real -= rhs.real;
		img -= rhs.img;
		return *this;
	}
	struct Complex operator*(const Complex rhs)
	{
		Complex result;
		result.real = real*rhs.real - img * rhs.img;
		result.img = real*rhs.img + img * rhs.real;
		return result;
	}
	struct Complex operator/(const Complex rhs)
	{
		Complex result;
		result.real = (real*rhs.real + img*rhs.img) / 
			(rhs.real*rhs.real + rhs.img*rhs.img);
		result.img = (img*rhs.real - real*rhs.img) / 
			(rhs.real*rhs.real + rhs.img*rhs.img);
		return result;
	}
};
inline Complex operator+(Complex lhs, const Complex& rhs)
/**
This function outputs the result of adding two Complex structs
*/
{
	lhs += rhs;
	return lhs;
}
inline Complex operator-(Complex lhs, const Complex& rhs)
/**
This function outputs the result of subtracting two Complex structs
*/
{
	lhs -= rhs;
	return lhs;
}
std::ostream& operator<<(std::ostream& os, const Complex& input)
{
	/**
	This function prints out a Complex number via overloading the <<
	operator
	*Note, it has to the first != to provide the proper output format
	otherwise it outputs nan(ind)
	*/
	if (input.real != input.real)
	{
		os << "nan";
	}
	else
	{
		os << input.real;
	}
	os << ' ';
	if (input.img < 0 && !(input.img != input.img))
	{
		os << '-';
	}
	else
	{
		os << '+';
	}
	os << " j ";
	if (input.img != input.img)
	{
		os << "nan";
	}
	else
	{
		os << fabs(input.img);
	}
	os << endl;
	return os;
}

struct MathOperation 
/**
This is our data structure that is used to return our parsed data back 
from our parse function, as well as it is used to pass the data to the
calculator function
*/
{
	char operation;
	Complex first;
	Complex second;
};
void usage(char const *progname)
/**
This is our function that tells the user how to use the program if they 
don't enter in the correct number of parameters
*/
{
	cerr << "Complex Number Calculator" << endl
		 << "Usage: " << progname << " <input file> <output file>" 
		<< endl;
	exit(1);
}

void consoleText()
{
	/**
	This is our function that tells the user how to use the program 
	once it starts
	*/
	cerr << "Type a letter to specify the aritmetic operator "
		<< "(A, S, M, D)" << endl
		<< "followed by two complex numbers expressed as "
		<< "pairs of doubles." << endl 
		<< "Type Q to quit." << endl;
}

bool stringstreamCont(std::istringstream &ss)
/**
This function makes sure that we still have inputs left in the stringstream.
*/
{
	bool fail = false;
	if (!ss)
	{
		cerr << "Too few operands ";
		fail = true;
	}
	return fail;
}
bool stringstreamEnd(std::istringstream &ss)
/**
This function makes sure that we don't have inputs left in the stringstream.
*/
{
	bool fail = false;
	if (ss.rdbuf()->in_avail() > 1)
	//test to see how many available charachters are left.
	{
		cerr << "Too many operands ";
		fail = true;
	}
	return fail;
}
MathOperation parseLine(string input)
/**
This function returns a MathOperation object when given a string input.
If the string input is malformed, it outputs a 'F'ail operation type.
It does some checks to make sure the stringstream still has the proper 
number of entries to do this.
*/
{
	MathOperation operation;
	std::istringstream ss(input);
	bool fail = false;
	fail |= stringstreamCont(ss);
	ss >> operation.operation;
	if (operation.operation == 'a' || operation.operation == 'A')
	{
		operation.operation = 'A';
	}
	else if (operation.operation == 's' || operation.operation == 'S')
	{
		operation.operation = 'S';
	}
	else if (operation.operation == 'm' || operation.operation == 'M')
	{
		operation.operation = 'M';
	}
	else if (operation.operation == 'd' || operation.operation == 'D')
	{
		operation.operation = 'D';
	}
	else if (operation.operation == 'q' || operation.operation == 'Q')
	{
		operation.operation = 'Q';
		return operation;
	}
	else
	{
		operation.operation = 'F';//for fail
	}
	fail |= stringstreamCont(ss);
	ss >> operation.first.real;
	fail |= stringstreamCont(ss);
	ss >> operation.first.img;
	fail |= stringstreamCont(ss);
	ss >> operation.second.real;
	fail |= stringstreamCont(ss);
	ss >> operation.second.img;
	fail |= stringstreamEnd(ss);
	if (fail == true)
	{
		operation.operation = 'F';
	}
	return operation;
}
Complex calculator(MathOperation input)
/**
This is where we calculate our complex number given some correctly formatted
MathOperation object.
*/
{
	Complex result;
	if (input.operation == 'A')
	{
		result = input.first + input.second;
	}
	else if (input.operation == 'S')
	{
		result = input.first - input.second;
	} 
	else if (input.operation == 'M')
	{
		result = input.first * input.second;
	}
	else if (input.operation == 'D')
	{
		result = input.first / input.second;
	} 
	return result;
}

int main(int argc, char ** argv)
/**
Our main program
*/
{
	bool batchMode = false;
	string input;
	Complex result;
	MathOperation parsedInput;

	streambuf *coutbuf = cout.rdbuf();
	streambuf *cinbuf = cin.rdbuf();
	//saving our stream buffers, incase we need to swap
	//them to file stream buffers.
	
	ofstream batchOutput;
	ifstream batchInput;
	//declaring our possible instances of our input and 
	//output files
	
	if (!(argc == 3 || argc == 1))
	{
		usage(argv[0]);
	}
	if (argc == 3)
	{
		istringstream ss1(argv[1]);
		istringstream ss2(argv[2]);
		string firstArgument = "/n";
		string secondArgument = "/n";
		ss1 >> firstArgument;
		ss2 >> secondArgument;
		if (!ss1) 
		{
			cerr << "error converting first argument: " 
				<< argv[1] << '\n';
			exit(1);
		}
		if (!ss2)
		{
			cerr << "error converting second argument: " 
				<< argv[2] << '\n';
			exit(1);
		}

		//batchMode is 'file-io' mode.
		//therefore we need to redirect cin
		//and cout to our file rdbuf()'s.
		batchMode = true;
		batchInput.open(firstArgument.c_str());
		batchOutput.open(secondArgument.c_str());
		cout.rdbuf(batchOutput.rdbuf());
		cin.rdbuf(batchInput.rdbuf());
		
	}
	if (batchMode == false)
	{
		consoleText();
		//we only want to output to the console if
		//we require console input
	}
	
	do
	{
		if (cin.eof() == false)
			//This will only be triggered in batch mode, 
			//will still be 'checked' and 'false' in 
			//normal mode.
		{
			getline(cin, input); 
			//can be used in either normal, or batch mode.
		}
		else
		{
			break;
		}
		parsedInput = parseLine(input);
		if (parsedInput.operation == 'Q')
		{
			break;
		}
		else if (parsedInput.operation == 'F')
		{
			cerr << endl; 
			cout << endl;
			continue;
		}
		result = calculator(parsedInput);

		cout << result;
	} while (true);

	batchInput.close();
	batchOutput.close();

	//returning our cin/cout streams back to their
	//original states.  Good practice
	cin.rdbuf(cinbuf);
	cout.rdbuf(coutbuf);

}

//END OF FILE