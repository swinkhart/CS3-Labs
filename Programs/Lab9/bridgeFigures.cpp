// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Seth Winkhart
// 3/28/2022

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// abstract body
class Fill
{
public:
    Fill(char fillChar, char borderChar) : fillChar_(fillChar), borderChar_(borderChar) {}
    virtual char getBorder() { return borderChar_; }
    virtual char getInternal() { return fillChar_; }
    virtual ~Fill() {}

protected:
    char fillChar_;
    char borderChar_;
};

// concrete body
class Hollow : public Fill
{
public:
    Hollow(char borderChar) : Fill(' ', borderChar) {}
    char getBorder() override { return borderChar_; }
    char getInternal() override { return ' '; }
    ~Hollow() override {}
};

// another concrete body
class Filled : public Fill
{
public:
    Filled(char fillChar) : Fill(fillChar, fillChar) {}
    char getBorder() override { return fillChar_; }
    char getInternal() override { return fillChar_; }
    ~Filled() override {}
};

// added function
class FullyFilled : public Fill
{
public:
    FullyFilled(char fillChar, char borderChar) : Fill(fillChar, borderChar) {}
    char getBorder() override { return borderChar_; }
    char getInternal() override { return fillChar_; }
    ~FullyFilled() override {}
};

// abstract handle
class Figure
{
public:
    Figure(int size, Fill *fill) : size_(size), fill_(fill) {}
    virtual void draw() = 0;
    virtual ~Figure() {}

protected:
    int size_;
    Fill *fill_;
};

// concrete handle
class Square : public Figure
{
public:
    Square(int size, Fill *fill) : Figure(size, fill) {}
    void draw() override;
};

void Square::draw()
{
    for (int i = 0; i < size_; ++i)
    {
        for (int j = 0; j < size_; ++j)
            if (i == 0 || j == 0 || i == size_ - 1 || j == size_ - 1)
                cout << fill_->getBorder();
            else
                cout << fill_->getInternal();
        cout << endl;
    }
}

int main()
{
    // demonstrating all 3 fills
    Fill *hollowPaint = new Hollow('X');
    Fill *filledPaint = new Filled('*');
    Fill *fullyFilledPaint = new FullyFilled('0', 'X');

    Figure *firstBox = new Square(5, hollowPaint);
    Figure *secondBox = new Square(7, filledPaint);
    Figure *thirdBox = new Square(10, fullyFilledPaint);

    firstBox->draw();
    cout << endl;
    secondBox->draw();
    cout << endl;
    thirdBox->draw();
}