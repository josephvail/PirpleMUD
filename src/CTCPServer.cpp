/*!
 * @file CTCPServer.cpp
 * @brief See CTCPServer.h
 * @details See CTCPServer.h
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa CTCPServer.h, CTCPClient.h, CSocket.h, CMUD.h
 */

#include <sys/time.h>
#include "PirpleMUD.h"

namespace Pirple
{
bool CTCPServer::Initialize(const pm_ushint& pPort)
{
    if (IsInitialized())
        return false;

    int opt;
    if ((descriptor = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("SYSERR: Error creating socket");
        exit(1); // TODO exit replaced with throws?
    }
    opt = 1;
    if (setsockopt(descriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
    {
        perror("SYSERR: setsockopt REUSEADDR");
        exit(1); // TODO exit replaced with throws?
    }

    if (!InitializeBuffers())
        return false;

    linger ld = {};

    ld.l_onoff = 0;
    ld.l_linger = 0;
    if (setsockopt(descriptor, SOL_SOCKET, SO_LINGER, (char *) &ld, sizeof(ld)) < 0)
        perror("SYSERR: setsockopt SO_LINGER");
    if (pPort > 0)
        port = pPort;

    if (!Bind())
        return false;
    listen(descriptor, 5);

    initialized = true;
    return true;
}

bool CTCPServer::Bind()
{
    if (IsInitialized())
        return false;

    /* Clear the structure */
    memset((char *) &bindAddress, 0, sizeof(bindAddress));
    bindAddress.s_addr = htonl(INADDR_ANY);
    /* Put the address that we've finally decided on into the logs */
    MUD.Log("Binding to all IP interfaces on this host.");
    /* Clear the structure */
    memset((char *)&socketAddress, 0, sizeof(socketAddress));

    socketAddress.sin_family = PF_INET;
    socketAddress.sin_port = htons(port);
    socketAddress.sin_addr = bindAddress;

    if (bind(descriptor, (sockaddr *) &socketAddress, sizeof(socketAddress)) < 0)
    {
        perror("SYSERR: bind");

        Close();
        exit(1); // TODO throw instead of exit?
    }

    return true;
}

bool CTCPServer::SleepUntil(struct timeval& pLastTime)
{
    /* Sleep if we don't have any connections */
    if (clients.empty())
    {
        MUD.Log("No connections.  Going to sleep.");
        FD_ZERO(&inputSet);
        FD_SET(descriptor, &inputSet);
        if (select(descriptor + 1, &inputSet, nullptr, nullptr, nullptr) < 0)
        {
//TODO                if (select(descriptor + 1, &inputSet, (fd_set *) 0, (fd_set *) 0, NULL) < 0) {
            if (errno == EINTR)
                MUD.Log("Waking up to process signal.");
            else
                perror("SYSERR: Select coma");
        }
        else
            MUD.Log("New connection.  Waking up.");
        gettimeofday(&pLastTime, nullptr);
//TODO            gettimeofday(&pLastTime, (struct timezone *) 0);
    }
    return true;
}

bool CTCPServer::PrepareFDSets()
{
    /* Set up the input, output, and exception sets for select(). */
    FD_ZERO(&inputSet);
    FD_ZERO(&outputSet);
    FD_ZERO(&exceptionSet);
    FD_SET(descriptor, &inputSet);

    maxDescriptor = descriptor;
    for(auto client: clients)
    {
        if (client->GetDescriptor() > maxDescriptor)
            maxDescriptor = client->GetDescriptor();
        FD_SET(client->GetDescriptor(), &inputSet);
        FD_SET(client->GetDescriptor(), &outputSet);
        FD_SET(client->GetDescriptor(), &exceptionSet);
    }

    return true;
}

bool CTCPServer::Poll()
{
    timeval lTime = {};
    lTime.tv_sec = 0;
    lTime.tv_usec = 0;
    /* Poll (without blocking) for new input, output, and exceptions */
    if (select(maxDescriptor + 1, &inputSet, &outputSet, &exceptionSet, &lTime) < 0)
    {
        perror("SYSERR: Select poll");
        return false;
    }
    return true;
}

bool CTCPServer::AcceptNewClients()
{
    /* If there are new connections waiting, accept them. */
    if (FD_ISSET(descriptor, &inputSet))
    {
        if (!ConnectNewClient())
            return false;
    }

    return true;
}

bool CTCPServer::ConnectNewClient()
{
    pm_descriptor desc = INVALID_SOCKET;
    socklen_t i = 0;
    sockaddr_in peer = {};

    /* accept the new connection */
    i = sizeof(peer);
    if ((desc = accept(descriptor, (sockaddr *) &peer, &i)) == INVALID_SOCKET)
    {
        perror("SYSERR: accept");
        return (false);
    }

    auto client = make_shared<CTCPClient>(CTCPClient(desc, peer));
    if (!client->InitializeBegin())
        return false;
    if (clients.size() >= MUD.GetMaxClients())
    {
        /* TODO
        write_to_descriptor(desc, "Sorry, PirpleMUD is full right now... please try again later!\r\n");
        CLOSE_SOCKET(desc);
         */
        return false;
    }
    if (!client->InitializeMiddle())
        return false;

    if (MUD.HostBanned(client->GetHost()) == CMUD::SBan::ALL)
    {
// TODO            mudlog(CMP, LVL_GOD, TRUE, "Connection attempt denied from [%s]", newd->host);
        return true;
    }

    if (!client->InitializeEnd())
        return false;

    clients.push_back(client);

    return (true);
}

bool CTCPServer::TransferClosingClients()
{
    /* Kick out the freaky folks in the exception set and marked for close */
    for(auto iter = clients.begin(); iter != clients.end();)
    {
        auto client = *iter;
        if (FD_ISSET(client->GetDescriptor(), &exceptionSet) || client->GetMode() == CTCPClient::MODE_CLOSE
                || client->GetMode() == CTCPClient::MODE_DISCONNECT)
        {
            if (FD_ISSET(client->GetDescriptor(), &exceptionSet))
            {
                FD_CLR(client->GetDescriptor(), &inputSet);
                FD_CLR(client->GetDescriptor(), &outputSet);
            }
            iter = clients.erase(iter);
            closeClients.push_back(client);
        }
        else
            ++iter;
    }
    return true;
}

int CTCPServer::ProcessInput()
{
    /* Process descriptors with input pending */
    for(auto iter = clients.begin(); iter != clients.end(); )
    {
        if (FD_ISSET((*iter)->GetDescriptor(), &inputSet))
        {
            if ((*iter)->ProcessInput() < 0)
            {
                closeClients.push_back(*iter);
                iter = clients.erase(iter);
            }
        }
        ++iter;
    }

    for(auto client: clients)
    {
        client->ParseInput();
    }

    return 1;
}

int CTCPServer::ProcessOutput()
{
    /* Send queued output out to the operating system (ultimately to user). */
    for(auto client: clients)
    {
        /* TODO
        if (client->HasOutput() && FD_ISSET(client->GetDescriptor(), &outputSet)) {
            client->ProcessOutput();
        // TODO                if (d->bufptr == 0)	// All output sent.
        // TODO                    d->has_prompt = TRUE;
        }*/
    }

    /* Print prompts for other descriptors who had no other output */
    /* TODO
        for (d = descriptor_list; d; d = d->next) {
            if (!d->has_prompt && d->bufptr == 0) {
                write_to_descriptor(d->descriptor, make_prompt(d));
                d->has_prompt = TRUE;
            }
        }*/

    return 1;
}

bool CTCPServer::ClosePendingClients()
{
    while(!closeClients.empty())
    {
        auto client = *closeClients.begin();
        closeClients.erase(closeClients.begin());
    }
    return true;
}
}
