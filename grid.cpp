#include "grid.h"
#include <fstream>
#include <iostream>
#include <exception>
#include <cerrno>
#include <cstring>
#include <cmath>
#include <vector>
#include <algorithm>

using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;
using std::make_unique;

hd::Grid::Grid(const std::string &trace, int square, int twindow, int relays, int width, int height)
    : m_square(square), m_width(width), m_height(height), m_fileName(trace), m_twindow(twindow), m_relays(relays)
{   
    m_file = std::make_unique<ifstream>();
}

hd::Grid::~Grid() 
{
    // nothing to do here yet
}

void hd::Grid::openTraceFile()
{
    try {
        m_file->open(m_fileName);
        if (!m_file->is_open()) {
            cerr << "Error while oppening the trace file. " << m_fileName << endl;
            cerr << "Details: " << std::strerror(errno) << endl;
            exit(1);
        }
    }
    catch (std::exception& e) {
        cout << "Error while oppening the trace file. " << endl;
        cout << "Details: " << e.what() << endl;
        exit(1);
    }
}


hd::Movement hd::Grid::readNextMovement()
{
    try {
        hd::Movement mov;
        (*m_file) >> mov;
        return mov;
    }
    catch (std::exception& e) {
        cout << "Error while reading the trace file. " << endl;
        cout << "Details: " << e.what() << endl;
        exit(1);
    }
}

void hd::Grid::readMetaInfo()
{
    (*m_file) >> m_startTime;
    (*m_file) >> m_endTime;
    m_currentTime = m_startTime;
    int minx, maxx, miny, maxy;
    (*m_file) >> minx;
    (*m_file) >> maxx;
    (*m_file) >> miny;
    (*m_file) >> maxy;
    if (m_width == -1) {
        m_width = maxx - minx;
    }
    if (m_height == -1) {
        m_height = maxy - miny;
    }
}

void hd::Grid::detectHotspots() 
{
    this->openTraceFile();
    this->readMetaInfo();
    this->initializePartitions();

    std::map<int, std::pair<int, int>> m_oldstate;
    std::map<int, std::pair<int, int>> m_state;
    long count = 0;

    while(!m_file->eof()) {
        // Advance time window 
        int endTwindow = m_currentTime + m_twindow;
        cout << "Advancing world to " << endTwindow << endl;
        while (m_currentTime <= endTwindow && !m_file->eof()) {
            auto mov = this->readNextMovement();
            m_currentTime = mov.time;
            int x = std::floor(mov.x / m_square);
            int y = std::floor(mov.y / m_square);
            m_state[mov.node] = std::make_pair(x,y);
            count++;   
        }

        cout << "Comparing states " << endl;
        // Compare old and current tstate
        for (const auto &state : m_state) {
            // state.first -> node
            // state.second -> partition
            if (m_oldstate.count(state.first) && m_oldstate[state.first] != state.second) {
                m_partitions[m_oldstate[state.first]]++;
            }
            m_oldstate[state.first] = state.second;
        }
    }
    cout << "Read " << count << " registers" << endl;
    cout << "Preparing to sort partitions ..." << endl;
    std::vector<std::pair<int, std::pair<int, int>>> pvalues;
    for (const auto &partition: m_partitions) {
        pvalues.push_back(std::make_pair(partition.second, partition.first));
    }
    std::sort(pvalues.begin(), pvalues.end(), std::greater<std::pair<int, std::pair<int, int>>>());
    cout << endl << endl;
    size_t limit = m_relays;
    for (const auto &p: pvalues) {
        cout << p.second.first << " x " << p.second.second <<  " => " 
             << p.first << " Use a relay point at " << std::floor((p.second.first+0.5)*m_square) 
             << " x " << std::floor((p.second.second+0.5)*m_square) << endl;
        if (!--limit) {
            break;
        }
    }
}

void hd::Grid::setHeight(int height)
{
    m_height = height;
}

void hd::Grid::setWidth(int width) 
{
    m_width = width;
}

void hd::Grid::initializePartitions()
{
    int qtdWidth = static_cast<int>(std::ceil(m_width / m_square));
    int qtdHeight = static_cast<int>(std::ceil(m_height / m_square));
    for (int i = 0; i<qtdWidth; ++i) {
        for (int j = 0; j<qtdHeight; ++j) {
            m_partitions[std::make_pair(i, j)] = 0;
        }
    }
}