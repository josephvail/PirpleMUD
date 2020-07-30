/*!
 * @file CPlayer.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CPLAYER_H
#define PIRPLEMUD_CPLAYER_H

#include "pirple_.h"
#include "CCharacter.h"

namespace Pirple
{
class CUser;
class CRoom;

class CPlayer: public CCharacter
{
public:
    enum PREFERENCEFLAG
    {
        PF_BRIEF                = (1 << 0L),
        PF_COMPACT              = (1 << 1L),
        PF_DEAF                 = (1 << 2L),
        PF_NO_TELL              = (1 << 3L),
        PF_DISPLAY_HIT_POINTS   = (1 << 4L),
        PF_DISPLAY_MANA_POINTS  = (1 << 5L),
        PF_DISPLAY_MOVE_POINTS  = (1 << 6L),
        PF_DISPLAY_EXITS        = (1 << 7L),
        PF_NO_HASSLE            = (1 << 8L),
        PF_QUEST                = (1 << 9L),
        PF_SUMMONABLE           = (1 << 10L),
        PF_NO_REPEAT            = (1 << 11L),
        PF_HOLY_LIGHT           = (1 << 12L),
        PF_COLOR_LEVEL_1        = (1 << 13L),
        PF_COLOR_LEVEL_2        = (1 << 14L),
        PF_NO_WIZNET            = (1 << 15L),
        PF_LOG_LEVEL_1          = (1 << 16L),
        PF_LOG_LEVEL_2          = (1 << 17L),
        PF_NO_AUCTION           = (1 << 18L),
        PF_NO_GOSSIP            = (1 << 19L),
        PF_NO_GRATS             = (1 << 20L),
        PF_ROOM_FLAGS           = (1 << 21L),
        PF_DISPLAY_PROMPT       = (1 << 22L)
    };
    enum { NUM_PREFERENCEFLAGS = 23 };

    enum SYSTEMFLAG
    {
        SF_KILLER               = (1 << 0L),
        SF_THIEF                = (1 << 1L),
        SF_FROZEN               = (1 << 2L),
        SF_SITE_ALLOWED         = (1 << 3L),
        SF_NO_SHOUT             = (1 << 4L),
        SF_NO_TITLE             = (1 << 5L),
        SF_DELETED              = (1 << 6L),
        SF_LOAD_ROOM            = (1 << 7L),
        SF_NO_WIZLIST           = (1 << 8L),
        SF_NO_DELETE            = (1 << 9L),
        SF_START_INVISIBLE      = (1 << 10L),
        SF_CRYO_SAVE            = (1 << 11L)
    };
    enum { NUM_SYSTEMFLAGS = 12 };
    // System flags that are not to be saved
    enum TEMPFLAG
    {
        TF_WRITING              = (1 << 0L),
        TF_MAILING              = (1 << 1L),
        TF_NEEDS_CRASH_SAVE     = (1 << 2L),
        TF_NOT_EXTRACTED_YET    = (1 << 3L)
    };
    enum { NUM_TEMPFLAGS = 4 };
private:
    string title = "";
    weak_ptr<CRoom> loadRoom;
    pm_level freezeLevel = 0;
    pm_level invisibleLevel = 0;
    pm_flags preferenceFlags = 0;
    weak_ptr<CUser> user;

public:
    virtual ~CPlayer()
    {
    }

    virtual bool IsMobile() const noexcept
    {
        return false;
    }
};

}

#endif //PIRPLEMUD_CPLAYER_H
