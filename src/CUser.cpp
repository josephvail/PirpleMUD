/*!
 * @file CUser.cpp
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#include "CTCPClient.h"
#include "CUser.h"

namespace Pirple
{
bool CUser::ParseInput(const string& pInput)
{
    return false;
}

bool CUser::ParseWait()
{
    /*
     * Not combined to retain --(d->wait) behavior. -gg 2/20/98
     * If no wait state, no subtraction.  If there is a wait
     * state then 1 is subtracted. Therefore we don't go less
     * than 0 ever and don't require an 'if' bracket. -gg 2/27/99
     */
    /*TODO
    if (d->character) {
        GET_WAIT_STATE(d->character) -= (GET_WAIT_STATE(d->character) > 0);

        if (GET_WAIT_STATE(d->character))
            continue;
    }*/
    // Return false when we want to parse some input
    return true;
}

void CUser::ResetWait()
{
    /*TODO
    // Reset the idle timer & pull char back from void if necessary
    d->character->char_specials.timer = 0;
    if (STATE(d) == CON_PLAYING && GET_WAS_IN(d->character) != NOWHERE) {
    if (IN_ROOM(d->character) != NOWHERE)
        char_from_room(d->character);
    char_to_room(d->character, GET_WAS_IN(d->character));
    GET_WAS_IN(d->character) = NOWHERE;
    act("$n has returned.", TRUE, d->character, 0, 0, TO_ROOM);
    }
    GET_WAIT_STATE(d->character) = 1;*/
}
}
