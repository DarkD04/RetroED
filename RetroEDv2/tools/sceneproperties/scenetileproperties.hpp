#pragma once

#include <QWidget>

#include <RSDKv1/tileconfigv1.hpp>
#include <RSDKv5/tileconfigv5.hpp>
#include "tools/sceneviewer.hpp"

namespace Ui
{
class SceneTileProperties;
}

class TileCollisionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TileCollisionWidget(QWidget *parent = nullptr);
    RSDKv5::TileConfig::CollisionMask *cmask = nullptr;
    RSDKv1::TileConfig::CollisionMask *cmaskv1 = nullptr;
    QImage tileImg;
    Vector2<int> DrawPoint1 = (-1, -1);
    Vector2<int> DrawPoint2 = (-1, -1);
    byte paintVer   = 0;
    byte maskIndex  = 0;
    bool mAngleMode = false;

protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void leaveEvent(QEvent *) override;

signals:
    void UpdateCol();
private:
    short selection = -1;
    short highlight = -1;

    bool pressedL = false;
    bool pressedR = false;
    bool enabling = false;
};

class SceneTileProperties : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTileProperties(QWidget *parent = nullptr);
    ~SceneTileProperties();

    void setupUI(ushort tID, QList<QImage> &tiles, SceneViewer *viewer, byte engineVer);

    void setColMask(RSDKv5::TileConfig::CollisionMask *cmA, RSDKv5::TileConfig::CollisionMask *cmB);
    void setColMask(RSDKv1::TileConfig::CollisionMask *cmA, RSDKv1::TileConfig::CollisionMask *cmB);
    void checkChunk(bool valid = false);
    void calcv1Angles(RSDKv5::TileConfig::CollisionMask *outputMask, RSDKv1::TileConfig::CollisionMask *inputMask);
    void tileSelected(ushort tile);
    void unsetUI();

    QPushButton *replaceTile  = nullptr;

signals:
    void updateChunkColMap();
    void updateChunkColTile(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr = 0);
    void updateChunkColTilev1(RSDKv1::TileConfig::CollisionMask *cmask, ushort sel, int colLyr = 0);
private:
    Ui::SceneTileProperties *ui;
    TileCollisionWidget edit;
    QImage tileImg;
    int collisionLyr     =  0;
    int selectedDrawTile =  0;
    byte gameType = ENGINE_v4;

    RSDKv5::TileConfig::CollisionMask *cmask[2] = { nullptr, nullptr };
    RSDKv1::TileConfig::CollisionMask *cmaskv1[2] = { nullptr, nullptr };
};


