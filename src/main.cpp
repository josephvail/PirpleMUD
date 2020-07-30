/*!
 * @file main.cpp
 * @brief Our main c++ file to create our game inside of main()
 * @details This file holds only our main() function.
 *
 * This file is equivalent to the main section of comm.c in CircleMUD.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 *
 * @sa CMUD.h
 *
 * @sa CircleMUD comm.c
 */

#include "PirpleMUD.h"

/*!
 * @fn main(int argc, char** argv)
 * @brief Main function of the program used to create a CMUD object.
 * @details We kept the same command line format as CircleMUD's function.
 * Refer to the command line option -h for format.
 * @param argc int
 * @param argv char**
 * @return int 0 on success. other value dependent on type of failure.
 * @sa Pirple::CMUD
 */
Pirple::CMUD MUD;

int main(int argc, char** argv)
{
    int pos = 1;

    while ((pos < argc) && (*(argv[pos]) == '-'))
    {
        switch (*(argv[pos] + 1))
        {
        case 'o':
            if (*(argv[pos] + 2))
                MUD.SetLogName(argv[pos] + 2);
            else if (++pos < argc)
                MUD.SetLogName(argv[pos]);
            else
            {
                puts("SYSERR: File name to log to expected after option -o.");
                exit(1);
            }
            break;
        case 'd':
            if (*(argv[pos] + 2))
                MUD.SetPath(argv[pos] + 2);
            else if (++pos < argc)
                MUD.SetPath(argv[pos]);
            else
            {
                puts("SYSERR: Directory arg expected after option -d.");
                exit(1);
            }
            break;
        case 'm':
            MUD.AddRunFlag(Pirple::CMUD::RF_MINI_MUD);
            MUD.AddRunFlag(Pirple::CMUD::RF_NO_RENT_CHECK);
            puts("Running in minimized mode & with no rent check.");
            break;
        case 'c':
            MUD.AddRunFlag(Pirple::CMUD::RF_SYNTAX_CHECK);
            puts("Syntax check mode enabled.");
            break;
        case 'q':
            MUD.AddRunFlag(Pirple::CMUD::RF_NO_RENT_CHECK);
            puts("Quick boot mode -- rent check supressed.");
            break;
        case 'r':
            MUD.AddRunFlag(Pirple::CMUD::RF_NO_NEW_PLAYERS);
            puts("Restricting game -- no new players allowed.");
            break;
        case 's':
            MUD.AddRunFlag(Pirple::CMUD::RF_NO_SPECIALS);
            puts("Suppressing assignment of special routines.");
            break;
        case 'n':
            MUD.AddRunFlag(Pirple::CMUD::RF_SLOW_NAMESERVER);
            break;
        case 'h':
            /* From: Anil Mahajan <amahajan@proxicom.com> */
            cout << "Usage: " << argv[0] << " [-c] [-m] [-q] [-r] [-s] [-d pathname] [port #]" << endl;
            cout << "  -c             Enable syntax check mode." << endl;
            cout << "  -d <directory> Specify library directory (defaults to 'lib')." << endl;
            cout << "  -h             Print this command line argument help." << endl;
            cout << "  -m             Start in mini-MUD mode." << endl;
            cout << "  -o <file>      Write log to <file> instead of stderr." << endl;
            cout << "  -q             Quick boot (doesn't scan rent for object limits)" << endl;
            cout << "  -r             Restrict MUD -- no new players allowed." << endl;
            cout << "  -s             Suppress special procedure assignments." << endl;
            cout << "  -n             Mark the MUD as slownameserver and disable DNS lookups." << endl;
            exit(0);
        default:
            cout << "SYSERR: Unknown option -" << *(argv[pos] + 1) << " in argument string." << endl;
            break;
        }
        pos++;
    }
    Pirple::pm_ushint port = 0;
    if (pos < argc)
    {
        if (!isdigit(*argv[pos]))
        {
            cout << "Usage: " << argv[0] << " [-c] [-m] [-q] [-r] [-s] [-d pathname] [port #]" << endl;
            exit(1);
        }
        else
        {
            port = atoi(argv[pos]);
        }
    }
    if (MUD.Initialize(port) == true)
    {
        MUD.Run();
    }
    MUD.Stop();
    return 0;
}
