/*!
 * @file CTCPClient.cpp
 * @brief See CTCPClient.h
 * @details See CTCPClient.h
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 */

#include <netdb.h>
#include <arpa/inet.h>
#include "CTCPServer.h"
#include "CMUD.h"
#include "CUser.h"

namespace Pirple
{

bool CTCPClient::InitializeBegin()
{
    if (IsInitialized())
        return false;
    SetNonBlocking();
    InitializeBuffers();

    return true;
}

bool CTCPClient::InitializeMiddle()
{
    if (IsInitialized())
        return false;
    HostLookup();
    return true;
}
bool CTCPClient::InitializeEnd()
{
    if (IsInitialized())
        return false;
    /* initialize descriptor data */
    /* TODO
            newd->idle_tics = 0;
            newd->output = newd->small_outbuf;
            newd->bufspace = SMALL_BUFSIZE - 1;
            newd->login_time = time(0);
            *newd->output = '\0';
            newd->bufptr = 0;
            newd->has_prompt = 1;  // prompt is part of greetings
            STATE(newd) = CON_GET_NAME;*/

    /*
     * This isn't exactly optimal but allows us to make a design choice.
     * Do we embed the history in descriptor_data or keep it dynamically
     * allocated and allow a user defined history size?
     */
// TODO        CREATE(newd->history, char *, HISTORY_SIZE);

    /* TODO        if (++last_desc == 1000)
                last_desc = 1;
            newd->desc_num = last_desc;
    */
// TODO        write_to_output(newd, "%s", GREETINGS);

    return true;
}

// TODO switch to more recent function such as getaddrinfo. Research IPv6 compatible
void CTCPClient::HostLookup()
{
    char lHost[HOST_LENGTH + 1];
    hostent* from = nullptr;

    /* find the sitename */
    if (MUD.IsRunFlagged(CMUD::RF_SLOW_NAMESERVER) || !(from = gethostbyaddr((char *) &socketAddress.sin_addr,
            sizeof(socketAddress.sin_addr), AF_INET)))
    {

        /* resolution failed */
        if (!MUD.IsRunFlagged(CMUD::RF_SLOW_NAMESERVER))
            perror("SYSERR: gethostbyaddr");

        /* find the numeric site address */
        strncpy(lHost, (char *)inet_ntoa(socketAddress.sin_addr), HOST_LENGTH);	/* strncpy: OK (n->host:HOST_LENGTH+1) */
    }
    else
    {
        strncpy(lHost, from->h_name, HOST_LENGTH);	/* strncpy: OK (n->host:HOST_LENGTH+1) */
    }
    *(lHost + HOST_LENGTH) = '\0';
    host = lHost;
    from = nullptr;
}

/*
 * ASSUMPTION: There will be no newlines in the raw input buffer when this
 * function is called.  We must maintain that before returning.
 *
 * Ever wonder why 'tmp' had '+8' on it?  The crusty old code could write
 * MAX_INPUT_LENGTH+1 bytes to 'tmp' if there was a '$' as the final
 * character in the input buffer.  This would also cause 'space_left' to
 * drop to -1, which wasn't very happy in an unsigned variable.  Argh.
 * So to fix the above, 'tmp' lost the '+8' since it doesn't need it
 * and the code has been changed to reserve space by accepting one less
 * character. (Do you really need 256 characters on a line?)
 * -gg 1/21/2000
 */
int CTCPClient::ProcessInput()
{
    // TODO write function
    return 0;
}

int CTCPClient::ProcessOutput()
{
    // TODO write function
    return 0;
}

bool CTCPClient::GetFromInputQueue()
{
    // TODO write function
    return false;
}

void CTCPClient::ParseInput()
{
    if (!user.expired()) {
        auto suser = user.lock();
        // If the user has a wait state, subtract and wait for next loop
        if (suser->ParseWait())
            return;
    }

    // If the socket has no input wait for the next loop
    if (!GetFromInputQueue())
        return;
    hasPrompt = false;
    // Socket has input and a user, we will set the wait to 1 and reset idle
    if (!user.expired()) {
        auto suser = user.lock();
        suser->ResetWait();
    }
    /* TODO
    if (d->str) // Writing boards, mail, etc.
        string_add(d, comm);
    else if (d->showstr_count) // Reading something w/ pager
        show_string(d, comm);
    else if (STATE(d) != CON_PLAYING) // In menus, etc.
        nanny(d, comm);
    else { // else: we're playing normally.
    }*/
    switch(mode)
    {
    case MODE_GET_NAME:
        // TODO add name parsing
        break;
    case MODE_NAME_CONFIRM:
        // TODO add name confirm parsing
        break;
    case MODE_PASSWORD:
        // TODO add password parsing
        break;
    case MODE_USER_PARSE:
        if (!user.expired()) {
            auto suser = user.lock();
            suser->ParseInput(currentInput);
        }
        else
            ; // TODO add error
        break;
    case MODE_CLOSE:
    case MODE_DISCONNECT:
    default:
        // TODO ERROR LOG
        break;
    }
}
}
