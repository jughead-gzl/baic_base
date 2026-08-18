#pragma once

#include <cstdint>

namespace structure
{
namespace hmi
{

/**
 * @brief 按钮的交互状态。
 *
 * 枚举值后缀中的数字表示状态的固定序号，便于与外部协议或配置中的
 * 数值进行对应。状态本身不表示按钮的视觉样式，界面层可以根据这些
 * 状态决定按钮的颜色、图标和交互反馈。
 */
enum class ButtonStateType : std::uint8_t
{
    /** @brief 不显示：按钮不应显示在界面上，也不应响应用户操作。 */
    NOT_DISPLAY_0,

    /** @brief 显示但不可用：按钮显示在界面上，但不允许用户操作。 */
    DISPLAY_DISABLED_1,

    /** @brief 显示并且可用：按钮显示在界面上，并允许用户操作。 */
    DISPLAY_ENABLED_2,

    /** @brief 显示并且选中：按钮显示在界面上，并处于选中状态。 */
    DISPLAY_SELECTED_3,
};

/**
 * @brief 保存并查询按钮当前状态。
 *
 * 该类只负责保存按钮状态和提供状态判断，不负责绘制按钮，也不负责
 * 分发点击事件。状态的修改由调用方通过 Set() 完成，界面或业务代码
    * 可以使用 IsEnabled()、IsDisplayedButDisabled() 和 IsSelected() 查询常用状态。
 */
class ButtonState
{
private:
    /** @brief 当前按钮状态，默认创建为可交互的普通状态。 */
    ButtonStateType state_{ButtonStateType::NOT_DISPLAY_0};

public:
    /**
     * @brief 使用默认状态构造按钮状态对象。
     *
    * 构造后按钮状态为 NOT_DISPLAY_0。
     */
    ButtonState() noexcept = default;

    /**
     * @brief 使用指定状态构造按钮状态对象。
     * @param state 初始按钮状态。
     */
    explicit ButtonState(ButtonStateType state) noexcept : state_(state)
    {
    }

    /**
     * @brief 获取当前按钮状态。
     * @return 当前保存的按钮状态。
     */
    ButtonStateType Get() const noexcept
    {
        return state_;
    }

    /**
     * @brief 设置当前按钮状态。
     * @param state 要保存的新状态。
     *
     * 该函数只更新状态，不会自动触发界面刷新、点击事件或其他副作用。
     */
    void Set(ButtonStateType state) noexcept
    {
        state_ = state;
    }

    /**
     * @brief 判断按钮是否可交互。
    * @return 当状态为 DISPLAY_ENABLED_2 或 DISPLAY_SELECTED_3 时返回 true，
     *         否则返回 false。
     *
     * NOT_DISPLAY_0 表示按钮不存在于界面中，DISPLAY_DISABLED_1 表示按钮
     * 虽然显示但不可操作；两者都不属于可交互状态。
     */
    bool IsEnabled() const noexcept
    {
         return state_ == ButtonStateType::DISPLAY_ENABLED_2;
    }

    /**
     * @brief 判断按钮是否处于显示但不可用状态。
     * @return 仅当当前状态为 DISPLAY_DISABLED_1 时返回 true。
     */
    bool IsDisplayedButDisabled() const noexcept
    {
        return state_ == ButtonStateType::DISPLAY_DISABLED_1;
    }

    /**
     * @brief 判断按钮是否处于显示并选中状态。
     * @return 仅当当前状态为 DISPLAY_SELECTED_3 时返回 true。
     */
    bool IsSelected() const noexcept
    {
        return state_ == ButtonStateType::DISPLAY_SELECTED_3;
    }
};

}  // namespace hmi
}  // namespace structure