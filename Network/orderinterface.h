#pragma once

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"

class QStringList;

class OrderInterface
{
public:
    OrderInterface();
    ~OrderInterface();
public:
    virtual void signal_respConnect(ConnectProto::pbRespConnect resp) = 0;
    virtual void signal_respDevices(const QStringList& devicesList) = 0;
    virtual void signal_respStart(KinectDataProto::pbRespStart protoRespStart) = 0;
    virtual void signal_reqEndConnect() = 0;

    virtual void signal_hasBeenConnected() = 0;
};
