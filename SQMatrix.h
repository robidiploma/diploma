#ifndef SQMatrix_H
#define SQMatrix_H

#include <memory>

class SQMatrix
{
public:
    SQMatrix(size_t);
    SQMatrix(size_t, float*);
    //SQMatrix(const SQMatrix&); // TODO
    ~SQMatrix();

    void SetElement(int, int, float);

    size_t GetSize();

    SQMatrix Invert();

    void Dump();

private:
    float* m_a;
    size_t m_size;
};

#endif // SQMatrix_H
