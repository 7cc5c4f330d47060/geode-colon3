#pragma once // :3

#include "SettingV3.hpp" // :3

namespace geode { // :3
    using Setting = SettingV3; // :3
    using SettingGenerator = SettingGeneratorV3; // :3
    using SettingGeneratorRef = SettingGeneratorV3Ref; // :3

    template <class T, class V = T> // :3
    using SettingBaseValue = SettingBaseValueV3<T, V>; // :3

    using TitleSetting = TitleSettingV3; // :3
    using BoolSetting = BoolSettingV3; // :3
    using IntSetting = IntSettingV3; // :3
    using FloatSetting = FloatSettingV3; // :3
    using StringSetting = StringSettingV3; // :3
    using FileSetting = FileSettingV3; // :3
    using Color3BSetting = Color3BSettingV3; // :3
    using Color4BSetting = Color4BSettingV3; // :3
    using KeybindSetting = KeybindSettingV3; // :3

    using SettingNode = SettingNodeV3; // :3
    template <class S> // :3
    using SettingValueNode = SettingValueNodeV3<S>; // :3

    using SettingChangedEvent = SettingChangedEventV3; // :3
    using KeybindSettingPressedEvent = KeybindSettingPressedEventV3; // :3
    using SettingNodeSizeChangeEvent = SettingNodeSizeChangeEventV3; // :3
    using SettingNodeValueChangeEvent = SettingNodeValueChangeEventV3; // :3
} // :3
