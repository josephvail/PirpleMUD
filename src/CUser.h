/*!
 * @file CUser.h
 * @brief Enter a brief description of this file.
 * @details Enter a detailed description of this file.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa
 */

#ifndef PIRPLEMUD_CUSER_H
#define PIRPLEMUD_CUSER_H

#include "pirple_.h"
#include "CCharacter.h"

namespace Pirple
{
class CTCPClient;
class CPlayer;
class CUser
{
    friend class CTCPClient;
public:
    /*!
     * @enum MODE
     * @brief Enumeration of CUser modes.
     * @details Modes of connectedness for game users. MODE_???
     * is our normal state.
     *
     * Replaces CircleMUD::descriptor_data::connected partially
     * and CON_X macros in structs.h of CircleMUD.
     * @sa CTCPClient::GetMode(), CUser::GetMode()
     *
     * @sa CircleMUD::descriptor_data, CircleMUD::CON_X macros in structs.h
     */
    enum MODE
    {
        MODE_CLOSE                 = 0, /*!< Connection closed */
        MODE_DISCONNECTED          = 1, /*!< Linkless player */
        MODE_NEW_PASSWORD          = 2, /*!< Prompt new player to create a password */
        MODE_NEW_PASSWORD_CONFIRM  = 3, /*!< Prompt new player to confirm created password */
        MODE_CHOOSE_SEX            = 4, /*!< Choose which sex the character is */
        MODE_CHOOSE_CLASS          = 5, /*!< Choose which class the character is */
        MODE_READ_MOTD             = 6, /*!< Read the MOTD when logging in */
        MODE_MAIN_MENU             = 7, /*!< The main menu of the game */
        MODE_EXTRA_DESCRIPTION     = 11,/*!< The menu item to change a player's description */
        MODE_CHANGE_PASSWORD_OLD   = 12,/*!< The menu item prompt to verify a player's password before changing */
        MODE_CHANGE_PASSWORD_NEW   = 13,/*!< The menu item prompt to create a new password */
        MODE_CHANGE_PASSWORD_VERIFY= 14,/*!< The menu item to verify new player password */
        MODE_DELETE_CONFIRM_1      = 15,/*!< The first main menu delete confirmation prompt */
        MODE_DELETE_CONFIRM_2      = 16 /*!< The second main menu delete confirmation prompt */
    };
    enum { NUM_MODES = MODE_DELETE_CONFIRM_2 + 1 };

public:
    struct SPlayerIndex
    {
        pm_index PlayerNumber = NOBODY;
        CCharacter::CLASS PlayerClass = CCharacter::CLASS_UNDEFINED;
    };
private:
    // TODO remove this, as it is not stored full time while playing.
    //char	passwd[MAX_PWD_LENGTH+1]; /* character's password      */
    string password = "";
    pm_ushint numBadPassword = 0;
    string poofIn = "";
    string poofOut = "";
    MODE mode = MODE_CLOSE;
    weak_ptr<CTCPClient> client;
    list<SPlayerIndex> accountPlayers;
    weak_ptr<CPlayer> currentPlayer;
private:
    // TODO populate with real shit
    bool ParseInput(const string& input);
    bool ParseWait();
    void ResetWait();
};
}

#endif //PIRPLEMUD_CUSER_H
