#include "emptytransport.h"

EmptyTransport::EmptyTransport()
{

}

void EmptyTransport::deserializeHeader(const u_char *bytes, int offset) {
    // Ничего
}

bool EmptyTransport::isHeaderEmpty() {
    // Ничего
    return false;
}

vector<string> EmptyTransport::getHeaderData() {
    //
    vector<string> v;
    v.push_back("No data available");
    return v;
}
