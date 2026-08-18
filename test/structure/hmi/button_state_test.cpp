#include <cassert>
#include <cstdint>

#include "../../../structure/hmi/button_state.h"

namespace
{
using structure::hmi::ButtonState;
using structure::hmi::ButtonStateType;

void TestEnumValues()
{
    assert(static_cast<std::uint8_t>(ButtonStateType::NOT_DISPLAY_0) == 0);
    assert(static_cast<std::uint8_t>(ButtonStateType::DISPLAY_DISABLED_1) == 1);
    assert(static_cast<std::uint8_t>(ButtonStateType::DISPLAY_ENABLED_2) == 2);
    assert(static_cast<std::uint8_t>(ButtonStateType::DISPLAY_SELECTED_3) == 3);
}

void TestDefaultState()
{
    const ButtonState state;

    assert(state.Get() == ButtonStateType::NOT_DISPLAY_0);
    assert(!state.IsEnabled());
    assert(!state.IsDisplayedButDisabled());
    assert(!state.IsSelected());
}

void TestParameterizedConstruction()
{
    const ButtonState state(ButtonStateType::DISPLAY_SELECTED_3);

    assert(state.Get() == ButtonStateType::DISPLAY_SELECTED_3);
    assert(!state.IsEnabled());
    assert(!state.IsDisplayedButDisabled());
    assert(state.IsSelected());
}

void TestSetAndStateQueries()
{
    ButtonState state;

    state.Set(ButtonStateType::DISPLAY_DISABLED_1);
    assert(state.Get() == ButtonStateType::DISPLAY_DISABLED_1);
    assert(!state.IsEnabled());
    assert(state.IsDisplayedButDisabled());
    assert(!state.IsSelected());

    state.Set(ButtonStateType::DISPLAY_ENABLED_2);
    assert(state.Get() == ButtonStateType::DISPLAY_ENABLED_2);
    assert(state.IsEnabled());
    assert(!state.IsDisplayedButDisabled());
    assert(!state.IsSelected());

    state.Set(ButtonStateType::DISPLAY_SELECTED_3);
    assert(state.Get() == ButtonStateType::DISPLAY_SELECTED_3);
    assert(!state.IsEnabled());
    assert(!state.IsDisplayedButDisabled());
    assert(state.IsSelected());

    state.Set(ButtonStateType::NOT_DISPLAY_0);
    assert(state.Get() == ButtonStateType::NOT_DISPLAY_0);
    assert(!state.IsEnabled());
    assert(!state.IsDisplayedButDisabled());
    assert(!state.IsSelected());
}
} // namespace

int main()
{
    TestEnumValues();
    TestDefaultState();
    TestParameterizedConstruction();
    TestSetAndStateQueries();
    return 0;
}
