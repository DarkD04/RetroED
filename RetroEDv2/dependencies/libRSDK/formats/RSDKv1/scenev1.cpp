#include "libRSDK.hpp"

#include "scenev1.hpp"

void RSDKv1::Scene::read(Reader &reader)
{
    filePath = reader.filePath;

    // Map width in 128 pixel units
    // In RSDKv1, it's one byte long
    width  = reader.read<byte>();
    height = reader.read<byte>();

    layout.reserve(height);
    for (int y = 0; y < height; ++y) {
        layout.append(QList<byte>());
        layout[y].reserve(width);
        for (int x = 0; x < width; ++x) {
            // 128x128 Block number is 8-bit in RSDKv1
            layout[y].append(reader.read<byte>());
        }
    }

    QString itmpath = QFileInfo(reader.filePath).absolutePath() + "/"
                      + QFileInfo(reader.filePath).baseName() + ".itm";
    Reader ITMreader(itmpath);

    title = ITMreader.readString();

    musicID      = ITMreader.read<byte>();
    backgroundID = ITMreader.read<byte>();
    playerXPos   = (short)(ITMreader.read<byte>() << 8);
    playerXPos |= ITMreader.read<byte>();
    playerYPos = (short)(ITMreader.read<byte>() << 8);
    playerYPos |= ITMreader.read<byte>();

    // Read number of object types, Only RSDKv2 and RSDKv3 support this feature
    // 2 bytes, big-endian, unsigned
    int objCount = ITMreader.read<byte>() << 8;
    objCount |= ITMreader.read<byte>();

    for (int n = 0; n < objCount; ++n) {
        entities.append(Entity(ITMreader, n));
    }
}

void RSDKv1::Scene::write(Writer &writer)
{
    filePath = writer.filePath;

    // Write width and height
    writer.write(width);
    writer.write(height);

    // Write tile map
    for (int h = 0; h < height; ++h) {
        for (int w = 0; w < width; ++w) {
            writer.write(layout[h][w]);
        }
    }
    writer.flush();

    bool useDCFormat = writer.filePath.contains("DATA/LEVELS");
    QString itmpath = QFileInfo(writer.filePath).absolutePath() + "/"
                      + QFileInfo(writer.filePath).baseName() + (useDCFormat ? ".ITM" : ".itm");
    Writer ITMwriter(itmpath);
    // Write zone name
    ITMwriter.write(title);

    // Write the Stage Init Data
    ITMwriter.write(musicID);
    ITMwriter.write(backgroundID);
    ITMwriter.write((byte)(playerXPos >> 8));
    ITMwriter.write((byte)(playerXPos & 0xFF));
    ITMwriter.write((byte)(playerYPos >> 8));
    ITMwriter.write((byte)(playerYPos & 0xFF));

    // Write number of objects
    ITMwriter.write((byte)(entities.count() >> 8));
    ITMwriter.write((byte)(entities.count() & 0xFF));

    std::sort(entities.begin(), entities.end(),
              [](const Entity &a, const Entity &b) -> bool { return a.slotID < b.slotID; });

    // Write object data
    for (Entity &obj : entities) {
        obj.write(ITMwriter);
    }

    ITMwriter.flush();
}
