/*!
 * @file CTCPClient.h
 * @brief The TCP client socket that is attached to each player when they connect.
 * @details CTCPClient is the client socket class for our players. Every connected
 * player has an attached CTCPClient. The client sockets are managed in a container
 * owned by CTCPServer.
 *
 * CTCPClient and CSocket together take the place of CircleMUD comm.c
 * functions, as well as CircleMUD::descriptor_data.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa          CTCPClient.cpp, CTCPServer.h, CSocket.h
 *
 * @sa          CircleMUD::descriptor_data, CircleMUD comm.c
 */

#ifndef PIRPLEMUD_CTCPCLIENT_H
#define PIRPLEMUD_CTCPCLIENT_H

#include "CSocket.h"

namespace Pirple
{
class CTCPServer;
class CUser;
/*!
 * @class CTCPClient
 * @brief Our client socket object that is attached to each user or player.
 * @details CTCPClient is the client socket for every user connected.
 * If a player is connected, they will have an attached CTCPClient.
 * The group of CTCPClients attached to the game are managed by a CTCPServer
 * socket.
 *
 * CTCPClient holds some of the data from CircleMUD comm.c
 * as well as CircleMUD::descriptor_data and socket related functions.
 * @sa CSocket, CTCPServer
 *
 * @sa CircleMUD::descriptor_data, CircleMUD comm.c
 */
class CTCPClient: public CSocket
{
    friend class CTCPServer;
public:
    /*!
     * @enum MODE
     * @brief Enumeration of CTCPClient states.
     * @details Modes of connectedness for client socket. MODE_PLAYING
     * is our normal state.
     *
     * Replaces CircleMUD::descriptor_data::connected
     * and CON_X macros in structs.h of CircleMUD.
     * @sa CTCPClient::GetMode()
     *
     * @sa CircleMUD::descriptor_data, CircleMUD::CON_X macros in structs.h
     */
    enum MODE
    {
        MODE_CLOSE                 = 0, /*!< Connection closed */
        MODE_DISCONNECT            = 1,/*!< Linkless player */
        MODE_GET_NAME              = 2, /*!< Prompt for name at login screen */
        MODE_NAME_CONFIRM          = 3, /*!< Confirm name prompt of a new player */
        MODE_PASSWORD              = 4, /*!< Prompt for password at login screen */
        MODE_USER_PARSE            = 5  /*!< User parser is parsing any input */
    };
    enum { NUM_MODES = MODE_USER_PARSE + 1 };

private:
    /* CircleMUD::txt_block */
    struct TextBlock
    {
        char* Text = nullptr;
        int Aliased = 0;
    };

    /* CircleMUD::txt_q */
    struct TextQueue
    {
        TextBlock* Head = nullptr;
        TextBlock* Tail = nullptr;
    };
private:
    MODE mode = MODE_CLOSE;         // CircleMUD::descriptor_data::connected
    string host = "";               // CircleMUD::descriptor_data::host
    unsigned short badPasswords = 0;// CircleMUD::descriptor_data::bad_pws
    time_t loginTime = 0;           // CircleMUD::descriptor_data::login_time
    weak_ptr<CTCPClient> snooping;  // CircleMUD::descriptor_data::snooping
    weak_ptr<CTCPClient> snoopedBy; // CircleMUD::descriptor_data::snoop_by
    weak_ptr<CUser> user;
    weak_ptr<CUser> original;       // For switched users
    bool hasPrompt = true;
    string currentInput = "";
private:
    CTCPClient(): CSocket(), mode(MODE_CLOSE), host(""), badPasswords(0),
        loginTime(0), snooping(), snoopedBy(), user(), hasPrompt(true), currentInput("")
    {
    }
private:
    CTCPClient(const pm_descriptor& pDescriptor,
               const struct sockaddr_in& pSockAddress): CSocket(pDescriptor, pSockAddress),
        mode(MODE_CLOSE), host(""), badPasswords(0), loginTime(0),
        snooping(), snoopedBy(), user(),
        hasPrompt(true), currentInput("")
    {
    }

public:
    virtual ~CTCPClient()
    {
        // TODO add close.
    }
    /*!
     * @fn GetMode()
     * @brief The mode of our player socket represented by CTCPClient::MODE.
     * @details The mode of our player socket represented by CTCPClient::MODE.
     *
     * Replaces CircleMUD::descriptor_data::connected variable.
     * @return const CTCPClient::MODE& CTCPClient::mode
     * @sa CTCPClient::MODE, CTCPClient::mode
     *
     * @sa CircleMUD::descriptor_data, CircleMUD CON_X macros in structs.h
     */
    const MODE& GetMode() const noexcept
    {
        return mode;
    }

    const string& GetHost() const noexcept
    {
        return host;
    }

    const unsigned short& GetBadPasswords() const noexcept
    {
        return badPasswords;
    }

    const time_t& GetLoginTime() const noexcept
    {
        return loginTime;
    }

    const weak_ptr<CTCPClient>& GetSnooping() const noexcept
    {
        return snooping;
    }

    const weak_ptr<CTCPClient>& GetSnoopedBy() const noexcept
    {
        return snoopedBy;
    }

private:
    bool InitializeBegin();
    bool InitializeMiddle();
    bool InitializeEnd();
    void ParseInput();
    int ProcessInput();
    int ProcessOutput();
    bool GetFromInputQueue();
    void HostLookup();
};
}

#endif //PIRPLEMUD_CTCPCLIENT_H
