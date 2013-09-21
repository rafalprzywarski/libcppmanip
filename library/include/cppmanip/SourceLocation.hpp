#ifndef SOURCELOCATION_HPP
#define SOURCELOCATION_HPP

struct SourceLocation
{
    unsigned row, col;
    SourceLocation() : row(), col() { }
};

#endif // SOURCELOCATION_HPP
