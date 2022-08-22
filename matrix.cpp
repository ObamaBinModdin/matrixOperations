#include "matrix.h"
#include <iostream>

Matrix::Matrix()
{
    this->values = new long* [0];

    this->width = 0;
    this->height = 0;
}

Matrix::Matrix(int size)
{
    this->values = new long* [size];

    for (int x = 0; x < size; x++)
    {
        this->values[x] = new long[size];
    }

    for (int r = 0; r < size; r++)
    {
        for (int c = 0; c < size; c++)
        {
            if (r == c)
            {
                this->values[r][c] = 1;
            }
            else
            {
                this->values[r][c] = 0;
            }
        }
    }

    this->width = size;
    this->height = size;
}

Matrix::Matrix(int height, int width)
{
    this->values = new long* [height];

    for (int x = 0; x < height; x++)
    {
        this->values[x] = new long[width];
    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            this->values[r][c] = 0;
        }
    }

    this->width = width;
    this->height = height;
}

Matrix::~Matrix()
{
    for (int x = 0; x < getHeight(); x++)
    {
        delete[] this->values[x];
    }
    delete[] this->values;

    this->values = nullptr;
}

int Matrix::getWidth() const
{
    return this->width;
}

int Matrix::getHeight() const
{
    return this->height;
}

void Matrix::resize(int height, int width)
{
    long** newValues = new long* [height];

    for (int x = 0; x < height; x++)
    {
        newValues[x] = new long[width];
    }

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            if (r < getHeight() && c < getWidth())
            {
                newValues[r][c] = this->values[r][c];
            }
            else
            {
                newValues[r][c] = 0;
            }
        }
    }

    for (int x = 0; x < getHeight(); x++)
    {
        delete[] this->values[x];
    }
    delete[] this->values;

    this->values = newValues;

    newValues = nullptr;

    this->width = width;
    this->height = height;
}

long& Matrix::operator()(const int row, const int column)
{
    return this->values[row][column];
}

long Matrix::operator()(const int row, const int column) const
{
    return this->values[row][column];
}

// Lab 4

Matrix::Matrix(const Matrix& other) : Matrix(other.height, other.width)
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            values[h][w] = other(h, w);
        }
    }
}

Matrix::Matrix(Matrix&& rhs)
{  
    width = rhs.width;
    height = rhs.height;
    values = rhs.values;
    rhs.width = 0;
    rhs.height = 0;
    rhs.values = nullptr;
}

void Matrix::transpose()
{
    long** newValues = new long* [this->width];

    for (int r = 0; r < this->getWidth(); r++)
    {
        newValues[r] = new long[this->getHeight()];
        for (int c = 0; c < this->getHeight(); c++)
        {
            newValues[r][c] = this->values[c][r];
        }
    }

    for (int x = 0; x < getHeight(); x++)
    {
        delete[] this->values[x];
    }
    delete[] this->values;

    this->values = newValues;

    newValues = nullptr;

    int holder = this->width;
    this->width = this->height;
    this->height = holder;
    holder = 0;
}

Matrix& Matrix::operator=(const Matrix& toCopy)
{
    Matrix returnMatrix = Matrix(toCopy);
    return returnMatrix;
}

Matrix& Matrix::operator=(Matrix&& toCopy)
{
    Matrix returnMatrix = Matrix(toCopy);
    return returnMatrix;
}

bool Matrix::operator==(const Matrix& compared) const
{
    if (this->getHeight() != compared.getHeight() || this->getWidth() != compared.getWidth())
    {
        return false;
    }

    for (int r = 0; r < this->getHeight(); r++)
    {
        for (int c = 0; c < this->getWidth(); c++)
        {
            if (this->values[r][c] != compared.values[r][c])
            {
                return false;
            }
        }
    }

    return true;
}

// Lab 5

