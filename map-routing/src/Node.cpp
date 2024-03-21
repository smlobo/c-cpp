//
// Created by Sheldon Lobo on 3/17/24.
//

#include <ostream>
#include <iomanip>
#include "Node.h"

std::ostream& operator<<(std::ostream& strm, const Node& n) {
    strm << n.id << " <" << std::setw(4) << n.x << "," << n.y << ">";
    return strm;
}
