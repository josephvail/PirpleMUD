/*!
 * @file CCharacter.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CCHARACTER_H
#define PIRPLEMUD_CCHARACTER_H

#include "pirple_.h"
#include "CPoints.h"

namespace Pirple
{
class CRoom;
class CItem;

class CCharacter
{
public:
    enum ATTRIBUTE
    {
        STRENGTH = 0,
        INTELLIGENCE,
        WISDOM,
        DEXTERITY,
        CONSTITUTION,
        CHARISMA
    };
    enum { NUM_ATTRIBUTES = CHARISMA + 1 };
    struct SAttributes
    {
        pm_attribute Values[NUM_ATTRIBUTES] = {0, 0, 0, 0, 0, 0};
        pm_attribute StrengthAdd = 0;
    };

    enum SAVINGTHROW
    {
        SAVING_PARA = 0,
        SAVING_ROD,
        SAVING_PETRI,
        SAVING_BREATH,
        SAVING_SPELL
    };
    enum { NUM_SAVINGTHROWS = SAVING_SPELL + 1 };
    struct SSavingThrows
    {
        CPoints<pm_savingthrow> Values[NUM_SAVINGTHROWS] = {{-20, 20}, {-20, 20}, {-20, 20}, {-20, 20}, {-20, 20}};
//        pm_savingthrow Values[NUM_SAVINGTHROWS] = {0, 0, 0, 0, 0}
    };

    enum ATTACKTYPE
    {
        ATTACKTYPE_HIT = 0,
        ATTACKTYPE_STING,
        ATTACKTYPE_WHIP,
        ATTACKTYPE_SLASH,
        ATTACKTYPE_BITE,
        ATTACKTYPE_BLUDGEON,
        ATTACKTYPE_CRUSH,
        ATTACKTYPE_POUND,
        ATTACKTYPE_CLAW,
        ATTACKTYPE_MAUL,
        ATTACKTYPE_THRASH,
        ATTACKTYPE_PIERCE,
        ATTACKTYPE_BLAST,
        ATTACKTYPE_PUNCH,
        ATTACKTYPE_STAB,
        ATTACKTYPE_SUFFERING
    };
    enum { NUM_ATTACKTYPES = ATTACKTYPE_SUFFERING + 1 };

    enum AFFECTFLAG
    {
        AF_BLIND                = (1 << 0L),
        AF_INVISIBLE            = (1 << 1L),
        AF_DETECT_ALIGNMENT     = (1 << 2L),
        AF_DETECT_INVISIBLE     = (1 << 3L),
        AF_DETECT_MAGIC         = (1 << 4L),
        AF_SENSE_LIFE           = (1 << 5L),
        AF_WATER_WALK           = (1 << 6L),
        AF_SANCTUARY            = (1 << 7L),
        AF_REUSE_1              = (1 << 8L), // Was AFF_GROUP, no need
        AF_CURSE                = (1 << 9L),
        AF_INFRAVISION          = (1 << 10L),
        AF_POISON               = (1 << 11L),
        AF_PROTECT_FROM_EVIL    = (1 << 12L),
        AF_PROTECT_FROM_GOOD    = (1 << 13L),
        AF_SLEEP                = (1 << 14L),
        AF_NO_TRACK             = (1 << 15L),
        AF_REUSE_2              = (1 << 16L),
        AF_REUSE_3              = (1 << 17L),
        AF_SNEAK                = (1 << 18L),
        AF_HIDE                 = (1 << 19L),
        AF_REUSE_4              = (1 << 20L),
        AF_CHARM                = (1 << 21L)
    };
    enum { NUM_AFFECTFLAGS = 22 };

    enum WEARPOSITION
    {
        WEAR_NONE = -1,
        WEAR_HEAD = 0,
        WEAR_NECK_1,
        WEAR_NECK_2,
        WEAR_ABOUT,
        WEAR_BODY,
        WEAR_ARMS,
        WEAR_HANDS,
        WEAR_LIGHT,
        WEAR_WIELD,
        WEAR_HOLD,
        WEAR_SHIELD,
        WEAR_WRIST_R,
        WEAR_WRIST_L,
        WEAR_FINGER_R,
        WEAR_FINGER_L,
        WEAR_WAIST,
        WEAR_LEGS,
        WEAR_FEET
    };
    enum { NUM_WEARPOSITIONS = WEAR_FEET + 1 };

    enum CLASS
    {
        CLASS_UNDEFINED = 0,
        CLASS_MAGE,
        CLASS_CLERIC,
        CLASS_THIEF,
        CLASS_WARRIOR
    };
    enum { NUM_CLASSES = CLASS_WARRIOR + 1 };

    enum SEX
    {
        SEX_NONE = 0,
        SEX_MALE,
        SEX_FEMALE
    };
    enum { NUM_SEXES = SEX_FEMALE + 1 };

    enum POSITION
    {
        POSITION_NONE = 0,
        POSITION_LAYING,
        POSITION_RECLINED,
        POSITION_SITTING,
        POSITION_KNEELING,
        POSITION_STANDING
    };
    enum { NUM_POSITIONS = POSITION_STANDING + 1 };

    enum STATE
    {
        STATE_NONE = 0,
        STATE_DEAD,
        STATE_MORTALLY_WOUNDED,
        STATE_INCAPACITATED,
        STATE_STUNNED,
        STATE_SLEEPING,
        STATE_AWAKE
    };
    enum { NUM_STATES = STATE_AWAKE + 1 };

    struct SPoints
    {
        CPoints<pm_points> hitPoints = CPoints<pm_points>(-200, 1000000, 1);
        CPoints<pm_points> manaPoints = CPoints<pm_points>(-200, 1000000, 1);
        CPoints<pm_points> movePoints = CPoints<pm_points>(-200, 1000000, 1);
        CPoints<pm_armorclass> armorClass = CPoints<pm_armorclass>(-100, 100, 0);
    };
private:
    pm_index number = NOBODY;
    CLASS class_ = CLASS_UNDEFINED;
    ATTACKTYPE defaultAttackType = ATTACKTYPE_HIT;
    list<SKILL> skills;
    list<SPELL> spells;
    pm_flags systemFlags = 0;
    pm_flags tempFlags = 0;
    pm_flags affectFlags = 0;
    shared_ptr<string> name;
    shared_ptr<string> shortDescription;
    shared_ptr<string> detailedDescription;
    STATE state = STATE_NONE;
    POSITION position = POSITION_NONE;
    pm_points wimpyPoints = 0;
    pm_level level = 0;
    pm_weight weight = 0;
    pm_height height = 0;
    SAttributes realAttributes;
    SAttributes affectedAttributes;
    SPoints points;
    SSavingThrows savingThrows;
    pm_currency gold = 0;
    pm_currency bank = 0;
    pm_experience experience = 0;
    pm_roll damageRoll = 0;
    pm_roll hitRoll = 0;
    weak_ptr<CCharacter> fighting;
    weak_ptr<CCharacter> hunting;
    weak_ptr<CRoom> inRoom;
    weak_ptr<CRoom> inPreviousRoom;
    list<weak_ptr<CItem>> items;
    pm_weight carriedWeight = 0;
    weak_ptr<CItem> equipment[NUM_WEARPOSITIONS];
    list<weak_ptr<CCharacter>> followers;
    weak_ptr<CCharacter> master;
    // int timer TODO add
    // TODO add player age and logged in last info
public:
    virtual ~CCharacter()
    {

    }

    virtual bool IsMobile() const noexcept = 0;
    bool IsPlayer() const noexcept
    {
        if (IsMobile())
            return false;
        else
            return true;
    }
};

}

#endif //PIRPLEMUD_CCHARACTER_H
