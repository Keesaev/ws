#include "unknowntransport.h"

UnknownTransport::UnknownTransport()
{

}

void UnknownTransport::deserializeHeader(const u_char *bytes, int offset) {
    //
}

bool UnknownTransport::isHeaderInvalid() {
    //
    return false;
}

vector<pair<string, string>> UnknownTransport::getHeaderData() {

    vector<pair<string, string>> v;
    v.push_back(make_pair("Protocol unkown", "No data available"));
    return v;
}
