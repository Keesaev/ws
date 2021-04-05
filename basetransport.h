#ifndef BASETRANSPORT_H
#define BASETRANSPORT_H

#include <QObject>
#include <vector>

class BaseTransport : public QObject
{
    Q_OBJECT
public:
    explicit BaseTransport(QObject *parent = nullptr);

    virtual void deserializeHeader(const u_char *bytes, int offset) = 0;

    virtual bool isHeaderEmpty() = 0;

    virtual std::vector<std::string> getHeaderData() = 0;

signals:

};

#endif // BASETRANSPORT_H
