// figures: class adapter pattern
// Seth Winkhart
// 4/8/2022

#include <iostream>

using std::cout;
using std::endl;

// base interface
class Figure
{
public:
    virtual void draw() = 0;
    virtual ~Figure() {}
};

// adaptee/implementer
class LegacyRectangle
{
public:
    LegacyRectangle(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) : topLeftX_(topLeftX), topLeftY_(topLeftY), bottomRightX_(bottomRightX), bottomRightY_(bottomRightY) {}

    void oldDraw()
    {
        for (int i = 0; i < bottomRightY_; ++i)
        {
            for (int j = 0; j < bottomRightX_; ++j)
                if (i >= topLeftY_ && j >= topLeftX_)
                    cout << '*';
                else
                    cout << ' ';
            cout << endl;
        }
    }

    void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY)
    {
        topLeftX_ = topLeftX;
        topLeftY_ = topLeftY;
        bottomRightX_ = bottomRightX;
        bottomRightY_ = bottomRightY;
    }

    int getTopLeftX() const { return topLeftX_; }
    int getTopLeftY() const { return topLeftY_; }
    int getBottomRightX() const { return bottomRightX_; }
    int getBottomRightY() const { return bottomRightY_; }
    // defining top/left and bottom/right coordinates
private:
    int topLeftX_;
    int topLeftY_;
    int bottomRightX_;
    int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter : public Figure,
                      private LegacyRectangle
{
public:
    SquareAdapter(int size) : LegacyRectangle(0, 0, size, size){};
    void draw() override { oldDraw(); }
    void resize(int newSize) { this->move(0, 0, newSize, newSize); }
    int size() const { return getBottomRightX(); }
};

int main()
{
    int size;
    cout << "Enter the size of the square: ";
    std::cin >> size;
    cout << endl;

    SquareAdapter *square = new SquareAdapter(size);
    cout << "The size of the square is: " << square->size() << endl;
    square->draw();

    int newSize;
    cout << "Enter the new size of the square: ";
    std::cin >> newSize;
    cout << endl;

    square->resize(newSize);
    cout << "The new size is: " << square->size() << endl;
    square->draw();
}
