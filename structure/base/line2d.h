#pragma once

#include <type_traits>
#include <stdexcept>

namespace structure
{

template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
class Line2D
{
public:
    using value_type = T;
private:
    Point2D<T> start_{static_cast<T>(0.0), static_cast<T>(0.0)};
    Point2D<T> end_{static_cast<T>(0.0), static_cast<T>(0.0)};
public:
    Line2D(T start_x = static_cast<T>(0.0), T start_y = static_cast<T>(0.0), T end_x = static_cast<T>(0.0), T end_y = static_cast<T>(0.0)) : start_(start_x, start_y), end_(end_x, end_y)
    {

    }
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Point2D<T>, std::decay_t<U>>>>
    Line2D(U && start, U && end) : start_(std::forward<U>(start)), end_(std::forward<U>(end))
    {

    }
    
    Line2D(const Line2D&) = default;
    Line2D& operator=(const Line2D&) = default;
    Line2D(Line2D&&) = default;
    Line2D& operator=(Line2D&&) = default;
public:
    const Point2D<T>& GetStart() const noexcept
    {
        return start_;
    }
    const Point2D<T>& GetEnd() const noexcept
    {
        return end_;
    }
    Point2D<T>& GetStart() noexcept
    {
        return start_;
    }
    Point2D<T>& GetEnd() noexcept
    {
        return end_;
    }  
    Point2D<T>& operator[](std::size_t index) noexcept
    {
        if (index == 0)
        {
            return start_;
        }
        else if (index == 1)
        {
            return end_;
        }
        else
        {
            throw std::out_of_range("Index out of range. Valid indices are 0 and 1.");
        }
    } 
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Point2D<T>, std::decay_t<U>>>>
    void SetStart(U && point) noexcept
    {
        start_ = std::forward<U>(point);
    }
    void SetStart(T start_x = static_cast<T>(0.0), T start_y = static_cast<T>(0.0)) noexcept
    {
        start_.SetStart(Point2D<T>(start_x, start_y));
    }
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Point2D<T>, std::decay_t<U>>>>
    void SetEnd(U && point) noexcept
    {
        end_ = std::forward<U>(point);
    }
    void SetEnd(T end_x = static_cast<T>(0.0), T end_y = static_cast<T>(0.0)) noexcept
    {
        end_.SetEnd(Point2D<T>(end_x, end_y));
    }
    T GetLength() const noexcept
    {
        return start_.DistOfEuclidean(end_);
    }
    T GetSlope() const
    {
        T deltaX = end_.GetX() - start_.GetX();
        if (std::abs(deltaX) < std::numeric_limits<T>::epsilon() * std::max(std::abs(start_.GetX()), std::abs(end_.GetX())))
        {
            throw std::runtime_error("Slope is undefined for vertical lines.");
        }
        return (end_.GetY() - start_.GetY()) / deltaX;
    }
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<std::decay_t<U>, Point2D<typename std::decay_t<U>::value_type>>>>
    decltype(auto) DistFromPoint(U && point) const noexcept
    {
        T A = end_.GetY() - start_.GetY();
        T B = start_.GetX() - end_.GetX();
        T C = end_.GetX() * start_.GetY() - start_.GetX() * end_.GetY();
        return std::abs(A * std::forward<U>(point).GetX() + B * std::forward<U>(point).GetY() + C) / std::sqrt(A * A + B * B);
    }
    bool IsPointOnLine(const Point2D<T>& point, T tolerance = std::numeric_limits<T>::epsilon()) const noexcept
    {
        T dist = DistFromPoint(point);
        return dist <= tolerance;
    }
    template <typename U, typename = typename std::enable_if_t<std::is_same_v<Line2D<T>, std::decay_t<U>>>>
    bool IsCross(U && other) const noexcept
    {
        T d1 = (end_.GetX() - start_.GetX()) * (std::forward<U>(other).start_.GetY() - start_.GetY()) - (end_.GetY() - start_.GetY()) * (std::forward<U>(other).start_.GetX() - start_.GetX());
        T d2 = (end_.GetX() - start_.GetX()) * (std::forward<U>(other).end_.GetY() - start_.GetY()) - (end_.GetY() - start_.GetY()) * (std::forward<U>(other).end_.GetX() - start_.GetX());
        T d3 = (std::forward<U>(other).end_.GetX() - std::forward<U>(other).start_.GetX()) * (start_.GetY() - std::forward<U>(other).start_.GetY()) - (std::forward<U>(other).end_.GetY() - std::forward<U>(other).start_.GetY()) * (start_.GetX() - std::forward<U>(other).start_.GetX());
        T d4 = (std::forward<U>(other).end_.GetX() - std::forward<U>(other).start_.GetX()) * (end_.GetY() - std::forward<U>(other).start_.GetY()) - (std::forward<U>(other).end_.GetY() - std::forward<U>(other).start_.GetY()) * (end_.GetX() - std::forward<U>(other).start_.GetX());
        return ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0));
    }
    void Translate(T dx = static_cast<T>(0.0), T dy = static_cast<T>(0.0)) noexcept
    {
        start_.Translate(dx, dy);
        end_.Translate(dx, dy);
    }
};

};