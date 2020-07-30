/*!
 * @file CSocket.h
 * @brief The base socket for the server and client sockets.
 * @details The abstract socket class that is the base for
 * client and server sockets.
 *
 * Takes some of the data from CircleMUD::descriptor_data and some
 * of the data from the server socket in CircleMUD comm.c.
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 * @sa CSocket.cpp, CTCPServer.h, CTCPClient.h
 *
 * @sa CircleMUD::descriptor_data, CircleMUD comm.c
 */

#ifndef PIRPLEMUD_CSOCKET_H
#define PIRPLEMUD_CSOCKET_H

#include "pirple_.h"
#include <netinet/in.h>
#include <fcntl.h>

namespace Pirple
{
/*!
 * @class CSocket
 * @brief Our base virtual socket class for servers and clients
 * @details The virtual base class for CTCPClient and CTCPServer
 * sockets for the game.
 *
 * CSocket holds some of the data from CircleMUD comm.c
 * as well as CircleMUD::descriptor_data and socket related functions.
 * @sa CTCPServer, CTCPClient
 *
 * @sa CircleMUD::descriptor_data, CircleMUD comm.c
 */
class CSocket
{
protected:
    /*!
     * @var descriptor
     * @sa CSocket::GetDescriptor(), SOCKET_CLOSED
     *
     * @sa CircleMUD::descriptor_data
     */
    pm_descriptor descriptor = SOCKET_CLOSED;
    bool initialized = false;
    sockaddr_in socketAddress;
public:
    CSocket(): descriptor(SOCKET_CLOSED), initialized(false), socketAddress()
    {
    }
    CSocket(const pm_descriptor& pDescriptor, const sockaddr_in& pSockAddr): descriptor(pDescriptor), initialized(false),
        socketAddress(pSockAddr)
    {

    }

    virtual ~CSocket()
    {
        // TODO add Close() call of some sort.
    }

    virtual int ProcessInput() = 0;
    virtual int ProcessOutput() = 0;
public:
    /*!
     * @fn GetDescriptor()
     * @brief The file descriptor for our socket.
     * @details Replaces CircleMUD::descriptor_data::descriptor and
     * CircleMUD::mother_desc from comm.c.
     * @return const pm_descriptor& CSocket::descriptor @ref SOCKET_CLOSED if none.
     * @sa CSocket::descriptor, SOCKET_CLOSED
     *
     * @sa CircleMUD::descriptor_data
     */
    inline const pm_descriptor& GetDescriptor() const noexcept
    {
        return descriptor;
    }

    inline const bool& IsInitialized() const noexcept
    {
        return initialized;
    }

protected:
    bool SetNonBlocking() const;
    bool InitializeBuffers() const;
    virtual bool Close();
};

}

#endif //PIRPLEMUD_CSOCKET_H
