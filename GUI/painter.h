#ifndef PAINTER_H
#define PAINTER_H

#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>

class QString;

class Painter : protected QOpenGLFunctions_4_3_Core
{
public:
    Painter();
    virtual ~Painter();

public:
    virtual bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile);
    virtual void paint() = 0;
protected:
    QOpenGLShaderProgram m_Program;
private:
};

#endif // PAINTER_H
