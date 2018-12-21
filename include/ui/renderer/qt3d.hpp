#ifndef UI_RENDERER_QT3D_H_NXI
#define UI_RENDERER_QT3D_H_NXI

#include <ui/renderer.hpp>

#include <QGuiApplication>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QPaintedTextureImage>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QTextureMaterial>
#include <Qt3DExtras/QNormalDiffuseSpecularMapMaterial>
#include <Qt3DExtras/QPlaneMesh>

#include <QPropertyAnimation>

#include "qt3dwindow.h"
#include "qorbitcameracontroller.h"

#include <include/nxi/page/node.hpp>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineSettings>

#include <QPainter>

namespace ui
{
    class image_texture : public Qt3DRender::QPaintedTextureImage
    {
    public:
        image_texture(const QImage& image) : image_{ image }
        {

        }

        void paint(QPainter* painter) override
        {
            painter->fillRect(0, 0, 100, 100, QColor(255, 255, 255));
            painter->drawImage(image_.rect(), image_);
        }

        QImage image_;
    };



    class qt3d_renderer : public renderer
    {
    public:
        qt3d_renderer()
        {
            QWebEngineView* w = new QWebEngineView(this);
            w->setFixedSize(800, 600);
            w->page()->load(QUrl("https://www.youtube.com"));
            w->setAttribute(Qt::WA_DontShowOnScreen);
            //w->show();

            container_ = w;
        }

        void display(Qt3DCore::QEntity* p)
        {

        }
        QWidget* widget() { return container_; }

        ~qt3d_renderer()
        {

        }

    private:
        QWidget* container_;
        Qt3DExtras::Qt3DWindow* view_;
    };
} // ui

#endif // UI_RENDERER_QT3D_H_NXI