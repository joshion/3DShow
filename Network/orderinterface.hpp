#ifndef ORDERINTERFACE_H
#define ORDERINTERFACE_H

#pragma once

class QStringList;

class OrderInterface
{
public:
	OrderInterface();
	~OrderInterface();
public:
    virtual void signal_respConnect() = 0;
    virtual void signal_respDevices(const QStringList& devicesList) = 0;
    virtual void signal_respStartRequire() = 0;
    virtual void signal_reqEndConnect() = 0;
};

#endif // ORDERINTERFACE_H