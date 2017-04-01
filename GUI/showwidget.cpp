#include "showwidget.h"

#include "transfersocketthread.h"
#include "imagetransfersocket.h"

#include "config.h"

#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QDebug>

namespace
{
    static const QString IP_ADRESS = "127.0.0.1";
}

ShowWidget::ShowWidget(QString title, Utilities::ShowType type, QWidget *parent)
    : QOpenGLWidget(parent),
    m_strTitle(title),
    m_eShowType(type),
    m_bFirstTime(true),
    m_fAspectRatio(4.0 / 3.0),
    m_pColorPainter(nullptr),
    m_pDepthPainter(nullptr),
    m_pSkelePainter(nullptr),
    m_pTimer(nullptr),
    m_pMenu(nullptr),
    m_bShowColor(false),
    m_bShowDepth(false),
    m_bShowSkele(false)
{
    setWindowTitle(m_strTitle);
    setMinimumSize(320, 240);
    setContextMenuPolicy(Qt::CustomContextMenu);
    createMenu();

    connect(this, &ShowWidget::signal_connectedToServer, this, &ShowWidget::slot_connectedToServer);
    connect(this, &ShowWidget::customContextMenuRequested, this, &ShowWidget::slot_customContextMenuRequested);

    /*
    * 如服务器在5秒内 没有对客户端的请求开始传输做出回应
    * 则关闭该窗口,终止向服务器请求传输
    */
    m_pTimerCloseSelf = new QTimer(this);
    m_pTimerCloseSelf->setSingleShot(true);
    connect(m_pTimerCloseSelf, &QTimer::timeout, [=] {
        close();
    });
    m_pTimerCloseSelf->start(5000);

    //m_pTimer = new QTimer(this);
    //connect(m_pTimer, &QTimer::timeout, this, &ShowWidget::slot_update);
    //m_pTimer->start(40);
}

ShowWidget::~ShowWidget()
{
    for (auto a : m_Type_Socket)
    {
        delete a;
    }
    m_Type_Socket.clear();

    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }

    if (m_pColorPainter)
    {
        delete m_pColorPainter;
        m_pColorPainter = nullptr;
    }
    if (m_pDepthPainter)
    {
        delete m_pDepthPainter;
        m_pDepthPainter = nullptr;
    }
    if (m_pSkelePainter)
    {
        delete m_pSkelePainter;
        m_pSkelePainter = nullptr;
    }
}

void ShowWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_NICEST);

    //glEnable(GL_MULTISAMPLE);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

    /* 只有在第一次show之后才会调用initializeGL(), 所以画图对象在此处创建 */
    m_pColorPainter = new ImagePainter;
    m_pDepthPainter = new ImagePainter;
    m_pSkelePainter = new FramePainter;
    m_pColorPainter->buildShaderProgram("image.vert", "image.frag");
    m_pDepthPainter->buildShaderProgram("image.vert", "image.frag");
    m_pSkelePainter->buildShaderProgram("frame.vert", "frame.frag");
}

void ShowWidget::paintGL()
{
    resizeViewPort();
    glClear(GL_COLOR_BUFFER_BIT);
    if (m_pColorPainter && m_bShowColor)
    {
        m_pColorPainter->paint();
    }
    if (m_pDepthPainter && m_bShowDepth)
    {
        m_pDepthPainter->paint();
    }
    if (m_pSkelePainter && m_bShowSkele)
    {
        m_pSkelePainter->paint();
    }
}

void ShowWidget::resizeGL(int w, int h)
{
}

/*
* 自定义调整视口大小
* 让图像按照原比例显示中窗口中间
*/
void ShowWidget::resizeViewPort()
{
    float w = this->width();
    float h = this->height();
    float fAspectRatio = (float) w / h;
    if (fAspectRatio > m_fAspectRatio)  // 窗口的宽度比图片的宽度大
    {
        int x = (w - h * m_fAspectRatio) / 2;
        glViewport(x, 0, h * m_fAspectRatio, h);
    }
    else if (fAspectRatio < m_fAspectRatio) // 窗口的宽度比图片的宽度小
    {
        int y = (h - w / m_fAspectRatio) / 2;
        glViewport(0, y, w, w / m_fAspectRatio);
    }
    else
    {
        glViewport(0, 0, w, h);
    }
}

void ShowWidget::createMenu()
{
    if (nullptr == m_pMenu)
    {
        m_pMenu = new QMenu(this);
    }
    m_pMenu->clear();
    if (m_eShowType & Utilities::ShowType::Color)
    {
        QAction *p = new QAction("Show Color", this);
        m_bShowColor = true;
        p->setCheckable(true);
        p->setChecked(true);
        connect(p, &QAction::toggled, [&](bool flag) {
            m_bShowColor = flag;
        });
        m_pMenu->addAction(p);
        p = nullptr;
    }

    if (m_eShowType & Utilities::ShowType::Depth)
    {
        QAction *p = new QAction("Show Depth", this);
        m_bShowDepth = true;
        p->setCheckable(true);
        p->setChecked(true);
        connect(p, &QAction::toggled, [&](bool flag) {
            m_bShowDepth = flag;
        });
        m_pMenu->addAction(p);
        p = nullptr;
    }

    if (m_eShowType & Utilities::ShowType::Skele)
    {
        QAction *p = new QAction("Show Skele", this);
        m_bShowSkele = true;
        p->setCheckable(true);
        p->setChecked(true);
        connect(p, &QAction::toggled, [&](bool flag) {
            m_bShowSkele = flag;
        });
        m_pMenu->addAction(p);
        p = nullptr;
    }
}

