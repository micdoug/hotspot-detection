#pragma once

#include <string>
#include <map>
#include <tuple>
#include <memory>
#include "movement.h"

namespace hd {
    class Grid {
        public:
            explicit Grid(const std::string &trace, int square, int timeWindow, int relays, int width=-1, int height=-1);
            virtual ~Grid();
            void detectHotspots();
            void setHeight(int height);
            void setWidth(int width);

        private:
            // Internal methods
            void openTraceFile();
            void readMetaInfo();
            void initializePartitions();
            Movement readNextMovement();

            // Internal properties
            std::string m_fileName;
            std::unique_ptr<std::ifstream> m_file;
            int m_width;
            int m_height;
            int m_square;
            std::map<std::pair<int, int>, int> m_partitions;
            int m_startTime;
            int m_endTime;
            int m_twindow;
            int m_currentTime;
            int m_relays;
    };
}