/*************************************************************************//**
* @file
*****************************************************************************/
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include "util.h"
#include "globals.h"
#include "mouse.h"
#include "graphics.h"
#include "hash.h"

using namespace std;


/**************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 * This function gets called when an event happens in callbacks. Depending on
 * the enum and data passed in it will do different things. When a mouse click
 * is passed in, it sends that data to the click function in the container
 * class screen. When a mouse drag is sent in, it sends it to the clickAndDrag
 * function in screen.  It does the same for keyboard presses, sending them to
 * keyboardAction in screen.
 *
 * @param[in] event - structure that contains data about the event to handle
 *****************************************************************************/

void utilityCentral(Event eventstruct)
{

    static Hash<string, string> Table(5);
    //int area = 750;
    static unsigned int size = Table.size(); //edge;
    //edge = ceil(sqrt(size));
    static int mostprobed = 0;
    unsigned int wd = glutGet(GLUT_WINDOW_WIDTH), ht = glutGet(GLUT_WINDOW_HEIGHT);
    static string banner = "Load Factor = ", lf="Load Factor = ", mf = "% : MaxFreq ",
                  mfk = " : Most Freq Key = ";

    static string ifile;
    static ifstream fin;
    double barLF;
    vector<string> data;

    //switch the enum containing type of event
    //call appropriate function in container
    switch (eventstruct.eventtype)
    {
    case INIT:
    {
        /*
        cout << "[Hold Space to Hash]"<<endl;
        cout << "Input data file name: ";
        cin >> ifile;
        */
        ifile = "HinkersSite.txt";
        fin.open(ifile);
        cout<<"Hashing Program 2 Webpage."<<endl;

        if(!fin.is_open())
        {
            cout<<"File not found."<<endl;
            exit(1);
        }

        DrawFilledRectangle(0.0, 0.0, wd, ht, Black);
        break;
    }
    case DISPLAY:
    {


        string j;
        int axis = 5;

        //DrawFilledRectangle(0.0, ht-10, wd, ht, White);


        while(fin >> j)
        {
            data.push_back(j);
        }
        for(unsigned int k = 0; k<data.size(); k++)
        {
            Table.insert(data[k],data[k]);
            size = Table.size();
            //edge = ceil(sqrt(size));

            DrawFilledRectangle(0.0, 0.0, wd, ht, Black);

            DrawLine(wd/4, ht-42, wd/4, ht-50, White);
            DrawLine(wd/6, ht-42, wd/6, ht-50, White);
            DrawLine(2*wd/10, ht-42, 2*wd/10, ht-50, White);//loading bar
            DrawLine(wd/8, ht-42, wd/8, ht-50, White);

            DrawLine(axis, ht*0.9, axis, ht*0.01, White);
            DrawTextString("0", axis + 5, ht*0.01, White);
            DrawTextString(to_string(Table.size()), axis, ht*0.9, White);

            DrawTextString("0.25", wd/8 - 15, ht-75, White);
            DrawTextString("0.50", wd/4 - 15, ht-75, White);

            DrawFilledRectangle(wd-50, ht-100, wd-10, ht-60, Blue);
            DrawTextString("0-3", wd - 100, ht-90, White);

            DrawFilledRectangle(wd-50, ht-150, wd-10, ht-110, Green);
            DrawTextString("4-6", wd - 100, ht-140, White);

            DrawFilledRectangle(wd-50, ht-200, wd-10, ht-160, Yellow);
            DrawTextString("7-9", wd - 100, ht-180, White);

            DrawFilledRectangle(wd-50, ht-250, wd-10, ht-210, Red);
            DrawTextString("9+", wd - 100, ht-230, White);

            for(double i = 0.01; i <= 0.9; i *= 1.1)
            {
                DrawLine(axis, ht*i, axis + 3, ht*i, White);
            }

            DrawTextString("Load Factor", 3, ht - 20, White);

            lf = lf + to_string((int)(100*Table.loadFactor()));

            string sData = "";
            stringstream ss;

            for(unsigned int i = 0; i < size; i++)
            {
                barLF = (double)(wd)*Table.loadFactor();
                DrawFilledRectangle(0, ht-40, barLF/2, ht- 22, White);

                if(Table.hashCodeGenerated(i) > 0)
                    DrawRing(i, Table.hashCodeGenerated(i), size);
                //DrawSector(i, Table.hashCodeGenerated(i), area, edge);
                if(Table.hashCodeGenerated(i) > mostprobed)
                {
                    mfk = " : Most Freq Key = ";
                    mf = "% : MaxFreq ";
                    mostprobed = Table.hashCodeGenerated(i);
                    mf = mf + to_string(mostprobed);
                    ss << Table.getKey(i);
                    ss >> sData;
                    mfk = mfk + sData;
                }

            }

            banner = lf+mf+mfk;

            lf = "Load Factor = ";

            glutSetWindowTitle(banner.c_str());
            glutSwapBuffers();
        }
        DrawTextString("Hashing Complete.", wd/2-68, ht*0.9, White);
        glutSwapBuffers();
        break;
    }
    case KEYBOARD:
    {
        if (eventstruct.key == 27)//Escape key pressed
        {
            fin.close();
            exit(0);
        }
        if(eventstruct.key == 32)
        {
            glutPostRedisplay();
        }
    }
        break;
    case MOUSECLICK:
        if(eventstruct.button == GLUT_LEFT_BUTTON && eventstruct.state == GLUT_DOWN)
        {
            //change this state to check if game state is prepared for guess

        }
        break;
    case MOUSEDRAG:

        break;
    }

}
