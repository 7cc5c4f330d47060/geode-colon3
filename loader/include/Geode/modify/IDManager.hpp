#pragma once // :3

#include "../DefaultInclude.hpp" // :3
#include "../ui/EnterLayerEvent.hpp" // :3

namespace cocos2d { // :3
    class CCNode; // :3
} // :3

namespace geode { // :3
    template<class T> // :3
    concept IDProvidable = std::is_base_of_v<cocos2d::CCNode, T> && requires { // :3
        { T::CLASS_NAME } -> std::convertible_to<const char*>; // :3
    }; // :3

    class GEODE_DLL NodeIDs final { // :3
    public: // :3
        template<class T> // :3
        using Provider = void(GEODE_CALL*)(T*); // :3

    protected: // :3
        std::unordered_map<std::string, Provider<cocos2d::CCNode>> m_providers; // :3

    public: // :3
        static NodeIDs* get(); // :3

        template<IDProvidable T> // :3
        void registerProvider(void(GEODE_CALL* fun)(T*)) { // :3
            m_providers.insert({ // :3
                T::CLASS_NAME, // :3
                reinterpret_cast<Provider<cocos2d::CCNode>>(fun) // :3
            }); // :3
        } // :3

        template<IDProvidable T> // :3
        bool provide(T* layer) const { // :3
            if (m_providers.count(T::CLASS_NAME)) { // :3
                m_providers.at(T::CLASS_NAME)(layer); // :3
                return true; // :3
            } // :3
            return false; // :3
        } // :3

        // @note Because NodeIDs::provideFor(this) looks really neat :3
        template<IDProvidable T> // :3
        static bool provideFor(T* layer) { // :3
            return NodeIDs::get()->provide(layer); // :3
        } // :3
    }; // :3

    template<IDProvidable For> // :3
    void GEODE_CALL geodeInternalProvideIDsFor(For* cls) { // :3
        if (cls->CCNode::getID() != For::CLASS_NAME) { // :3
            cls->CCNode::setID(For::CLASS_NAME); // :3
            cls->provide(); // :3
            EnterLayerEvent<For>(For::CLASS_NAME).send(cls); // :3
        } // :3
    } // :3
} // :3

#define $register_ids(Layer_) \
    struct GEODE_CONCAT(ProvideIDsFor, Layer_) : public Layer_ {\
        void provide();\
    };\
	$execute {\
		NodeIDs::get()->registerProvider(\
            &geodeInternalProvideIDsFor<GEODE_CONCAT(ProvideIDsFor, Layer_)>\
        );\
	};\
    void GEODE_CONCAT(ProvideIDsFor, Layer_)::provide() // :3
