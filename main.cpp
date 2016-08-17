/*************************************************************************//**
 * @file 
 *
 * @mainpage Program 2 - Hash Visualization
 * 
 * @section CSC300 Data Structures
 *
 * @author Zac Christy, Kenneth Petry, Catherine Oborski (I think)
 * 
 * @date March 30, 2016
 * 
 * @par Professor: 
 *         Dr. Hinker
 * 
 * @par Course: 
 *         CSC300 - M001 - 10:00 a.m.
 * 
 * @par Location: 
 *         McLaury - Room 205
 *
 * @section program_section Program Information 
 * 
 * @details 
 * 
 * Generic Program Description:
 * 
 * This program is designed to allow the user to see a visual representation
 * of a hash table being filled with data
 *
 * Detailed Program Description:
 * 
 *
 *
 * @par Usage: 
 * @verbatim  
 * c:\> make 
 * d:\> c:\bin\make
 * @endverbatim 
 *
 * @section todo_bugs_modification_section Todo, Bugs, and Modifications
 * 
 * @bug Visualizer will not respond to mouse or keyboard while running
 *
 * @todo Fix visualizer.
 * 
 * @par Modifications and Development Timeline: 
 @verbatim 
 Date          Modification 
 ---------------------------------------------------------------------------- 
 March 15 - Started program.
 March 16 - Tested different hash codes.
 March 18 - Started functionality of hash class.
 March 19 - Wrote insert and find function
 March 20 - Started Visualization.
 
 March 21 -> March 29 -
 	Bug fixes and optimization.
 
 March 30 - Program completed and turned in.
 
 @endverbatim
 *
 *****************************************************************************/

// include files
#include <iostream>
#include <stdlib.h>
#include "util.h"
#include "graphics.h"
#include <GL/glut.h>



using namespace std;

/// OpenGL callback function prototypes
/// Display handler
void display( void );
/// Handler for reshape events
void reshape( int w, int h );
/// Handler for keyboard events
void keyboard( unsigned char key, int x, int y );
/// Handler for mouse click events
void mouseClick(int button, int state, int x, int y);
/// Handler for mouse motion (i.e. drag) events
void mouseMotion(int x, int y);
/// Displays letter clicked
void displayGuess(int x, int y);


// other function prototypes
/// Function to do the initialization of the openGL event handlers
void initOpenGL( const char *filename, int nrows, int ncols);



/***************************************************************************//**
 * @author Paul Hinker
 *
 * @par Description:
 * A barebones main to demonstrate the use of OpenGL and it's callback
 * functionality.
 *
 * @param[in] argc - a value denoting the number of arguments declared in the
 * command line
 * @param[in] argv - a pointer array full of the character strings signifying
 * the files and function calls needed by the user
 *
 * @returns 0 program ran successful.
 * @returns 1 the program failed or there was an error.
 ******************************************************************************/
int main( int argc, char *argv[] )
{

  glutInit( &argc, argv );
  initOpenGL( argv[1], 800, 1500);

  // go into OpenGL/GLUT main loop, never to return
  glutMainLoop();

  // Keep the compiler from complaining
  return 0;
}


/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * Initialize a variety of glut callback functions
 *
 * @param[in] windowTitle - The title to place on the window
 * @param[in] nrows       - Number of pixels for the window height
 * @param[in] ncols       - Number of pixels for the window width
 ******************************************************************************/
///various commands to initialize OpenGL and GLUT
void initOpenGL( const char *windowTitle, int nrows, int ncols)
{
  //int imgCols = 0, imgRows = 0;
  // 32-bit graphics and single buffering

  utilityCentral(Event(nrows, ncols));
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

  glutInitWindowSize( ncols, nrows );  // initial window size
  glutInitWindowPosition( 100, 50 );   // initial window  position
  glutCreateWindow("[HOLD SPACE TO HASH]");   // window title


  glClearColor( 0.0, 0.6, 0.3, 0.0 );         // use black for glClear command

  // callback routines
  glutDisplayFunc( display );   // how to redisplay window
  glutReshapeFunc( reshape );   // how to resize window
  glutKeyboardFunc( keyboard ); // how to handle key presses
  glutMouseFunc(mouseClick);          // how to handle mouse clicks
  glutMotionFunc(mouseMotion);        // how to handle mouse movements
}

/******************************************************************************/
/*                          OpenGL callback functions                         */
/******************************************************************************/

/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * callback function that tells OpenGL how to redraw window

 ******************************************************************************/

void display( void )
{
  glClear( GL_COLOR_BUFFER_BIT);

  utilityCentral(Event());

  //glutSwapBuffers();
}

/******************************************************************************/


/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * callback function that tells OpenGL how to resize window
 * note that this is called when the window is first created
 *
 * @param[in] w - with of the screen
 * @param[in] h - height of the screen
 ******************************************************************************/
///callback function that tells OpenGL how to resize window
///note that this is called when the window is first created
void reshape( int w, int h )
{
  // how to project 3-D scene onto 2-D
  glMatrixMode( GL_PROJECTION );    // use an orthographic projection
  glLoadIdentity();         // initialize transformation matrix
  gluOrtho2D( 0.0, w, 0.0, h );
  glViewport( 0, 0, w, h );       // adjust viewport to new window
  glutPostRedisplay();
}

/******************************************************************************/


/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * callback function that tells OpenGL how to handle keystrokes
 *
 * @param[in] key - Key pressed on keyboard
 * @param[in] x - x location where the mouse was when the key was typed
 * @param[in] y - y location where the mouse was when the key was typed
 ******************************************************************************/
///callback function that tells OpenGL how to handle keystrokes
void keyboard( unsigned char key, int x, int y )
{

  utilityCentral(Event(key, x, y));

}


/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * Transfer mouse click events to the central processing function
 *
 * @param[in] button - Which mouse button was clicked
 * @param[in] x - x location where the mouse was when button was clicked
 * @param[in] y - y location where the mouse was when button was clicked
 ******************************************************************************/
///Transfer mouse click events to the central processing function
void mouseClick(int button, int state, int x, int y)
{
   utilityCentral(Event(button, state, x, y));

}


/***************************************************************************//**
 * @author Paul Hinker
 * @par Description:
 * Transfer mouse movement events to the central processing function
 *
 * @param[in] x - x location of mouse
 * @param[in] y - y location of mouse
 ******************************************************************************/
///Transfer mouse movement events to the central processing function
void mouseMotion(int x, int y)
{
  utilityCentral(Event(MOUSEDRAG, x, y));
  glutPostRedisplay();
}

