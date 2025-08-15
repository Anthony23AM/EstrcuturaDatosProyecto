#ifndef IMOSTRABLE_H
#define IMOSTRABLE_H

class IMostrable {
public:
    virtual void mostrar() const = 0;
    virtual ~IMostrable() {}
};

#endif