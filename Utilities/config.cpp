#include "config.h"


Config::Config()
    : m_strIPAdress("127.0.0.1"),
    m_strGuid(""),
    m_uServerPort(7892),
    m_uColorPort(0),
    m_uDepthPort(0),
    m_uSkelePort(0)
{
}

Config::~Config()
{
}
