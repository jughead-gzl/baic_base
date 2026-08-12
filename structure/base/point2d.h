#pragma

#include <type_traits>
#include <cmath>

namespace structure
{

template <typename T, typename = typename std::enable_if_t<std::is_floating_point_v<T>>>
class Point2D
{
public:
    using value_type = T;
private:
    T x_{static_cast<T>(0)};
    T y_{static_cast<T>(0)};
public:
    Point2D(T x = static_cast<T>(0), T y = static_cast<T>(0)) : x_(x), y_(y)
    {

    }
    Point2D(const Point2D&) = default;
    Point2D& operator=(const Point2D&) = default;
    Point2D(Point2D&&) = default;
    Point2D& operator=(Point2D&&) = default;
public:
    const T& GetX() const noexcept
    {
        return x_;
    }
    const T& GetY() const noexcept
    {
        return y_;
    }
    T& GetX() noexcept
    {
        return x_;
    }
    T& GetY() noexcept
    {
        return y_;
    }
    void SetX(T x = static_cast<T>(0)) noexcept
    {
        x_ = x;
    }
    void SetY(T y = static_cast<T>(0)) noexcept
    {
        y_ = y;
    }
public:
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfChebyshev(U && point) const noexcept
    {
        return std::max(std::abs(x_ - std::forward<U>(point).GetX()), std::abs(y_ - std::forward<U>(point).GetY()));
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfManhattan(U && point) const noexcept
    {
        return std::abs(x_ - std::forward<U>(point).GetX()) + std::abs(y_ - std::forward<U>(point).GetY());
    }
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    T DistOfEuclidean(U && point) const noexcept
    {
        return std::sqrt(std::pow((x_ - std::forward<U>(point).GetX()), 2) + std::pow(y_ - std::forward<U>(point).GetY(), 2));
    }

    /**
     * @brief Translate the point by the given offsets in x and y directions.
     * @param dx Offset in the x direction (default is 0).
     * @param dy Offset in the y direction (default is 0).
     */
    void Translate(T dx = static_cast<T>(0), T dy = static_cast<T>(0)) noexcept
    {
        x_ += dx;
        y_ += dy;
    }

    /**
     * @brief Rotate the point around the origin by the given angle in degrees.
     * @param angle Angle in degrees (default is 0).
     */
    void Rotate(T angle = static_cast<T>(0)) noexcept
    {
        T radian = angle * static_cast<T>(M_PI) / static_cast<T>(180);
        T new_x = x_ * std::cos(radian) - y_ * std::sin(radian);
        T new_y = x_ * std::sin(radian) + y_ * std::cos(radian);
        x_ = new_x;
        y_ = new_y;
    }

    /**
     * @brief Scale the point by the given factors in x and y directions.
     * @param sx Scaling factor in the x direction (default is 1).
     * @param sy Scaling factor in the y direction (default is 1).
     */
    void Scale(T sx = static_cast<T>(1), T sy = static_cast<T>(1)) noexcept
    {
        x_ *= sx;
        y_ *= sy;
    }

    /**
     * @brief Three-way comparison operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return std::strong_ordering indicating the comparison result
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) operator<=>(U && other) const noexcept
    {
        if (x_ < std::forward<U>(other).GetX()) return std::strong_ordering::less;
        if (x_ > std::forward<U>(other).GetX()) return std::strong_ordering::greater;
        if (y_ < std::forward<U>(other).GetY()) return std::strong_ordering::less;
        if (y_ > std::forward<U>(other).GetY()) return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    /**
     * @brief Equality operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return true if the points are equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) operator==(U && other) const noexcept
    {
        return x_ == std::forward<U>(other).GetX() && y_ == std::forward<U>(other).GetY();
    }

    /**
     * @brief Inequality operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return true if the points are not equal, false otherwise
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) operator!=(U && other) const noexcept
    {
        return !(*this == std::forward<U>(other));
    }

    /**
     * @brief Addition operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return A new Point2D that is the sum of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) operator+(U && other) const noexcept
    {
        return Point2D(x_ + std::forward<U>(other).GetX(), y_ + std::forward<U>(other).GetY());
    }

    /**
     * @brief Subtraction operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return A new Point2D that is the difference between this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) operator-(U && other) const noexcept
    {
        return Point2D(x_ - std::forward<U>(other).GetX(), y_ - std::forward<U>(other).GetY());
    }

    /**
     * @brief Stream insertion operator for Point2D.
     * @tparam CharT Character type of the output stream
     */
    template <typename CharT, typename = typename std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
    friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, const Point2D& point)
    {
        os << "(" << point.GetX() << ", " << point.GetY() << ")";
        return os;
    }
    /**
     * @brief Stream extraction operator for Point2D.
     * @tparam CharT Character type of the input stream
     */
    template <typename CharT, typename = typename std::enable_if_t<std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>>>
    friend std::basic_istream<CharT>& operator>>(std::basic_istream<CharT>& is, Point2D& point)
    {
        CharT ch1, ch2, ch3;
        T x, y;
        is >> ch1 >> x >> ch2 >> y >> ch3;
        if (ch1 == '(' && ch2 == ',' && ch3 == ')')
        {
            point.SetX(x);
            point.SetY(y);
        }
        else
        {
            is.setstate(std::ios::failbit);
        }
        return is;
    }

    /**
     * @brief Convert the Point2D to a string representation.
     * @return A string in the format "(x, y)"
     */
    std::string ToString() const noexcept
    {
        return std::string("(") + std::to_string(x_) + ", " + std::to_string(y_) + ")";
    }

    /**
     * @brief Dot product operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return The dot product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) Dot(U && other) const noexcept
    {
        return x_ * std::forward<U>(other).GetX() + y_ * std::forward<U>(other).GetY();
    }

    /**
     * @brief Cross product operator for Point2D.
     * @tparam U Type of the other point, must be derived from Point2D
     * @return The cross product of this point and the other point
     */
    template <typename U, typename = typename std::enable_if_t<std::is_base_of_v<Point2D, std::decay_t<U>>>>
    decltype(auto) Cross(U && other) const noexcept
    {
        return x_ * std::forward<U>(other).GetY() - y_ * std::forward<U>(other).GetX();
    }

    /**
     * @brief Normalize the point to have a length of 1, if it is not already at the origin.
     * This modifies the point in place.
     */
    void Normalize() noexcept
    {
        T length = std::sqrt(x_ * x_ + y_ * y_);
        if (length > std::numeric_limits<T>::epsilon())
        {
            x_ /= length;
            y_ /= length;
        }
    }
};
};