void ShowWidget::KeepThisWidget()
{
    if (m_pTimerCloseSelf)
    {
        m_pTimerCloseSelf->stop();
        delete m_pTimerCloseSelf;
        m_pTimerCloseSelf = nullptr;
    }
}

void ShowWidget::closeEvent(QCloseEvent * event)
{
    emit signal_closed(m_strTitle);
}

void ShowWidget::createTransferSocketThreads()
{
    TransferSocketThread *pSocketThread = nullptr;
    if (m_eShowType & Utilities::ShowType::Color)
    {
        pSocketThread = new TransferSocketThread { m_strTitle, Utilities::SocketType::Color,
            Config::GetInstance()->IPAdress(), Config::GetInstance()->colorPort(), this };
        m_Type_Socket.insert(Utilities::ShowType::Color, pSocketThread);
    }

    if (m_eShowType & Utilities::ShowType::Depth)
    {
        pSocketThread = new TransferSocketThread { m_strTitle, Utilities::SocketType::Depth,
            Config::GetInstance()->IPAdress(), Config::GetInstance()->depthPort(), this };
        m_Type_Socket.insert(Utilities::ShowType::Depth, pSocketThread);
    }

    if (m_eShowType & Utilities::ShowType::Skele)
    {
        pSocketThread = new TransferSocketThread { m_strTitle, Utilities::SocketType::Skele,
            Config::GetInstance()->IPAdress(), Config::GetInstance()->skelePort(), this };
        m_Type_Socket.insert(Utilities::ShowType::Skele, pSocketThread);
    }
    pSocketThread = nullptr;
}

void ShowWidget::slot_customContextMenuRequested(QPoint point)
{
    if (m_pMenu)
    {
        m_pMenu->exec(QCursor::pos());
    }
}

void ShowWidget::updateColor()
{
    if (false == m_Type_Socket.contains(Utilities::ShowType::Color))
    {
        return;
    }

    if (m_Type_Socket[Utilities::ShowType::Color] && m_Type_Socket[Utilities::ShowType::Color]->getTransferSocketPtr())
    {
        ImageTransferSocket* pSocket = (ImageTransferSocket*) m_Type_Socket[Utilities::ShowType::Color]->getTransferSocketPtr();
        if (m_bFirstTime)
        {
            if (pSocket && pSocket->matsSize() > 0)
            {
                cv::Mat mat = pSocket->popMat();
                m_fAspectRatio = (float) mat.cols / mat.rows;
                m_bFirstTime = false;
            }
        }

        if (m_pColorPainter && pSocket && pSocket->matsSize() > 0)
        {
            qDebug() << __FILE__ << __LINE__ <<  pSocket->matsSize();
            m_pColorPainter->loadTexture(pSocket->popMat());
        }

        pSocket = nullptr;
    }
}

void ShowWidget::updateDepth()
{
    if (false == m_Type_Socket.contains(Utilities::ShowType::Depth))
    {
        return;
    }

    if (m_Type_Socket[Utilities::ShowType::Depth] && m_Type_Socket[Utilities::ShowType::Depth]->getTransferSocketPtr())
    {
        ImageTransferSocket* pSocket = (ImageTransferSocket*) m_Type_Socket[Utilities::ShowType::Depth]->getTransferSocketPtr();
        if (m_bFirstTime)
        {
            if (pSocket && pSocket->matsSize() > 0)
            {
                cv::Mat mat = pSocket->popMat();
                m_fAspectRatio = (float) mat.cols / mat.rows;
                m_bFirstTime = false;
            }
        }

        if (m_pDepthPainter && pSocket && pSocket->matsSize() > 0)
        {
            m_pDepthPainter->loadTexture(pSocket->popMat());
        }

        pSocket = nullptr;
    }
}

void ShowWidget::updateSkele()
{
    if (m_pSkelePainter)
    {
        m_pSkelePainter->loadFrame();
    }
}

void ShowWidget::slot_connectedToServer()
{
    firstTimeShow();
}

void ShowWidget::firstTimeShow()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ShowWidget::slot_update);
    m_pTimer->start(20);
}

void ShowWidget::slot_update()
{
    if (m_eShowType & Utilities::ShowType::Color)
    {
        makeCurrent();
        updateColor();
    }

    if (m_eShowType & Utilities::ShowType::Depth)
    {
        makeCurrent();
        updateDepth();
    }

    if (m_eShowType & Utilities::ShowType::Skele)
    {
        makeCurrent();
        updateSkele();
    }
    update();
}