// Written by Michael Feeney, Fanshawe College, 2010
// mfeeney@fanshawec.on.ca
// It may be distributed under the terms of the General Public License:
// http://www.fsf.org/licenses/gpl.html
// Use this code at your own risk. It is indented only as a learning aid.
//
#include <iostream>
#include <fstream>
#include <string>
#include "Global.h"

void printTheWhatsThisProgramAboutBlurb(void)
{
	using namespace std;

	// Read the 'whatsthis.txt' file and print to the screen
	ifstream daFile("whats_this.txt");
	// Did it open?
	if (daFile.is_open())
	{
		string someText;
		while (!daFile.eof())
		{
			// read a string and print to screen
			daFile >> someText;
			if (someText == "<cr>")
			{
				cout << endl;
			}
			else
			{
				cout << someText << " ";
			}
		}
		cout << endl;
		daFile.close();
	}
	return;
}