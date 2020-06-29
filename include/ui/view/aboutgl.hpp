#ifndef INCLUDE_UI_VIEW_ABOUTGL_HPP_NXI
#define INCLUDE_UI_VIEW_ABOUTGL_HPP_NXI

#include <QWidget>

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
#include <Qt3DExtras/QText2DEntity>
#include <Qt3DExtras/QExtrudedTextMesh>

#include <QPropertyAnimation>

#include "qt3dwindow.h"
#include "qorbitcameracontroller.h"

#include <ui/system/session.hpp>
#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <QtWidgets/QHBoxLayout>
#include <include/nxw/hbox_layout.hpp>

namespace ui::views
{
    class aboutgl : public QWidget
    {
    public:
        aboutgl(ui::session& session) : session_{ session }
        {
            /*
            Qt3DExtras::Qt3DWindow* view = new Qt3DExtras::Qt3DWindow();
            view->defaultFrameGraph()->setClearColor(QColor(QRgb(0xFFFFFF)));

            QWidget* container = QWidget::createWindowContainer(view);

            QWidget *widget = new QWidget;
            auto *layout = new nxw::hbox_layout(widget);
            layout->addWidget(container, 1);
            setLayout(layout);

            // Root entity
            Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();

            // Camera
            Qt3DRender::QCamera *cameraEntity = view->camera();

            cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
            cameraEntity->setPosition(QVector3D(0, 0, 20.0f));
            cameraEntity->setUpVector(QVector3D(0, 1, 0));
            cameraEntity->setViewCenter(QVector3D(0, 0, 0));



            view->setRootEntity(rootEntity);



            // Sphere shape data
            Qt3DExtras::QSphereMesh *sphereMesh = new Qt3DExtras::QSphereMesh();
            sphereMesh->setRings(20);
            sphereMesh->setSlices(20);
            sphereMesh->setRadius(2);

            // Sphere mesh transform
            Qt3DCore::QTransform *sphereTransform = new Qt3DCore::QTransform();

            sphereTransform->setScale(1.3f);
            sphereTransform->setTranslation(QVector3D(-5.0f, -4.0f, 0.0f));

            Qt3DExtras::QPhongMaterial *sphereMaterial = new Qt3DExtras::QPhongMaterial();
            sphereMaterial->setDiffuse(QColor(QRgb(0xa69929)));

            // Sphere
            auto m_sphereEntity = new Qt3DCore::QEntity(rootEntity);
            m_sphereEntity->addComponent(sphereMesh);
            m_sphereEntity->addComponent(sphereMaterial);
            m_sphereEntity->addComponent(sphereTransform);

            auto text = new Qt3DCore::QEntity(rootEntity);

            auto *textMaterial = new Qt3DExtras::QPhongMaterial(rootEntity);
            textMaterial->setDiffuse(QColor(0,100,0));

            auto *textTransform = new Qt3DCore::QTransform();
            textTransform->setTranslation(QVector3D(-5, 0, 0));
            textTransform->setScale(2.5f);

            auto textMesh = new Qt3DExtras::QExtrudedTextMesh();
            textMesh->setText("nxi browser");
            textMesh->setDepth(.01f);

            text->addComponent(textMaterial);
            text->addComponent(textTransform);
            text->addComponent(textMesh);
             */
        }

    private:
        ui::session& session_;

    };
} // nxi::views

#endif // INCLUDE_UI_VIEW_ABOUTGL_HPP_NXI
