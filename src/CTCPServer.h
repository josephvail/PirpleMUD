/*!
 * @file CTCPServer.h
 * @brief The Server for game sockets, or possibly one of multiple servers.
 * @details The server socket accepting client connections from players.
 *
 * Most of the code adapted from CircleMUD comm.c.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa CTCPServer.cpp, CTCPClient.h, CSocket.h
 *
 * @sa CircleMUD comm.c, CircleMUD structs.h
 */

#ifndef PIRPLEMUD_CTCPSERVER_H
#define PIRPLEMUD_CTCPSERVER_H

#include "CSocket.h"
#include "CTCPClient.h"

namespace Pirple
{
/*!
 * @class CTCPServer
 * @brief Our server object that manages all client connections.
 * @details CTCPServer is the server socket that manages all CTCPClient objects.
 *
 * CTCPServer holds some of the data from CircleMUD comm.c
 * as well as CircleMUD::descriptor_data and socket related functions.
 * @sa CSocket, CTCPClient
 *
 * @sa CircleMUD::descriptor_data, CircleMUD comm.c
 */
class CTCPServer: public CSocket
{
    friend class CMUD;
private:
    in_addr bindAddress;
    pm_ushint port = 4444;
    list<shared_ptr<CTCPClient>> clients;
    list<shared_ptr<CTCPClient>> closeClients;
    fd_set inputSet;
    fd_set outputSet;
    fd_set exceptionSet;
    pm_descriptor maxDescriptor = 0;
public:
    virtual ~CTCPServer()
    {
        clients.clear();
        closeClients.clear();
    }

    const pm_ushint& GetPort() const noexcept
    {
        return port;
    }
    /*!
     * @fn SetPort(const pm_ushint& pPort)
     * @brief Set CTCPServer::port if CTCPServer::IsInitialized() is false only
     * @param pPort const pm_ushint&
     * @return bool true if CTCPServer::port set, false if failed due to CTCPServer::IsInitialized().
     * @sa CTCPServer::port, CTCPServer::GetPort(), CTCPServer::IsInitialized(), CTCPServer::Initialize()
     *
     * @sa CircleMUD comm.c
     */
    inline bool SetPort(const pm_ushint& pPort) noexcept
    {
        if (IsInitialized())
            return false;
        port = pPort;
        return true;
    }

    bool Initialize(const pm_ushint& pPort);

    bool SleepUntil(struct timeval& pLastTime);
    bool PrepareFDSets();
    bool Poll();
    bool AcceptNewClients();
    bool TransferClosingClients();
    int ProcessInput();
    int ProcessOutput();
    bool ClosePendingClients();
private:
    bool ConnectNewClient();
    bool Bind();
};
}

#endif //PIRPLEMUD_CTCPSERVER_H