Matrix Matrix::operator+(const Matrix& rightMatrix) const
{
    int newHeight;
    int newWidth;

    if (this->getHeight() < rightMatrix.getHeight())
    {
        newHeight = this->getHeight();
    }
    else
    {
        newHeight = rightMatrix.getHeight();
    }

    if (this->getWidth() < rightMatrix.getWidth())
    {
        newWidth = this->getWidth();
    }
    else
    {
        newWidth = rightMatrix.getWidth();
    }

    Matrix newMatrix = Matrix(newHeight, newWidth);

    for (int r = 0; r < newHeight; r++)
    {
        for (int c = 0; c < newWidth; c++)
        {
            newMatrix.values[r][c] = this->values[r][c] + rightMatrix.values[r][c];
        }
    }

    return newMatrix;
}

Matrix& Matrix::operator+=(const Matrix& rightMatrix)
{
    Matrix original = Matrix(*this);

    if (this->getHeight() > rightMatrix.getHeight())
    {
        this->height = rightMatrix.getHeight();
    }

    if (this->getWidth() > rightMatrix.getWidth())
    {
        this->width = rightMatrix.getWidth();
    }

    this->values = new long* [this->getHeight()];
    
    for (int r = 0; r < this->getHeight(); r++)
    {
        this->values[r] = new long[this->getWidth()];
        for (int c = 0; c < this->getWidth(); c++)
        {
            this->values[r][c] = original.values[r][c] + rightMatrix.values[r][c];
        }
    }

    return *this;
}

Matrix Matrix::operator*(const Matrix& rightMatrix) const
{
    if (this->getHeight() != rightMatrix.getWidth())
    {
        Matrix invMatrix = Matrix(*this);
        return invMatrix;
    }// Return left Matrix if multiplication is invalid.

    int newHeight;
    int newWidth;

    if (this->getHeight() > rightMatrix.getHeight())
    {
        newHeight = this->getHeight();
    }
    else
    {
        newHeight = rightMatrix.getHeight();
    }

    if (this->getWidth() > rightMatrix.getWidth())
    {
        newWidth = this->getWidth();
    }
    else
    {
        newWidth = rightMatrix.getWidth();
    }

    Matrix newMatrix = Matrix(newHeight, newWidth);

    for (int r = 0; r < this->getHeight(); r++)
    {
        for (int c = 0; c < rightMatrix.getWidth(); c++)
        {
            newMatrix.values[r][c] = 0;
            for (int k = 0; k < rightMatrix.getHeight(); k++)
            {
                newMatrix.values[r][c] += this->values[r][k] * rightMatrix.values[k][c];
            }
        }
    }

    return newMatrix;
}

Matrix& Matrix::operator*=(const Matrix& rightMatrix)
{
    if (this->getHeight() != rightMatrix.getWidth())
    {
        return *this;
    }// Return left Matrix if multiplication is invalid.

    Matrix copyMatrix = Matrix(*this);

    if (this->getHeight() < rightMatrix.getHeight())
    {
        this->height = rightMatrix.getHeight();
    }

    if (this->getWidth() < rightMatrix.getWidth())
    {
        this->width = rightMatrix.getWidth();
    }

    this->values = new long* [this->getHeight()];
    for (int x = 0; x < this->getHeight(); x++)
    {
        this->values[x] = new long[this->width];
    }

    for (int r = 0; r < this->getHeight(); r++)
    {
        for (int c = 0; c < this->getWidth(); c++)
        {
            this->values[r][c] = 0;
        }

    }


    for (int r = 0; r < copyMatrix.getHeight(); r++)
    {
        for (int c = 0; c < rightMatrix.getWidth(); c++)
        {
            for (int k = 0; k < rightMatrix.getHeight(); k++)
            {
                this->values[r][c] += copyMatrix.values[r][k] * rightMatrix.values[k][c];
            }
        }
    }

    return *this;
}

Matrix Matrix::operator*(long multiplier) const
{
    Matrix copyMatrix = Matrix(*this);

    for (int r = 0; r < copyMatrix.getHeight(); r++)
    {
        for (int c = 0; c < copyMatrix.getWidth(); c++)
        {
            copyMatrix.values[r][c] *= multiplier;
        }
    }

    return copyMatrix;
}

Matrix& Matrix::operator*=(long multiplier)
{
    for (int r = 0; r < this->getHeight(); r++)
    {
        for (int c = 0; c < this->getWidth(); c++)
        {
            this->values[r][c] *= multiplier;
        }
    }

    return *this;
}
