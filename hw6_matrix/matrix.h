#pragma once
#include <vector>
#include <map>
#include <utility>
#include <cstddef>
#include <stdexcept>

namespace MyMatrix{
template <typename T, T DefaultValue>
class Matrix {
public:
    Matrix() = default;

    // Прокси-доступ к строке
    class RowProxy {
    public:
        RowProxy(Matrix& parent, size_t row) : parent_(parent), row_(row) {}

        // Чтение и запись ячейки
        class CellProxy {
        public:
            CellProxy(Matrix& parent, size_t row, size_t col)
                : parent_(parent), row_(row), col_(col) {}

            // Присваивание и удаление
            CellProxy& operator=(const T& value) {
                if (value != DefaultValue)
                    parent_.data_[{row_, col_}] = value;
                else
                    parent_.data_.erase({row_, col_});
                return *this;
            }

            // Приведение к T (чтение), если отсутствует ключ вывод значения по умолчанию
            operator T() const {
                auto it = parent_.data_.find({row_, col_});
                return (it != parent_.data_.end()) ? it->second : DefaultValue;
            }

        private:
            Matrix& parent_;
            size_t row_, col_;
        };

        CellProxy operator[](size_t col) {
            return CellProxy(parent_, row_, col);
        }

    private:
        Matrix& parent_;
        size_t row_;
    };

    RowProxy operator[](size_t row) {
        return RowProxy(*this, row);
    }

    // Количество занятых ячеек
    size_t size() const {
        return data_.size();
    }

    // Итератор по занятым ячейкам
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

private:
    std::map<std::pair<size_t, size_t>, T> data_;
};

template <typename T, T DefaultValue, size_t N>
class MatrixN {
public:
    MatrixN() = default;

    // Прокси-доступ к координате
    class Proxy {
    public:
        Proxy(MatrixN& parent, std::vector<size_t> indices)
            : parent_(parent), indices_(std::move(indices)) {}

        Proxy operator[](size_t index) {
            auto next = indices_;
            next.push_back(index);
            return Proxy(parent_,std::move(next));
        }

        // Присваивание
        Proxy& operator=(const T& value) {
            if (indices_.size() != N)
                throw std::out_of_range("Incomplete index");
            if (value != DefaultValue)
                parent_.data_[indices_] = value;
            else
                parent_.data_.erase(indices_);
            return *this;
        }
        // Чтение
        operator T() const {
            if (indices_.size() != N)
                throw std::out_of_range("Incomplete index");
            auto it = parent_.data_.find(indices_);
            return (it != parent_.data_.end()) ? it->second : DefaultValue;
        }

    private:
        MatrixN& parent_;
        std::vector<size_t> indices_;
    };

    Proxy operator[](size_t index) {
        return Proxy(*this, {index});
    }

    size_t size() const { return data_.size(); }

    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }

private:
    std::map<std::vector<size_t>, T> data_;
};

}//Matrix