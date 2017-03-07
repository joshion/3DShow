#pragma once
#include <string>

class TcpSocketInterface
{
public:
	TcpSocketInterface();
	~TcpSocketInterface();
public:
    virtual void signal_respConnect() = 0;
    virtual void signal_respDevices() = 0;
    virtual void signal_respStartRequire() = 0;
    virtual void signal_reqEndConnect() = 0;
};
