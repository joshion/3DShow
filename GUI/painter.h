#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

class QString;

class Painter : protected QOpenGLFunctions_3_3_Core
{
public:
    Painter();
    virtual ~Painter();

public:
    virtual bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile);
    virtual void paint() = 0;
protected:
    QOpenGLShaderProgram m_Program;
};
