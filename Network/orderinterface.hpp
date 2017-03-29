#pragma once

#include "KinectDataProto.pb.h"

class QStringList;

class OrderInterface
{
public:
	OrderInterface();
	~OrderInterface();
public:
    virtual void signal_respConnect() = 0;
    virtual void signal_respDevices(const QStringList& devicesList) = 0;
    virtual void signal_respStartRequire(KinectDataProto::pbRespStart protoRespStart) = 0;
    virtual void signal_reqEndConnect() = 0;

    virtual void signal_hasBeenConnected() = 0;
};
