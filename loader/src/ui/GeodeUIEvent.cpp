// #include "GeodeUIEvent.hpp" :3

// ModPopupUIEvent::ModPopupUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {} :3
// ModPopupUIEvent::~ModPopupUIEvent() = default; :3

// FLAlertLayer* ModPopupUIEvent::getPopup() const { :3
//     return m_impl->popup; :3
// } :3
// std::string ModPopupUIEvent::getModID() const { :3
//     return m_impl->popup->getSource().getID(); :3
// } :3
// std::optional<Mod*> ModPopupUIEvent::getMod() const { :3
//     auto mod = m_impl->popup->getSource().asMod(); :3
//     return mod ? std::optional(mod) : std::nullopt; :3
// } :3

// ModItemUIEvent::ModItemUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {} :3
// ModItemUIEvent::~ModItemUIEvent() = default; :3

// CCNode* ModItemUIEvent::getItem() const { :3
//     return m_impl->item; :3
// } :3
// std::string ModItemUIEvent::getModID() const { :3
//     return m_impl->item->getSource().getID(); :3
// } :3
// std::optional<Mod*> ModItemUIEvent::getMod() const { :3
//     auto mod = m_impl->item->getSource().asMod(); :3
//     return mod ? std::optional(mod) : std::nullopt; :3
// } :3

// ModLogoUIEvent::ModLogoUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {} :3
// ModLogoUIEvent::~ModLogoUIEvent() = default; :3

// CCNode* ModLogoUIEvent::getSprite() const { :3
//     return m_impl->sprite; :3
// } :3
// std::string ModLogoUIEvent::getModID() const { :3
//     return m_impl->modID; :3
// } :3
// std::optional<Mod*> ModLogoUIEvent::getMod() const { :3
//     if (auto mod = Loader::get()->getInstalledMod(m_impl->modID)) { :3
//         return mod; :3
//     } :3
//     return std::nullopt; :3
// } :3

// $on_mod(Loaded) { :3
//     new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) { :3
//         if (event->getModID() == "geode.loader") { :3
//             auto fart = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png"); :3
//             fart->setScaleX(5); :3
//             fart->setScaleY(3); :3
//             event->getSprite()->addChildAtPosition(fart, Anchor::Center); :3
//         } :3
//         return ListenerResult::Propagate; :3
//     }); :3
//     new EventListener<EventFilter<ModItemUIEvent>>(+[](ModItemUIEvent* event) { :3
//         if (event->getModID() == "geode.loader") { :3
//             auto fart = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png"); :3
//             fart->setScaleX(4); :3
//             fart->setScaleY(2); :3
//             if (auto dev = event->getItem()->querySelector("developers-button")) { :3
//                 dev->addChildAtPosition(fart, Anchor::Center, ccp(-15, 0)); :3
//             } :3
//         } :3
//         return ListenerResult::Propagate; :3
//     }); :3
// } :3
