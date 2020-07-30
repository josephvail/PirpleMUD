/*!
 * @file CMobile.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CMOBILE_H
#define PIRPLEMUD_CMOBILE_H

#include "pirple_.h"
#include "CCharacter.h"

namespace Pirple
{
class CZone;
class CMobile: public CCharacter
{
public:
    enum SYSTEMFLAG
    {
        SF_HAS_SPECIAL_PROC     = (1 << 0L),
        SF_SENTINEL             = (1 << 1L),
        SF_SCAVENGER            = (1 << 2L),
        SF_REUSE                = (1 << 3L),
        SF_AWARE                = (1 << 4L),
        SF_AGGRESSIVE           = (1 << 5L),
        SF_STAY_IN_ZONE         = (1 << 6L),
        SF_WIMPY                = (1 << 7L),
        SF_AGGRESSIVE_TO_EVIL   = (1 << 8L),
        SF_AGGRESSIVE_TO_GOOD   = (1 << 9L),
        SF_AGGRESSIVE_TO_NEUTRAL = (1 << 10L),
        SF_HAS_MEMORY           = (1 << 11L),
        SF_HELPER               = (1 << 12L),
        SF_NO_CHARM             = (1 << 13L),
        SF_NO_SUMMON            = (1 << 14L),
        SF_NO_SLEEP             = (1 << 15L),
        SF_NO_BASH              = (1 << 16L),
        SF_NO_BLIND             = (1 << 17L),
    };
    enum { NUM_SYSTEMFLAGS = 18 };
    enum TEMPFLAG
    {
        TF_NOT_EXTRACTED_YET    = (1 << 0L)
    };
    enum { NUM_TEMPFLAGS = 1 };

private:
    shared_ptr<string> longDescription;
    POSITION defaultPosition = POSITION_NONE;
    STATE defaultState = STATE_NONE;
    pm_roll damageNumberDice = 0;
    pm_roll damageSizeDice = 0;
    weak_ptr<CZone> zone;
    list<weak_ptr<CCharacter>> memories;

public:
    virtual ~CMobile()
    {

    }

    virtual bool IsMobile() const noexcept
    {
        return true;
    }
};

}

#endif //PIRPLEMUD_CMOBILE_H
