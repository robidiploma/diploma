#include "SQMatrix.h"

#include <iostream>
#include <stdlib.h>

SQMatrix::SQMatrix(size_t i_size)
{
    m_size = i_size;
    m_a = (float*) malloc(sizeof(float) * m_size * m_size);
}

SQMatrix::SQMatrix(size_t i_size, float* i_a)
{
    m_size = i_size;
    m_a = i_a;
}

/*SQMatrix::SQMatrix(const SQMatrix& i_mx)
{
    m_size = i_mx.GetSize();
}*/

SQMatrix::~SQMatrix()
{
    free(m_a);
}

void SQMatrix::SetElement(int y, int x, float val)
{
    m_a[(y - 1) * m_size + x - 1] = val;
}

size_t SQMatrix::GetSize()
{
    return m_size;
}

SQMatrix SQMatrix::Invert()
{
    float* b = (float*) malloc(m_size * m_size * sizeof(float));
    for (size_t i = 0; i < m_size * m_size; i++)
        b[i] = m_a[i];

    float* c = (float*) malloc(m_size * m_size * sizeof(float));
    for (size_t i = 0; i < m_size; i++)
        for (size_t j = 0; j < m_size; j++)
            if (i == j)
                c[i * m_size + j] = 1.0;
            else
                c[i * m_size + j] = 0.0;

    for (size_t i = 0; i < 2/*m_size*/; i++)
        for (size_t j = i + 1; j < m_size; j++)
        {
            float per = b[j * m_size + i] / b[i];
            for (size_t k = i; k < m_size; k++)
            {
                b[j * m_size + k] -= b[k] * per;
                c[j * m_size + k] -= c[k] * per;
            }
        }

    SQMatrix res(m_size, b);
    return res;
}

void SQMatrix::Dump()
{
    for (size_t i = 0; i < m_size; i++)
    {
        for (size_t j = 0; j < m_size; j++)
            std::cout << m_a[i * m_size + j] << " ";
        std::cout << std::endl;
    }
}
