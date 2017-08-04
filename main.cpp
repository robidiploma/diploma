#include "GeomData.h"

int main()
{
    GeomData gd = GeomData::GetInstance();
    gd.LoadData("C:\\Users\\laptop\\Desktop\\GeomData.txt");

    gd.Dump();

    SQMatrix mx(3);
    mx.SetElement(1, 1, 3);
    mx.SetElement(1, 2, 2);
    mx.SetElement(1, 3, 2);
    mx.SetElement(2, 1, 1);
    mx.SetElement(2, 2, 4);
    mx.SetElement(2, 3, 2);
    mx.SetElement(3, 1, 4);
    mx.SetElement(3, 2, 4);
    mx.SetElement(3, 3, 5);
    mx.Dump();
    mx.Invert().Dump();

    return 1;
}
