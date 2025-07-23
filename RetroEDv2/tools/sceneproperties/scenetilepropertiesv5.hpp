#pragma once

#include <QWidget>
#include <QDialog>

#include <RSDKv5/tileconfigv5.hpp>

namespace Ui
{
class SceneTilePropertiesv5;
}

class TileCollisionWidgetv5 : public QWidget
{
    Q_OBJECT
public:
    explicit TileCollisionWidgetv5(QWidget *parent = nullptr);
    RSDKv5::TileConfig::CollisionMask *cmask = nullptr;
    QImage tileImg;
    Vector2<int> DrawPoint1 = (-1, -1);
    Vector2<int> DrawPoint2 = (-1, -1);
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

class SceneTilePropertiesv5 : public QWidget
{
    Q_OBJECT

public:
    explicit SceneTilePropertiesv5(QWidget *parent = nullptr);
    ~SceneTilePropertiesv5();

    void setupUI(RSDKv5::TileConfig::CollisionMask *cmA, RSDKv5::TileConfig::CollisionMask *cmB,
                 ushort *tile, QImage tileImg);
    void unsetUI();
    void updatePropFlags(bool c, byte pos);
    int collisionLyr = 0;

    QPushButton *replaceTile  = nullptr;

signals:
    void updateTileFlags(bool c, byte pos);
    void updateTileColMap(RSDKv5::TileConfig::CollisionMask *cmask, ushort sel, int colLyr = 0);
private:
    Ui::SceneTilePropertiesv5 *ui;
    TileCollisionWidgetv5 edit;
    QImage tileImg;

    RSDKv5::TileConfig::CollisionMask *cmask[2] = { nullptr, nullptr };
    int tileID = 0;
    ushort *curTile = nullptr;
};


