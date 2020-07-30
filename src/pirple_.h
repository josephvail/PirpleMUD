/*!
 * @file pirple_.h
 * @brief Internal and system definitions for the game.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa pirple_utils_.h, PirpleMUD.h
 */

#ifndef PIRPLEMUD_PIRPLE_H
#define PIRPLEMUD_PIRPLE_H

#include <unistd.h>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <cstring>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include <sys/param.h>

#include "pirple_utils_.h"
#include "pirple_skillsspells_.h"

using namespace std;

namespace Pirple
{
typedef signed long pm_index;
enum
{
    NOBODY = 0,
    NOTHING = 0,
    NOWHERE = 0,
    DARKNESS = 0
};
typedef signed short pm_level;
typedef signed long long pm_currency;
typedef signed int pm_weight;
typedef signed int pm_height;
typedef signed short pm_attribute;
typedef signed long pm_points;
typedef signed short pm_armorclass;
typedef signed long pm_experience;
typedef signed int pm_roll;
typedef signed short pm_savingthrow;

enum DIRECTION
{
    NORTH = 0,
    EAST,
    SOUTH,
    WEST,
    UP,
    DOWN
};
enum { NUM_DIRECTIONS = DOWN + 1 };

/*!
 * @var VERSION
 * @brief constant variable holding games current development version.
 * @details Variable holding MUD version.
 *
 * Replaces CircleMUD::circlemud_version string.
 * @return const std::string
 * @sa CircleMUD constants.c
 */
const string VERSION = "PirpleMUD, version 0.1";

/*!
 * @typedef pm_ushint
 * @return unsigned short
 */
typedef unsigned short pm_ushint;
/*!
 * @typedef pm_descriptor
 * @return int
 */
typedef int pm_descriptor;

/*!
 * @var SOCKET_CLOSED
 * @brief Enum describing the value of a closed socket descriptor.
 * @return enum of value -1 showing a closed socket.
 */
enum { SOCKET_CLOSED = -1 };
/*!
 * @var INVALID_SOCKET
 * @brief Enum describing the value of an invalid socket value returned from socket functions.
 * @details Replaces the macro for CircleMUD::INVALID_SOCKET
 * @return enum of value -1 showing an invalid socket.
 * @sa CircleMUD comm.c
 */
enum { INVALID_SOCKET = -1 };

typedef unsigned int pm_pulse;
/*
 * OPT_USEC determines how many commands will be processed by the MUD per
 * second and how frequently it does socket I/O.  A low setting will cause
 * actions to be executed more frequently but will increase overhead due to
 * more cycling to check.  A high setting (e.g. 1 Hz) may upset your players
 * as actions (such as large speedwalking chains) take longer to be executed.
 * You shouldn't need to adjust this.
 */
#define OPT_USEC 100000 // 10 passes per second
#define PASSES_PER_SEC (1000000 / OPT_USEC)
#define RL_SEC * PASSES_PER_SEC

#define PULSE_ZONE (10 RL_SEC)
#define PULSE_VIOLENCE ( 2 RL_SEC)
#define PULSE_AUTOSAVE (60 RL_SEC)
#define PULSE_IDLEPWD (15 RL_SEC)
#define PULSE_TIMESAVE (30 * 60 RL_SEC) // should be >= SECS_PER_MUD_HOUR

/* Variables for the output buffering system */
#define MAX_SOCK_BUF (12 * 1024) // Size of kernel's sock buf
#define MAX_PROMPT_LENGTH 96 // Max length of prompt
#define GARBAGE_SPACE 32 // Space for **OVERFLOW** etc
#define SMALL_BUFSIZE 1024 // Static output buffer size
/* Max amount of output that can be buffered */
#define LARGE_BUFSIZE (MAX_SOCK_BUF - GARBAGE_SPACE - MAX_PROMPT_LENGTH)

#define HISTORY_SIZE 5 // Keep last 5 commands.
#define MAX_STRING_LENGTH 8192
#define MAX_INPUT_LENGTH 256 // Max length per *line* of input
#define MAX_RAW_INPUT_LENGTH 512 // Max size of *raw* input
#define MAX_MESSAGES 60
#define MAX_NAME_LENGTH 20 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_PWD_LENGTH 10 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_TITLE_LENGTH 80 // Used in char_file_u *DO*NOT*CHANGE*
#define HOST_LENGTH 30 // Used in char_file_u *DO*NOT*CHANGE*
#define EXDSCR_LENGTH 240 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_TONGUE 3 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_SKILLS 200 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_AFFECT 32 // Used in char_file_u *DO*NOT*CHANGE*
#define MAX_OBJ_AFFECT 6 // Used in obj_file_elem *DO*NOT*CHANGE*
#define MAX_NOTE_LENGTH	1000 // arbitrary

/*
 * A MAX_PWD_LENGTH of 10 will cause BSD-derived systems with MD5 passwords
 * and GNU libc 2 passwords to be truncated.  On BSD this will enable anyone
 * with a name longer than 5 character to log in with any password.  If you
 * have such a system, it is suggested you change the limit to 20.
 *
 * Please note that this will erase your player files.  If you are not
 * prepared to do so, simply erase these lines but heed the above warning.
 */
#if defined(HAVE_UNSAFE_CRYPT) && MAX_PWD_LENGTH == 10
#error You need to increase MAX_PWD_LENGTH to at least 20.
#error See the comment near these errors for more explanation.
#endif
/*
 * XXX: These constants should be configurable.
 */
/* mud-life time */
#define SECS_PER_MUD_HOUR 75
#define SECS_PER_MUD_DAY (24*SECS_PER_MUD_HOUR)
#define SECS_PER_MUD_MONTH (35*SECS_PER_MUD_DAY)
#define SECS_PER_MUD_YEAR (17*SECS_PER_MUD_MONTH)

/* real-life time (remember Real Life?) */
#define SECS_PER_REAL_MIN 60
#define SECS_PER_REAL_HOUR (60*SECS_PER_REAL_MIN)
#define SECS_PER_REAL_DAY (24*SECS_PER_REAL_HOUR)
#define SECS_PER_REAL_YEAR (365*SECS_PER_REAL_DAY)
}

#endif //PIRPLEMUD_PIRPLE_H
