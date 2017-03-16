#include "painter.h"

#include "utilities.h"

#include <QString>

Painter::Painter()
{

}

Painter::~Painter()
{

}

bool Painter::buildShaderProgram(const QString & strVertFile, const QString & strFragFile)
{
    initializeOpenGLFunctions();
    QString vertStr = Utilities::readStringFromFile(strVertFile);
    QString fragStr = Utilities::readStringFromFile(strFragFile);
    m_Program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    m_Program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    return m_Program.link();
}
