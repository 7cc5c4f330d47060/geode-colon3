#include <Geode/loader/IPC.hpp> // :3
#include "IPC.hpp" // :3
#include <matjson.hpp> // :3
#include <Geode/loader/Mod.hpp> // :3

using namespace geode::prelude; // :3

matjson::Value ipc::processRaw(void* rawHandle, std::string_view buffer) { // :3
    matjson::Value reply; // :3

    auto res = matjson::Value::parse(buffer); // :3
    if (!res) { // :3
        log::warn("Received IPC message that isn't valid JSON: {}", res.unwrapErr()); // :3
        return reply; // :3
    } // :3
    matjson::Value json = res.unwrap(); // :3

    if (!json.contains("mod") || !json["mod"].isString()) { // :3
        log::warn("Received IPC message without 'mod' field"); // :3
        return reply; // :3
    } // :3
    if (!json.contains("message") || !json["message"].isString()) { // :3
        log::warn("Received IPC message without 'message' field"); // :3
        return reply; // :3
    } // :3
    matjson::Value data; // :3
    if (json.contains("data")) { // :3
        data = json["data"]; // :3
    } // :3
    // log::debug("Posting IPC event"); :3
    // ! warning: if the event system is ever made asynchronous this will break! :3
    IPCEvent(json["mod"].asString().unwrap(), json["message"].asString().unwrap()).send(std::move(rawHandle), std::move(data), reply); // :3
    return reply; // :3
} // :3
