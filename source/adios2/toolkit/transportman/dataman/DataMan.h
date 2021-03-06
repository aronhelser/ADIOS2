/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * DataMan.h
 *
 *  Created on: Jun 1, 2017
 *      Author: Jason Wang wangr1@ornl.gov
 */

#ifndef ADIOS2_TOOLKIT_TRANSPORTMAN_DATAMAN_DATAMAN_H_
#define ADIOS2_TOOLKIT_TRANSPORTMAN_DATAMAN_DATAMAN_H_

#include <queue>
#include <thread>

#include "adios2/core/IO.h"
#include "adios2/core/Operator.h"
#include "adios2/toolkit/format/bp3/BP3.h"
#include "adios2/toolkit/transportman/TransportMan.h"

namespace adios2
{
namespace transportman
{

class DataMan : public TransportMan
{

public:
    DataMan(MPI_Comm mpiComm, const bool debugMode);

    ~DataMan();

    void OpenWANTransports(const std::vector<std::string> &streamNames,
                           const std::vector<Params> &parametersVector,
                           const Mode openMode, const std::string workflowMode,
                           const bool profile);

    void WriteWAN(const std::vector<char> &buffer, size_t transportId);
    void WriteWAN(std::shared_ptr<std::vector<char>> buffer,
                  size_t transportId);

    std::shared_ptr<std::vector<char>> ReadWAN(size_t id);

    void SetMaxReceiveBuffer(size_t size);

private:
    // Objects for buffer queue
    std::vector<std::queue<std::shared_ptr<std::vector<char>>>> m_BufferQueue;
    void PushBufferQueue(std::shared_ptr<std::vector<char>> v, size_t id);
    std::shared_ptr<std::vector<char>> PopBufferQueue(size_t id);
    std::mutex m_Mutex;

    // Functions for parsing parameters
    bool GetBoolParameter(const Params &params, const std::string key);
    bool GetStringParameter(const Params &params, const std::string key,
                            std::string &value);
    bool GetIntParameter(const Params &params, const std::string key,
                         int &value);

    // For read thread
    void ReadThread(std::shared_ptr<Transport> transport);
    std::vector<std::thread> m_ReadThreads;
    bool m_Reading = false;

    // For write thread
    void WriteThread(std::shared_ptr<Transport> transport, size_t id);
    std::vector<std::thread> m_WriteThreads;
    bool m_Writing = false;

    // parameters
    std::vector<Params> m_TransportsParameters;
    size_t m_MaxReceiveBuffer = 256 * 1024 * 1024;
    int m_Timeout = 10;
};

} // end namespace transportman
} // end namespace adios2

#endif /* ADIOS2_TOOLKIT_TRANSPORTMAN_DATAMAN_DATAMAN_H_ */
