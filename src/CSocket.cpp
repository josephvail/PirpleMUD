/*!
 * @file CSocket.cpp
 * @brief See CSocket.h
 * @details See CSocket.h
 * @author Joe Vail
 * @copyright Copyright (c) 2018 jvindy software under the PirpleMUD License.@n
 * PirpleMUD is based on CircleMUD, Copyright(c) 1995.@n
 * CircleMUD is based on DikuMUD, Copyright(c) 1990, 1991.@n
 * See @ref LICENSE file for more information.
 */

#include "CSocket.h"

namespace Pirple
{

bool CSocket::SetNonBlocking() const
{
    int flags;

    flags = fcntl(descriptor, F_GETFL, 0);
    flags |= O_NONBLOCK;
    if (fcntl(descriptor, F_SETFL, flags) < 0)
    {
        perror("SYSERR: Fatal error executing CSocket::SetNonBlocking()");
        exit(1); // TODO remove exit and throw instead?
    }
    return true;
}
bool CSocket::InitializeBuffers() const
{
    if (IsInitialized())
        return false;
    // I am not currently changing default system buffer sizes for SO_SNDBUF and SO_RCVBUF
//        int opt = MAX_SOCK_BUF;
    /*
            if (setsockopt(descriptor, SOL_SOCKET, SO_SNDBUF, (char *) &opt, sizeof(opt)) < 0) {
                perror("SYSERR: setsockopt SNDBUF");
                return false;
            }
            if (setsockopt(descriptor, SOL_SOCKET, SO_RCVBUF, (char *) &opt, sizeof(opt)) < 0) {
                perror("SYSERR: setsockopt SNDBUF");
                return false;
            }
    */
    return true;
}

bool CSocket::Close()
{
    close(descriptor);
    initialized = false;
    return true;
}
}
