#pragma once
#include <memory>
#include <string>
#include <iostream>
namespace myeditor{
    // Абстрактный интерфейс для всех фигур
    class IShapeBase {
    public:
        virtual ~IShapeBase() = default;
        virtual void draw() const = 0;
        virtual std::string name() const = 0;
    };

    // Шаблонный класс примитива
    template<typename T>
    class Shape : public IShapeBase {
    protected:
        T x_, y_;
    public:
        Shape(T x, T y) : x_(x), y_(y) {}
        virtual ~Shape() = default;
    };

    // Конкретные фигуры
    template<typename T>
    class Circle : public Shape<T> {
        T radius_;
    public:
        Circle(T x, T y, T r) : Shape<T>(x, y), radius_(r) {}
        void draw() const override {
            std::cout << "Draw Circle at (" << this->x_ << "," << this->y_ << "), r=" << radius_ << std::endl;
        }
        std::string name() const override { return "Circle"; }
    };

    template<typename T>
    class Rectangle : public Shape<T> {
        T w_, h_;
    public:
        Rectangle(T x, T y, T w, T h) : Shape<T>(x, y), w_(w), h_(h) {}
        void draw() const override {
            std::cout << "Draw Rectangle at (" << this->x_ << "," << this->y_
                    << "), size=(" << w_ << "," << h_ << ")"<<std::endl;
        }
        std::string name() const override { return "Rectangle"; }
    };
}//namespace myeditor