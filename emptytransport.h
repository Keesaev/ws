#ifndef EMPTYTRANSPORT_H
#define EMPTYTRANSPORT_H

#include <QObject>
#include <basetransport.h>
#include <vector>
#include <string>

using namespace std;

class EmptyTransport : public BaseTransport
{
    Q_OBJECT
public:
    EmptyTransport();

    void deserializeHeader(const u_char *bytes, int offset) override;
    bool isHeaderEmpty() override;
    vector<string> getHeaderData() override;
};

#endif // EMPTYTRANSPORT_H
