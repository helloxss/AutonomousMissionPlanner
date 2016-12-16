#include "autonomousvehicleproject.h"

#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "backgroundraster.h"
#include "waypoint.h"
#include "trackline.h"
#include <gdal_priv.h>


AutonomousVehicleProject::AutonomousVehicleProject(QObject *parent) : QObject(parent)
{
    GDALAllRegister();

    m_model = new QStandardItemModel(this);
    m_scene = new QGraphicsScene(this);
    m_model->setObjectName("projectModel");
    QStandardItem *item = new QStandardItem("Background");
    m_model->setItem(0,item);
    topLevelItems["Background"] = item;
    item = new QStandardItem("Platform");
    m_model->setItem(1,item);
    topLevelItems["Platform"] = item;
    item = new QStandardItem("Mission");
    m_model->setItem(2,item);
    topLevelItems["Mission"] = item;
}

AutonomousVehicleProject::~AutonomousVehicleProject()
{
}

QStandardItemModel *AutonomousVehicleProject::model() const
{
    return m_model;
}

QGraphicsScene *AutonomousVehicleProject::scene() const
{
    return m_scene;
}

void AutonomousVehicleProject::openBackground(const QString &fname)
{
    BackgroundRaster *bgr = new BackgroundRaster(fname, this);
    QStandardItem *item = new QStandardItem(fname);
    item->setData(QVariant::fromValue<BackgroundRaster*>(bgr));
    topLevelItems["Background"]->appendRow(item);
    m_scene->addItem(bgr);
}

BackgroundRaster *AutonomousVehicleProject::getBackgroundRaster() const
{
    if(m_model)
    {
        QModelIndex bgindex = m_model->index(0,0,m_model->index(0,0));
        return m_model->data(bgindex,Qt::UserRole+1).value<BackgroundRaster*>();
    }
    return 0;
}

void AutonomousVehicleProject::addWaypoint(QGeoCoordinate position, BackgroundRaster *parentItem)
{
    Waypoint *wp = new Waypoint(this,parentItem);
    wp->setLocation(position);
    wp->setPos(parentItem->geoToPixel(position));
    QStandardItem *item = new QStandardItem("wayoint");
    item->setData(QVariant::fromValue<Waypoint*>(wp));
    topLevelItems["Mission"]->appendRow(item);
    wp->setFlag(QGraphicsItem::ItemIsMovable);
    wp->setFlag(QGraphicsItem::ItemIsSelectable);
    wp->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

TrackLine * AutonomousVehicleProject::addTrackLine(QGeoCoordinate position, BackgroundRaster *parentItem)
{
    TrackLine *tl = new TrackLine(this,parentItem);
    tl->setPos(parentItem->geoToPixel(position));
    QStandardItem *item = new QStandardItem("trackline");
    item->setData(QVariant::fromValue<TrackLine*>(tl));
    topLevelItems["Mission"]->appendRow(item);
    tl->setFlag(QGraphicsItem::ItemIsMovable);
    tl->setFlag(QGraphicsItem::ItemIsSelectable);
    tl->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    tl->addWaypoint(position);

    return tl;
}