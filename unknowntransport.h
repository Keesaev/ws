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

    virtual void deserializeHeader(const u_char *bytes, int offset);
    virtual bool isHeaderEmpty();
    virtual vector<string> getHeaderData();
};

#endif // EMPTYTRANSPORT_H
