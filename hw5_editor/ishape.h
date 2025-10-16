#pragma once
#include <string>
#include <memory>   
namespace myeditor{
    // Абстрактный интерфейс для всех фигур
    class IShapeBase {
    public:
        virtual ~IShapeBase() = default;
        virtual void draw() const = 0;
        virtual std::string name() const = 0;
    };
    using IShapeBasePtr = std::shared_ptr<IShapeBase>;
}//namespace myeditor